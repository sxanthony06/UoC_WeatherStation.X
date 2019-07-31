#include "at_parser.h"
#include <stddef.h>
#include <string.h>
#include "utilities.h"

/* 
 * Handler vars 
 */
static T_AT_handler                 fpHandler;
static unsigned char                flags;

/*
 * Handlers Storage
 *      counter / size / pointer
 */
static uint16_t                     handlers_store_counter;
static uint16_t                     handlers_store_size;
static T_AT_storage*                handlers_store;

static char CMD_TYPE_LOOKUPTABLE[3][3] =
{
    "=?",           //AT_CMD_TYPE_TEST
    "?",            //AT_CMD_TYPE_QUERY
    "=",            //AT_CMD_TYPE_SET
};

/*-----------------------------------------------------------------------------            
        Private Function Implementations
 ------------------------------------------------------------------------------*/

static uint32_t makeHash(const char *pCmd )
{
    uint16_t ch;
    uint32_t hash;

    ch = 0;
    hash = 0x05;
    while((ch = *pCmd++))
        hash += (ch << 1);
    return hash;
}

static uint8_t findHandler(const char* parsed_atcmd )
{
    uint8_t     len;
    uint8_t    counter;
    uint32_t    hash;

    len = strlen(parsed_atcmd);
    hash = makeHash(parsed_atcmd);
    for(counter = 0; counter < handlers_store_counter; counter++)
        if(handlers_store[counter].len == len)
            if(handlers_store[counter].hash == hash)
                return counter;
    return 0;
}

/*-----------------------------------------------------------------------------
        Public Function Implementations
 ------------------------------------------------------------------------------*/

void init_atcmd_parser(T_AT_handler pHandler, uint16_t cmdTimeout, T_AT_storage* pStorage){
    T_AT_storage cmd;

    cmd.handler = pHandler;
  
    memset(cmd.at_cmd_type_timeouts, 0, sizeof(cmd.at_cmd_type_timeouts));
  
    cmd.at_cmd_type_timeouts[AT_CMD_TYPE_EXECUTE]   = cmdTimeout;
    cmd.hash    = makeHash("");
    cmd.len = 0;
    handlers_store_counter   = 0;
    handlers_store  = pStorage;
    
    // TODO: Find out if use of void pointer is necessary of even correct
    memset((void*)handlers_store, 0, sizeof(pStorage) * sizeof(T_AT_storage));
    handlers_store[handlers_store_counter] = cmd;
    handlers_store_counter++;
}

void parse_input_for_atcmd(const char *at_cmd, T_AT_handler *pHandler, uint16_t *pTimeout, uint8_t *matched_atcmd_type)
{
    uint8_t handler_id;
    uint8_t pos, start_pos_cmd;
    uint8_t lut_entry, at_cmd_type_lut_size;
    char    tmp[ AT_CMD_MAX_LENGTH + 1 ];
    const char * substr_to_parse_str;
    
    pos = 0;
    start_pos_cmd = AMOUNT_OF_CHAR_TO_IGNORE_IN_ATCMD;
    handler_id = 0;
    *matched_atcmd_type = 0;                  
    at_cmd_type_lut_size = sizeof(CMD_TYPE_LOOKUPTABLE)/sizeof(CMD_TYPE_LOOKUPTABLE[0]);
    memset( tmp, 0, AT_CMD_MAX_LENGTH + 1 );
    
    for(lut_entry = 0; lut_entry < at_cmd_type_lut_size; lut_entry++){
        /* Function strstr returns a pointer to the first occurrence of the LUT_CMD_TYPE e.g. '=', '=?' in parameter 'at_cmd'.
         * Pointer arithmetic is used to calculate amount of chars between start of string and first occurrence
         * of LUT_CMD_TYPE. This amount is used to extract the command from to_parse_str.
         */
        //TODO: Implement more robust way to find type of at_cmd
        substr_to_parse_str = strstr((const char *)at_cmd, (const char *)CMD_TYPE_LOOKUPTABLE[lut_entry]);
        if(substr_to_parse_str != 0){
            strncpy(tmp, (const char*)at_cmd + start_pos_cmd, substr_to_parse_str - at_cmd - start_pos_cmd);
            break;
        }
    }
    *matched_atcmd_type = lut_entry;

    
    handler_id = findHandler(tmp);
    *pHandler = handlers_store[handler_id].handler;
    if((*pTimeout = handlers_store[handler_id].at_cmd_type_timeouts[*matched_atcmd_type]) == 0) 
        *pTimeout = handlers_store[0].at_cmd_type_timeouts[AT_CMD_TYPE_EXECUTE];
}

uint8_t save_atcmd_handler( char *str, uint8_t at_cmd_type, uint16_t timeout, T_AT_handler pHandler )
{
    T_AT_storage cmd;
    
    memset(cmd.at_cmd_type_timeouts, 0, sizeof(cmd.at_cmd_type_timeouts));    

    if(!pHandler)
        pHandler = handlers_store[0].handler;
    if(!timeout)
        timeout = handlers_store[0].at_cmd_type_timeouts[AT_CMD_TYPE_EXECUTE];
    cmd.len = strlen(str);
    if(cmd.len >= AT_CMD_MAX_LENGTH)
        return 0;
    if(handlers_store_counter == handlers_store_size)
        return 0;
    if(findHandler(str))
        return 0;
    cmd.hash                                = makeHash(str);
    cmd.at_cmd_type_timeouts[at_cmd_type]   = timeout;
    cmd.handler                             = pHandler;
    handlers_store[handlers_store_counter]    = cmd;
    handlers_store_counter++;
    return (handlers_store_size - handlers_store_counter);
}

void save_atcmd_timeout(const char* at_cmd, uint8_t at_cmd_type, uint16_t timeout){
    uint8_t handler_id;
    handler_id = findHandler(at_cmd);
    handlers_store[handler_id].at_cmd_type_timeouts[at_cmd_type] = timeout;
}