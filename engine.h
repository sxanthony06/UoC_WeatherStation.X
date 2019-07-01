/* 
 * File:   at_engine.h
 * Author: Stephen Anthony
 *
 * Created on June 25, 2019, 8:22 AM
 */

#include "xc.h"
#include "device_config.h"
#include "eusart1.h"
#include "at_parser.h"

#ifndef AT_ENGINE_H
#define	AT_ENGINE_H

#ifdef	__cplusplus
extern "C" {
#endif

/*-------------------------- HAL POINTERS ------------------------------------*/
/*       Serial write function prototypes depend on compiler/toolchain.       */

#if defined( __MIKROC_PRO_FOR_ARM__ )       || \
    defined( __MIKROC_PRO_FOR_DSPIC__ )     || \
    defined( __MIKROC_PRO_FOR_PIC32__ )
typedef void ( *T_AT_UART_Write )(unsigned int data_out);

#elif defined( __MIKROC_PRO_FOR_AVR__ )     || \
      defined( __MIKROC_PRO_FOR_PIC__ )     || \
      defined( __MIKROC_PRO_FOR_8051__ )    || \
      defined( __MIKROC_PRO_FOR_FT90x__ )
typedef void ( *T_AT_UART_Write )(unsigned char data_out);
#else
typedef void ( *T_AT_UART_Write )(unsigned char data_out);
#endif

/*----------------------------------------------------------------------------*/

void init_engine(T_AT_UART_Write fp_uart_write);

static void send_text_via_uart(const char* str);

/**
 * @brief Simple AT Command
 *
 * @param[in] pCmd          pointer to command string
 *
 * Function should be used in case of simple AT commands which have no
 * additional arguments expected. Most of the AT Commands uses this function.
 */
void execute_atcmd(const char *pCmd );

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif	/* AT_ENGINE_H */



