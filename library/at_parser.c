#include "at_parser.h"

/* 
 * Handler vars 
 */
static T_AT_handler                 fpHandler;
static unsigned char                flags;

/*
 * Handlers Storage
 *      counter / size / pointer
 */
static uint16_t                     handler_store_counter;
static uint16_t                     handler_store_size;
static T_AT_storage*                handler_store;

static char CMD_TYPE_LUT[3][3] =
{
    "=?",           // Test
    "?",            // Get
    "=",            // Set
};

/*-----------------------------------------------------------------------------            
        Private Function Implementations
 ------------------------------------------------------------------------------*/

static uint32_t makeHash( char *pCmd )
{
    uint16_t ch;
    uint32_t hash;

    ch = 0;
    hash = 0x05;
    while((ch = *pCmd++))
        hash += (ch << 1);
    return hash;
}

static uint8_t findHandler( char* parsed_atcmd )
{
    uint8_t     len;
    uint8_t    counter;
    uint32_t    hash;

    len = strlen(parsed_atcmd);
    hash = makeHash(parsed_atcmd);
    for(counter = 0; counter < handler_store_counter; counter++)
        if(handler_store[counter].len == len)
            if(handler_store[counter].hash == hash)
                return counter;
    return 0;
}

/*-----------------------------------------------------------------------------
        Public Function Implementations
 ------------------------------------------------------------------------------*/

void init_atcmd_parser(T_AT_handler pHandler, uint32_t cmdTimeout, T_AT_storage* pStorage){
    T_AT_storage cmd;

    cmd.handler     = pHandler;
    cmd.timeout     = cmdTimeout;
    cmd.hash        = makeHash("");
    cmd.len         = 0;
    handler_store_counter      = 0;
    handler_store  = pStorage;
    
    memset((void*)handler_store, 0, sizeof(pStorage) * sizeof(T_AT_storage));
    handler_store[handler_store_counter] = cmd;
    handler_store_counter++;
}

void parse_input_for_atcmd(const char *to_parse_str, T_AT_handler *pHandler, uint16_t *pTimeout, uint8_t *pMatched_atcmd_type)
{
    uint8_t handler_id;
    uint8_t pos, start_pos_cmd;
    uint8_t lut_id, lut_size;
    char    tmp[ _AT_CMD_MAXSIZE + 1 ];
    const char * substr_to_parse_str;
    
    pos = 0;
    start_pos_cmd = 2;
    handler_id = 0;
    *pMatched_atcmd_type = 0;
    lut_size = sizeof(CMD_TYPE_LUT)/sizeof(CMD_TYPE_LUT[0]);
    memset( tmp, 0, _AT_CMD_MAXSIZE + 1 );
    
    for(lut_id = 0; lut_id < lut_size; lut_id++){
        /* Function strstr returns a pointer to the first occurrence of the LUT_CMD_TYPE e.g. '=', '=?' in to_parse_string.
         * Pointer arithmetic is used to calculate amount of chars between start of string and first occurrence
         * of LUT_CMD_TYPE. This amount is used to extract the command from to_parse_str.
         */
        substr_to_parse_str = strstr((const char *)to_parse_str, (const char *)CMD_TYPE_LUT[lut_id]);
        if(substr_to_parse_str != 0){
            strncpy(tmp, (const char*)to_parse_str + start_pos_cmd, substr_to_parse_str - to_parse_str - start_pos_cmd);
            *pMatched_atcmd_type = lut_id;
            break;
        }
    }
    
    handler_id = findHandler(tmp);
    *pHandler = handler_store[handler_id].handler;
    *pTimeout = handler_store[handler_id].timeout;
}

uint8_t save_atcmd_handler( char *str, uint32_t timeout, T_AT_handler pHandler )
{
    T_AT_storage cmd;

    if(!pHandler)
        pHandler = handler_store[0].handler;
    if(!timeout)
        timeout = handler_store[0].timeout;
    cmd.len = strlen(str);
    if(cmd.len >= _AT_CMD_MAXSIZE)
        return 0;
    if(handler_store_counter == handler_store_size)
        return 0;
    if(findHandler(str))
        return 0;
    cmd.hash        = makeHash(str);
    cmd.timeout     = timeout;
    cmd.handler     = pHandler;
    handler_store[handler_store_counter] = cmd;
    handler_store_counter++;
    return (handler_store_size - handler_store_counter);
}