/* 
 * File:   at_engine.h
 * Author: Stephen Anthony
 *
 * Created on June 25, 2019, 8:22 AM
 */



#ifndef AT_ENGINE_H
#define	AT_ENGINE_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*-------------------------- HAL POINTERS ------------------------------------*/
/*       Serial write function prototypes depend on compiler/toolchain.       */

#if defined( __MIKROC_PRO_FOR_ARM__ )       || \
    defined( __MIKROC_PRO_FOR_DSPIC__ )     || \
    defined( __MIKROC_PRO_FOR_PIC32__ )
typedef void ( *ENGINE_uartWriteHandler )(unsigned int data_out);

#elif defined( __MIKROC_PRO_FOR_AVR__ )     || \
      defined( __MIKROC_PRO_FOR_PIC__ )     || \
      defined( __MIKROC_PRO_FOR_8051__ )    || \
      defined( __MIKROC_PRO_FOR_FT90x__ )
typedef void ( *ENGINE_uartWriteHandler )(unsigned char data_out);
#else
typedef void ( *ENGINE_uartWriteHandler )(unsigned char);
typedef uint8_t ( *ENGINE_uartReadHandler  )(void);
typedef uint8_t ( *ENGINE_countRxBufferHandler)(void);
typedef void    ( *ENGINE_delayInMilliSecHandler)(uint32_t);
#endif

/*----------------------------------------------------------------------------*/

uint8_t engine_initiate(ENGINE_uartWriteHandler write_handler, ENGINE_uartReadHandler read_handler, 
        ENGINE_countRxBufferHandler count_unread_chard_handler,
        ENGINE_delayInMilliSecHandler delay_in_ms);

static void send_text_via_uart(const char* data_out);

static void retrieve_uart_buffer_content(char *const);

/**
 * @brief Simple AT Command
 *
 * @param[in] pCmd          pointer to command string
 *
 * Function should be used in case of simple AT commands which have no
 * additional arguments expected. Most of the AT Commands uses this function.
 */
void execute_atcmd(const char *pCmd, ...);

uint8_t get_ipd_data(char *const, uint8_t);

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif	/* AT_ENGINE_H */



