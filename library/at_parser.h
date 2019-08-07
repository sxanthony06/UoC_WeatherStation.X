/*

    at_engine.h

    Copyright (c) 2011-2017 MikroElektronika.  All right reserved.

------------------------------------------------------------------------------*/

/**
 * @file                                            	at_engine.h
 * @brief                     AT Parser
 *
 * @mainpage AT Parser
 *
 * @{
 *
 * ### Library Description ###
 *
 * Generic AT Parser usable on various architectures.
 *
 * ### Features ###
 *
 *    - Timer based engine
 *    - External buffer
 *    - External handler storage
 *    - External defined HFC functions
 *
 * @note In case of hardware flow control (HFC) usage definition of the
 * external RTS/CTS control and check functions is needed.
 *
 * @}
 *
 * @defgroup AT
 * @brief        * @{
 *
 * Global Library Prefix : **AT**
 *
 * @example C4G_Click_ARM_KIN.c
 * @example C4G_Click_ARM_STM.c
 * @example C4G_Click_PIC.c
 * @example C4G_Click_PIC32.c
 * @example C4G_Click_DSPIC.c
 *
 ******************************************************************************/

#ifndef _AT_PARSER_H_
#define _AT_PARSER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
    
/**
 * @name        Configuration defs
 *
 * Configuration preprocessors.
 *
 ******************************************************************************/
///@{
	
/** Max command size excluding command args */
 
///@}
/**
 * @name                 AT Parser Types
 ******************************************************************************/
///@{
    
/**
 * @typedef Handler Prototype
 */
typedef void (* T_AT_handler )(const char* buffer, uint8_t type );

/**
 * @brief Parser Structure
 *
 * Struct is used for storing the command with timeout and callbacks.
 * Command strings are converted to the hash code for easiest comparison.
 */
typedef struct
{
    /** Command Length */
    uint16_t                    len;
    /** Command Hash Value */
    uint32_t                    hash;
    /** Command Timeout */
    uint16_t                    at_cmd_type_timeouts[4];
    /** Get Callback */
    T_AT_handler                handler;

}T_AT_storage;


///@}
/**
 * @name                 Private Functions' Definitions
 ******************************************************************************/
///@{



/*
 * Simple Hash code generation
 *
 * Hash code is used to save the command to the storage in aim to have fixed
 * storage space for all functions.
 */
static uint32_t makeHash(const char *pCmd );

/*
 * Search handler storage for provided command
 *
 * Function search the storage based on sting length and hash code.
 * If function returns zero command does not exists in storage area.
 */
static uint8_t findHandler(const char* pCmd );

/*
 * Search input for strings from LUT table.
 * LUT table must be 2 dimensional char array.
 *
 * Depend of flag returned value is :
 * - index of found string at LUT
 * - found string offset inside input
 * - (-1) no match
 */
static int searchLut( volatile unsigned char* pInput, char (*pLut)[ 3 ], int lutSize, int flag );

///@}
/**
 * @name                 AT Global Functions' Definitions
 ******************************************************************************/
///@{

/**
 * @brief AT Parser Initialization
 *
 * @param[in] pWrite            UARTx_Write function
 * @param[in] pHandler          default handler
 * @param[in] cmdTimeout        default timeout
 * @param[in] pBuffer           response buffer
 * @param[in] bufferSize        size of the buffer in bytes
 * @param[in] pStorage          handler storage
 * @param[in] storageSize       handler storage size
 *
 * Initialization should be executed before any other function. User can
 * execute this function later inside the application to reset AT Engine to
 * the default state.
 */
void parser_initiate(T_AT_handler pHandler, uint16_t cmdTimeout, T_AT_storage* pStorage);

/**
 * @brief Receive
 *
 * @param[in] rxInput       char received from the module
 *
 * Function is used to populate response buffer with characters received from
 * the module. This function should be placed inside UART ISR function.It also
 * can be used inside function which constatnly poll the UART RX register.
 */

/*
 * Parsing
 *
 * @param[in] char* input - AT Command
 * @param[out] at_cmd_cb* cb - handler pointer for the particular command
 * @param[out] uint32_t* timeout - timeout for the particular command
 *
 * Function parses provided raw command string and returns previously saved
 * handler and timeout for the particular command. If command is not found
 * the default handler and default timeout will be returned.
 */
void parse_input_for_atcmd(const char *pInput, T_AT_handler *pHandler, uint16_t *timeout, uint8_t *matched_type);

/**
 * @brief Engine Tick
 *
 * Function must be placed inside the Timer ISR function which will be called
 * every one millisecond.
 */
void AT_tick();

/**
 * @brief Save AT Command to Storage
 *
 * @param[in] pCmd          command string
 * @param[in] timeout       timeout for provided command
 * @param[in] pHandler      handler for provided command
 *
 * Saves handlers and timeout for the particular command.
 */
uint8_t save_atcmd_handler(char*, uint8_t, uint16_t, T_AT_handler);

void save_atcmd_timeout(const char*, uint8_t, uint16_t);



#ifdef __cplusplus
} // extern "C"
#endif

#endif