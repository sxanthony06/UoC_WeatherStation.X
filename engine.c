#include "engine.h"
#include "utilities.h"
#include <stdlib.h>
#include "at_parser.h"
#include <string.h>

/* Write pointer */
static ENGINE_uartWriteHandler uart_transmit;
static ENGINE_uartReadHandler uart_receive;
static ENGINE_countRxBufferHandler get_count_unread_chars;
static ENGINE_delayInMilliSecHandler delay_in_ms;

static char eusart_rx_buffer[EUSART1_RX_BUFFER_SIZE];

uint8_t engine_initiate(ENGINE_uartWriteHandler tmp_write_handler, ENGINE_uartReadHandler tmp_read_handler, 
        ENGINE_countRxBufferHandler tmp_count_unread_chars_handler,
        ENGINE_delayInMilliSecHandler tmp_delay_in_ms){
    
    //Check for null in function pointers
    if(!(tmp_write_handler && tmp_read_handler && tmp_count_unread_chars_handler && tmp_delay_in_ms))
        return 0;
    
    uart_transmit = tmp_write_handler;
    uart_receive = tmp_read_handler;
    get_count_unread_chars = tmp_count_unread_chars_handler;
    delay_in_ms = tmp_delay_in_ms;
    return 1;
}

//TODO: Remove unnecessary code
static void send_text_via_uart(const char* str){
#ifdef _DEBUG_
        LOG( "< WR >\r\n" );
#endif
    while(*str)
    {
#if( _AT_HFC_CONTROL )
        while(!AT_getStateDCE());
#endif
        uart_transmit(*str++);
    }
    uart_transmit(13);
    uart_transmit(10);

#ifdef _DEBUG_
    LOG_CH(delimiter);
    LOG("\r\n");
#endif
}

static void retrieve_uart_unread_chars
(char *const buffer)
{
    uint8_t counter, count_unread_chars;
    
    count_unread_chars = get_count_unread_chars();
    for(counter = 0; counter < count_unread_chars; counter++){
        buffer[counter] = uart_receive();
        // In case there is more unread chars than space in parameter buffer, rollover to beginning of buffer
        if((count_unread_chars > EUSART1_RX_BUFFER_SIZE) && counter == EUSART1_RX_BUFFER_SIZE-1){
            counter = 0;
            count_unread_chars = count_unread_chars - EUSART1_RX_BUFFER_SIZE;
        }
    }
}

//TODO: Implement EUSART function that limits its return to the latest AT command response
void execute_atcmd(const char *atcmd, ...)
{
    T_AT_handler atcmd_callback;
    uint16_t timeout = 0;
    uint8_t matched_atcmd_type;
    
    memset(eusart_rx_buffer, 0, sizeof(eusart_rx_buffer));
    parse_input_for_atcmd(atcmd, &atcmd_callback, &timeout, &matched_atcmd_type);
    send_text_via_uart(atcmd);    
    

    
    delay_in_ms(timeout);
    
    // Copy EUSART1 buffer content since the buffer may be manipulated during EUSART1 interrupts
    retrieve_uart_unread_chars((char *const)eusart_rx_buffer);
    atcmd_callback((const char*)eusart_rx_buffer, matched_atcmd_type);
}

//TODO: Implement new version of this that looks for +IPD within whole UART buffer and not just portion with unread characters
uint8_t get_ipd_data(char *const tmp_ipd_data_buf, uint8_t max_len){
    char tmp_char_count_buf[2];                                      // <len> of +IPD data is send as array of chars. Conversion is necessary, also a buf[2] implies a max string of 99 chars
    uint8_t recv_data_len;
    uint8_t ipd_data_delimiter_pos = 0;
    
    memset(tmp_ipd_data_buf, 0, max_len);
    
    do{
        while(get_count_unread_chars() == 0)
            delay_in_ms(50);        
        memset(eusart_rx_buffer, 0, strlen(eusart_rx_buffer));
        retrieve_uart_unread_chars((char *const)eusart_rx_buffer);
    }while(!strstr((const char*)eusart_rx_buffer, "+IPD"));
                                    
    while(eusart_rx_buffer[++ipd_data_delimiter_pos] != ':' && ipd_data_delimiter_pos < EUSART1_RX_BUFFER_SIZE);       // IPD <data> is preceded by ':' symbol e.g. +IPD, 0, <len>: <data>. This ':' will function as delimiter to find length (<len>) of data and actual data 
    if(ipd_data_delimiter_pos >= EUSART1_RX_BUFFER_SIZE)
        return 0;
    strncpy((char*)tmp_char_count_buf, eusart_rx_buffer + (ipd_data_delimiter_pos-2), 2);
    if(tmp_char_count_buf[0] == ','){
        recv_data_len = tmp_char_count_buf[1] - 48;     
    }else{
        recv_data_len = ((tmp_char_count_buf[0] - 48) * 10) + (tmp_char_count_buf[1] - 48);      
    }
    if(recv_data_len > max_len)                                                              // If IPD <data> is too long, return 0 (unsuccessful)
        return 0;
    strncpy((char*)tmp_ipd_data_buf, eusart_rx_buffer + (ipd_data_delimiter_pos + 1), recv_data_len);
    return 1;
}
