#include "engine.h"

/* Write pointer */
static T_AT_UART_Write uart_write_byte;

void init_engine(T_AT_UART_Write fp_uart_write){
    uart_write_byte = fp_uart_write;
}

static void send_text_via_uart(const char* str){
#ifdef _DEBUG_
        LOG( "< WR >\r\n" );
#endif
    while(*str)
    {
#if( _AT_HFC_CONTROL )
        while(!AT_getStateDCE());
#endif
        uart_write_byte(*str++);
    }
    uart_write_byte(13);
    uart_write_byte(10);

#ifdef _DEBUG_
    LOG_CH(delimiter);
    LOG("\r\n");
#endif
}

void execute_atcmd(const char *atcmd )
{
    T_AT_handler atcmd_callback;
    uint16_t timeout;
    uint8_t matched_atcmd_type, at_cmd_success, atcmd_rep_remaining, tmr0_rep_remaining;
    uint8_t rx_storage[45];
    
    atcmd_rep_remaining = 3;
    at_cmd_success = 0;
    memset(rx_storage, 0, sizeof(rx_storage));
    
    do {
        parse_input_for_atcmd(atcmd, &atcmd_callback, &timeout, &matched_atcmd_type);
        send_text_via_uart(atcmd);    

        for(tmr0_rep_remaining = 0; tmr0_rep_remaining < (timeout/100); tmr0_rep_remaining++){
            __delay_ms(100);
        }

        EUSART1_Flush_RxBuffer((char *const)rx_storage);
   
        tmr0_rep_remaining--;
    }while(tmr0_rep_remaining > 0 && !strstr((const char*)rx_storage, "OK"));

    /* TODO:
        Implement code that puts microcontroller in low-power standby mode if
        AT commands still fails after 3 intents.*/
    
    /* TODO:
        Next statement makes code tightly coupled. If cmd_types_lut in at_parser.c
        changes, following statement will probably give erroneous result. There needs
        to be a way to use cmd_types_lut's own content to make comparison against 
        matched_atcmd_type.*/
    if(matched_atcmd_type > 1)
        atcmd_callback((const char*)rx_storage, matched_atcmd_type);
}
