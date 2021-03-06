/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F45K22
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "engine.h"
#include "bmp180.h"
#include "at_parser.h"
#include <string.h>
#include <stdio.h>
#include "utilities.h"
#include <stdbool.h>


static enum at_cmd_results latest_atcmd_result;
static enum conn_status wlan_conn_status, server_conn_status;
static volatile uint16_t count_tmr0_rollovers = 0;

typedef struct common_net_info{
    uint8_t start_pos;
    uint8_t maxsize;
    uint8_t len;
    char value[EEPROM_STRING_MAX_LENGTH];
} cni;

static void default_ATCMD_response_parser(const char*, unsigned char );
static void check_WLAN_connection_callback(const char*, unsigned char );
static void check_server_conn_callback(const char*, unsigned char);
static uint8_t fetch_WLAN_data_eeprom(cni*, cni*);
static uint8_t fetch_server_data_eeprom(cni*, cni*);
static void clear_buffer(uint8_t, size_t,...);
static void dht11_process_curent_sensor_readings(float* dht11_temperature, float* dht11_humidity, uint8_t attempts);
static uint8_t acquire_WLAN_info(cni*, cni*);
static uint8_t acquire_server_info(cni*, cni*);
static void acquire_info_for_server_connection_via_tcp_server(cni*, cni*, cni*, cni*, uint16_t);
static uint8_t resolve_recv_net_info(cni*, cni*, cni*, cni*);
static void boot_esp01_tcpserver(void);
static void connect_to_WLAN(const char*, const char*);
static void connect_to_server(const char*, const char*);
static void write_WLAN_data_to_EEPROM(cni*, cni*);
static void write_server_data_to_EEPROM(cni*, cni*);
static void increment_tmr0_rollover_count(void);


/*
                         Main application
 */
void main(void)
{
    T_AT_storage handlers_store[5];
    float dht11_temperature, dht11_humidity;

    cni wlan_ssid = {0, EEPROM_STRING_MAX_LENGTH, 0};
    cni wlan_passw = {wlan_ssid.start_pos + wlan_ssid.maxsize + 1, EEPROM_STRING_MAX_LENGTH, 0};
    cni server_ip = {wlan_passw.start_pos + wlan_passw.maxsize + 1, EEPROM_STRING_MAX_LENGTH, 0};
    cni server_port = {server_ip.start_pos + server_ip.maxsize + 1, EEPROM_SERVER_PORT_MAX_LENGTH, 0};

    memset(wlan_ssid.value, 0, wlan_ssid.maxsize);
    memset(wlan_passw.value, 0, wlan_passw.maxsize);
    memset(server_ip.value, 0, server_ip.maxsize);
    memset(server_port.value, 0, server_port.maxsize);
    
    struct bmp180_dev dev;
    dev.dev_id = BMP180_I2C_ADDR;
    dev.delay_ms = SYSTEM_DELAY_IN_MS;
    dev.read = I2C_read;
    dev.write = I2C_write;

    // Configure and initialize the device. POWER_LED and ESP01_CHPD_pin are set HIGH during PIN_MANAGER_Initialize()
    SYSTEM_Initialize();
    __delay_ms(2000);   

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();


    engine_initiate(EUSART1_Write, EUSART1_Read, EUSART1_is_rx_ready, SYSTEM_DELAY_IN_MS);
    parser_initiate(default_ATCMD_response_parser, 100, handlers_store);
    TMR0_SetInterruptHandler(increment_tmr0_rollover_count);
    bmp180_init(&dev);
    
    save_atcmd_handler("+CWJAP_CUR", AT_CMD_TYPE_SET, 7000, check_WLAN_connection_callback);
    save_atcmd_handler("+CIPSTART", AT_CMD_TYPE_SET, 2000, check_server_conn_callback);     
    
    execute_atcmd("ATE0");
    execute_atcmd("AT+CWMODE_DEF=1");           // On power-up ESP01 sets into last configured Station/AP mode (even if not written to FLASH). This AT command defaults it to station mode, consuming less power.
    execute_atcmd("AT+CIPSNTPCFG=1,-4,\"0.pool.ntp.org\",\"1.pool.ntp.org\",\"3.pool.ntp.org\"");   // Configure device's location timezone and network time servers (to get time).
    execute_atcmd("AT+CWQAP");
    execute_atcmd("AT+CWJAP_CUR?");
 

    if(wlan_conn_status != CONNECTED){
        if(fetch_WLAN_data_eeprom(&wlan_ssid, &wlan_passw) & fetch_server_data_eeprom(&server_ip, &server_port)){
            connect_to_WLAN(wlan_ssid.value, wlan_passw.value);

            execute_atcmd("AT+CWAUTOCONN=1");   // Configure ESP8266 to use uploaded info to connect automatically to AP on power-up
            connect_to_server(server_ip.value, server_port.value);
        }else{
            TMR0_Reload();
            TMR0_StartTimer();
            acquire_info_for_server_connection_via_tcp_server(&wlan_ssid, &wlan_passw, &server_ip, &server_port, 600);
            TMR0_StopTimer();
            if(wlan_conn_status != CONNECTED)
                //TODO: Implement function to put PIC to sleep
                NOP();
            }
    }
    if(server_conn_status != CONNECTED){
        if(fetch_server_data_eeprom(&server_ip, &server_port)){
            connect_to_server(server_ip.value, server_port.value);
        }
    }

    dht11_process_curent_sensor_readings(&dht11_humidity, &dht11_temperature, 2);
    
    while (1)
    {
        __delay_ms(1000);
    }
}

/* Callback function specifically for handling (ESP8266) execute commands that
 return "OK" on success or "ERROR".*/
static void default_ATCMD_response_parser(const char* atcmd_resp, unsigned char type){
    char * tmp;

    tmp = strstr(atcmd_resp, "OK");
    latest_atcmd_result = (tmp != 0)? SUCCESS : ERROR;
}

static void check_WLAN_connection_callback(const char* atcmd_resp, unsigned char type){
    // ONLY SET and QUERY types are defined for AT+CWJAP_CUR as of ESSPRESSIF ESP8266 AT instruction set v1.6.2
    char * x;
    char * y;
    if(type == AT_CMD_TYPE_QUERY){
        //TODO: strstr is not case-insensitive. Use an algorithm that makes all char in char array 
        //  lowercase on atcmd and than a search for 'no ap' or 'error' or relevant string
        x = strstr(atcmd_resp, "No AP");
        y = strstr(atcmd_resp, "ERROR");
        if(y != 0){
            latest_atcmd_result = FAILURE;
            wlan_conn_status = UNDEFINED;
        }else if(x != 0){
            latest_atcmd_result = SUCCESS;
            wlan_conn_status = DISCONNECTED;
        }else {
            latest_atcmd_result = SUCCESS;
            wlan_conn_status = CONNECTED;
        }
    }else if(type == AT_CMD_TYPE_SET){
        x = strstr(atcmd_resp, "FAIL");
        y = strstr(atcmd_resp, "ERROR");
        if(y != 0){
            latest_atcmd_result = FAILURE;
            wlan_conn_status = UNDEFINED;
        }else if(x != 0){
            latest_atcmd_result = SUCCESS;
            wlan_conn_status = DISCONNECTED;
        }else {
            latest_atcmd_result = SUCCESS;
            wlan_conn_status = CONNECTED;
        }        
    }
}

static void check_server_conn_callback(const char* atcmd_resp, unsigned char type){
    char  *x, *y;
    x = strstr(atcmd_resp, "CONNECT");      // Function strstr searches for 'CONNECT' in atcmd_resp, so x will be unline NULL also for 'ALREADY CONNECTED'. See strstr() in string.h for more info
    y = strstr(atcmd_resp, "ERROR");
    if(y != 0){
        latest_atcmd_result = SUCCESS;
        server_conn_status = DISCONNECTED;
    }else if(x != 0){
        latest_atcmd_result = SUCCESS;
        server_conn_status = CONNECTED;
    }
}

static void parse_sntp_output_callback(const char* atcmd_resp, unsigned char type){
    char* token;
    
    token = strchr(atcmd_resp, ':');    
    token = strtok(token+1, "\r\n");        
    if(token != NULL){
        latest_atcmd_result = SUCCESS;
        strncpy(date_in_string, token, DATE_STRING_MAX_LENGTH);
    }else{
        latest_atcmd_result = ERROR;
    }
}

static uint8_t fetch_WLAN_data_eeprom(cni* ap_ssid, 
        cni *ap_passw){
    uint8_t i;

    ap_ssid->len = DATAEE_ReadByte(ap_ssid->start_pos);
    ap_passw->len = DATAEE_ReadByte(ap_passw->start_pos);

    if(ap_ssid->len > EEPROM_STRING_MAX_LENGTH || ap_passw->len > EEPROM_STRING_MAX_LENGTH)
        return 0;
    for(i = 0; i < ap_ssid->len; i++){
        ap_ssid->value[i] = DATAEE_ReadByte(i + 1 + ap_ssid->start_pos);
    }
    for(i = 0; i < ap_passw->len; i++){
        ap_passw->value[i] = DATAEE_ReadByte(i + 1 + ap_passw->start_pos );
    }
    return 1;
}

static uint8_t fetch_server_data_eeprom(cni *server_ip, cni *server_port){
    uint8_t i;
    server_port->len = DATAEE_ReadByte(server_port->start_pos);
    server_ip->len = DATAEE_ReadByte(server_ip->start_pos);

    if(server_ip->len > EEPROM_STRING_MAX_LENGTH || server_port->len > EEPROM_SERVER_PORT_MAX_LENGTH)
        return 0;
    
    for(i = 0; i < server_ip->len; i++){
        server_ip->value[i] = DATAEE_ReadByte(i + 1 + server_ip->start_pos);
    }
    for(i = 0; i < server_port->len; i++){
        server_port->value[i] = DATAEE_ReadByte(i + 1+ server_port->start_pos);
    }
    return 1;
}

/*TODO: Implement function that take a list of argument of varrying size, checks for
    nulls (in pointers) and runs memset on the arrays.*/
static void clear_buffer(uint8_t filler, size_t size,...){

}

static uint8_t dht11_process_curent_sensor_readings(float* dht11_temperature, 
        float* dht11_humidity){
    
    uint8_t data_in_bytes[5], b, i, tmp, s, rslt = 0;
    uint16_t hum_tmp = 0, temp_tmp = 0, measurement;
    volatile const uint16_t* dht_measurements;
    
    tmp = 0;
    s = sizeof(data_in_bytes);
    memset(data_in_bytes, 0, sizeof(data_in_bytes));
    TMR3_clear_array_pw_measurements();

    TMR3_Reload();
    // Start sequence of signals to instruct DHT11 sensor to send readings of temperature and humidity
    RC0_SetDigitalOutput();
    PORTBbits.RB5 = 0;
    __delay_ms(20);
    RC0_SetDigitalInput();
    __delay_us(40);
    TMR3_StartTimer();
    TMR3_StartSinglePulseAcquisition();
    /* Wait maximum time possible for measuring temperature and humidity with dht11. 
     * Full data is 40 bits, 24 bits possibly HIGH and 8 (fractional  part of humidity) 
     * always low. 24 * 62.5nsec * 1140 + 8 * 62.nsec * 400. 400 and 1140 is the maximum 
     * count gained using TMR1 in GATE SINGLE PULSE MODE with clock source 16Mhz and no 
     * prescaler. This mode is used to bypass software overhead when measuring pulsewidths. 
     */
    __delay_ms(3);
    TMR3_StopTimer();
    dht_measurements = TMR3_retrieve_pw_measurements();
    for(b = 0; b < s; b++){
        for(i = 0; i < 8; i++){
            measurement = dht_measurements[tmp++];
            if(((measurement*62.5)/1000) > 50)
                data_in_bytes[b] |= (1 << (7-i));
        }
    }
    if(data_in_bytes[0]+data_in_bytes[1]+data_in_bytes[2]+data_in_bytes[3] == data_in_bytes[4]){ 
        //DHT-11 40-bit data is presented in Qm.n fixed point format.
        hum_tmp = (hum_tmp | (data_in_bytes[0]) << 8) | data_in_bytes[1];
        temp_tmp = (temp_tmp | (data_in_bytes[2] << 8)) | data_in_bytes[3];
        *dht11_humidity = (float) hum_tmp/256;
        *dht11_temperature = (float) temp_tmp/256;
        rslt = 1;
    }else{
    //Data integrity check failed.
    // TODO: Implement function that logs status code in EEPROM and puts system to sleep
        
    }
    return rslt;
}

static void acquire_info_for_server_connection_via_tcp_server(cni *wlan_ssid, cni *wlan_passw,
        cni *server_ip, cni *server_port, uint16_t max_rollovers){

    // If there are no AP and server info saved on EEPROM, put ESP01 in state to get info from ANDROID/IPHONE app (tcp client)
    uint8_t is_valid_ap_info = 0;
    uint8_t is_valid_server_info = 0;
    while((wlan_conn_status == DISCONNECTED || server_conn_status == DISCONNECTED) && count_tmr0_rollovers <= max_rollovers){
        boot_esp01_tcpserver();              
        do{
            if(!is_valid_ap_info || wlan_conn_status == DISCONNECTED){
                if(acquire_WLAN_info(wlan_ssid, wlan_passw) == 1){             // Retrieves AP info from first two strings sent through app
                    is_valid_ap_info = 1;                                                                             
                }else{
                //TODO: Implement function to return message to tcp client with type of error in AP info
                }
            }
            if(!is_valid_server_info || server_conn_status == DISCONNECTED){
                if(acquire_server_info(server_ip, server_port) == 1){    // Retrieves server info from last two strings sent through app
                    is_valid_server_info = 1;                                                     
                }else{
                    //TODO: Implement function to return message to tcp client with type of error in server info
                }
            }
        }while((!is_valid_ap_info || !is_valid_server_info) && count_tmr0_rollovers <= max_rollovers);
        execute_atcmd("AT+CIPSERVER=0");
        save_atcmd_timeout("+CWJAP_CUR", AT_CMD_TYPE_SET, 20000);
        connect_to_WLAN(wlan_ssid->value, wlan_passw->value);   
        save_atcmd_timeout("+CWJAP_CUR", AT_CMD_TYPE_SET, 5000);
        if(wlan_conn_status == CONNECTED){
            write_WLAN_data_to_EEPROM(wlan_ssid, wlan_passw);
        }else{
            // TODO: Implement function to send back message to tcp client that AP info wasn't accepted
            is_valid_ap_info = 0;
            memset(wlan_ssid->value, 0, sizeof(wlan_ssid->value));
            memset(wlan_passw->value, 0, sizeof(wlan_passw->value));
        }
        connect_to_server(server_ip->value, server_port->value);
        if(server_conn_status == CONNECTED){
            write_server_data_to_EEPROM(server_ip, server_port);
        }else{
            // TODO: Implement function to send back message to tcp client that server info wasn't accepted
            is_valid_server_info = 0;
            memset(server_ip->value, 0, sizeof(server_ip->value));
            memset(server_port->value, 0, sizeof(server_port->value));                
        }   
    }    
}

static uint8_t acquire_WLAN_info(cni *ssid, cni *passw){
    if(get_ipd_data(ssid->value, ssid->maxsize) == 0)
        return 0;
    if(get_ipd_data(passw->value, passw->maxsize) == 0)
        return 0;
    return 1;
}

static uint8_t acquire_server_info(cni *ip, cni *port){
    // TODO: Implement more test to check if IPv4 notation is correct
    if(get_ipd_data(ip->value, ip->maxsize) == 0)
        return 0;
    if(get_ipd_data(port->value, port->maxsize) == 0)
        return 0;
    return 1;
}

static void connect_to_WLAN(const char* ssid, const char* passw){
    char at_cmd_tmp_buf[45];
    memset(at_cmd_tmp_buf, 0, sizeof(at_cmd_tmp_buf));
    
    if(wlan_conn_status == CONNECTED){
        execute_atcmd("AT+CWQAP");
        __delay_ms(3000);
    }
    execute_atcmd("AT+CWMODE_DEF=1");
    execute_atcmd("AT+CWDHCP_CUR=1,1");
    sprintf(at_cmd_tmp_buf, "AT+CWJAP_CUR=\"%s\",\"%s\"", ssid, passw);
    execute_atcmd(at_cmd_tmp_buf);
}

static void connect_to_server(const char* ip, const char* port){
    char at_cmd_tmp_buf[45];
    memset(at_cmd_tmp_buf, 0, sizeof(at_cmd_tmp_buf));
    execute_atcmd("AT+CIPMUX=0");       // Allow multiple connections when ESP8266 is in server mode
    sprintf(at_cmd_tmp_buf, "AT+CIPSTART=\"TCP\",\"%s\",%s", ip, port);
    execute_atcmd(at_cmd_tmp_buf);
}

static void boot_esp01_tcpserver(void){
    if(wlan_conn_status == CONNECTED){
        execute_atcmd("AT+CWQAP");
        __delay_ms(3000);
    }
    execute_atcmd("AT+CWMODE_CUR=3");   // Enable SoftAP (access point) mode
    execute_atcmd("AT+CIPAP_CUR=\"192.168.5.1\",\"192.168.5.1\",\"255.255.255.0\"");    // Sets IP address allocated to ESP8266 in SoftAP mode
    execute_atcmd("AT+CWDHCP_CUR=0,1"); // Enable DHCP server of ESP822 in SoftAP mode
    execute_atcmd("AT+CWDHCPS_CUR=1,15,\"192.168.5.2\",\"192.168.5.7\"");  // Sets IP address range that can be allocated by ESP8266 in SoftAP mode
    execute_atcmd("AT+CIPDINFO=0");     // Do not show remote IP and port with +IPD
    execute_atcmd("AT+CIPMODE=0");
    execute_atcmd("AT+CIPMUX=1");       // Allow multiple connections when ESP8266 is in server mode
    execute_atcmd("AT+CIPSERVERMAXCONN=1");     // Max 1 TCP socket when ESP8266 is in server mode
    execute_atcmd("AT+CIPSERVER=1,80");         // Let ESP8266 TCP server listen on port 80 with default ESP8266 IP address (192.168.5.1)


}

static void write_WLAN_data_to_EEPROM(cni *ssid, cni *passw){
    uint8_t i;
    
    ssid->len = strlen(ssid->value);
    passw->len = strlen(passw->value);
    DATAEE_WriteByte(ssid->start_pos, ssid->len);
    for(i = 0; i < ssid->len; i++){
        DATAEE_WriteByte((i + 1 + ssid->start_pos), ssid->value[i]);
    }
    DATAEE_WriteByte(passw->start_pos, passw->len);
    for(i = 0; i < passw->len; i++){
        DATAEE_WriteByte((i + 1 + passw->start_pos), passw->value[i]);
    }    
}

static void write_server_data_to_EEPROM(cni *ip, cni *port){
    uint8_t i;
    
    ip->len = strlen(ip->value);
    port->len = strlen(port->value);
    
    DATAEE_WriteByte(ip->start_pos, ip->len);
    for(i = 0; i < ip->len; i++){
        DATAEE_WriteByte((i + 1 + ip->start_pos), ip->value[i]);
    }
    DATAEE_WriteByte(port->start_pos, port->len);
    for(i = 0; i < port->len; i++){
        DATAEE_WriteByte((i + 1 + port->start_pos), port->value[i]);
    } 
}

static void acquire_neccessary_info_for_server_connection_via_smartconfig(void){
    TMR0_SetInterruptHandler(increment_tmr0_rollover_count);
    execute_atcmd("AT+CWDHCP_CUR=1,1");
    execute_atcmd("AT+CWSTARTSMART=1");    
    __delay_ms(10000);
    execute_atcmd("AT+CWSTOPSMART");
}

static void increment_tmr0_rollover_count(){
    count_tmr0_rollovers--;
}
/*End of File
*/