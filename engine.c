#include "engine.h"

/* Write pointer */
static T_AT_UART_Write uart_write_byte;
static T_AT_UART_Read uart_read_byte;

void init_engine(T_AT_UART_Write write_handler, T_AT_UART_Read read_handler){
    uart_write_byte = write_handler;
    uart_read_byte = read_handler;
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

static void retrieve_uart_buffer_content(char *const buffer)
{
    uint8_t tmp, i;
    for(i = 0; (tmp = uart_read_byte()) != 0; i++){
        buffer[i] = tmp;
    }
}

void execute_atcmd(const char *atcmd )
{
    T_AT_handler atcmd_callback;
    uint16_t timeout;
    uint8_t matched_atcmd_type, i;
    uint8_t eusart_rx_buffer[45];
    
    memset(eusart_rx_buffer, 0, sizeof(eusart_rx_buffer));
    
    parse_input_for_atcmd(atcmd, &atcmd_callback, &timeout, &matched_atcmd_type);
    send_text_via_uart(atcmd);    
    
    for(i = 0; i < (timeout/100); i++){
        __delay_ms(100);
    }

    retrieve_uart_buffer_content((char *const)eusart_rx_buffer);
    atcmd_callback((const char*)eusart_rx_buffer, matched_atcmd_type);
}
