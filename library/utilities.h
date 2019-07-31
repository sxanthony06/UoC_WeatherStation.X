/* 
 * File:   utilities.h
 * Author: sxant
 *
 * Created on July 21, 2019, 9:36 PM
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
  Section: Macro constants declarations
*/

// EUSART1 declarations
#define EUSART1_TX_BUFFER_SIZE 45
#define EUSART1_RX_BUFFER_SIZE 90

    
// EEPROM declarations 
#define EEPROM_STRING_MAX_LENGTH  16
#define EEPROM_SERVER_PORT_MAX_LENGTH 5


// TMR1 declarations    
#define PULSE_WIDTH_MEASUREMENTS_SIZE 40

/**
  Section: Macro functions declarations
*/

#define testbit(var, bit) ((var) & (1 <<(bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))
    
#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))    

#define AT_CMD_TYPE_TEST    0
#define AT_CMD_TYPE_QUERY   1
#define AT_CMD_TYPE_SET     2
#define AT_CMD_TYPE_EXECUTE 3

#define AMOUNT_OF_CHAR_TO_IGNORE_IN_ATCMD 2
    
#define AT_CMD_MAX_LENGTH   45			    

// AT COMMAND results
enum at_cmd_results{SUCCESS, FAILURE, ERROR};

// AP and server connection status
enum conn_status{UNDEFINED, CONNECTED, DISCONNECTED};    
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */

