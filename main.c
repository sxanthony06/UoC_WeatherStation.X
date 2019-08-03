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
#include "at_parser.h"
#include <string.h>
#include <stdio.h>
#include "utilities.h"
#include <stdbool.h>

static T_AT_storage handlers_store[5];
static enum at_cmd_results latest_atcmd_result;
static enum conn_status wlan_conn_status, server_conn_status;

struct common_net_info{
    uint8_t start_pos;
    uint8_t maxsize;
    uint8_t len;
    char value[EEPROM_STRING_MAX_LENGTH];
}static wlan_ssid, wlan_passw, server_ip, server_port;

static float current_temperature, current_humidity;

static void default_ATCMD_response_parser(const char*, unsigned char );
static void check_WLAN_connection_callback(const char*, unsigned char );
static void check_server_conn_callback(const char*, unsigned char);
static uint8_t fetch_WLAN_data_eeprom(struct common_net_info*, struct common_net_info*);
static uint8_t fetch_server_data_eeprom(struct common_net_info*, struct common_net_info*);
static void clear_buffer(uint8_t, size_t,...);
static void dht11_start_measuring(void);
static void dht11_process_measurements(void);
static uint8_t acquire_WLAN_info(struct common_net_info*, struct common_net_info*);
static uint8_t acquire_server_info(struct common_net_info*, struct common_net_info*);
static void acquire_neccessary_info_for_server_connection_via_tcp_server(void);
static uint8_t resolve_recv_net_info(struct common_net_info*, struct common_net_info*,
        struct common_net_info*, struct common_net_info*);
static void boot_esp01_tcpserver(void);
static void connect_to_WLAN(const char*, const char*);
static void connect_to_server(const char*, const char*);
static void write_WLAN_data_to_EEPROM(struct common_net_info*, struct common_net_info*);
static void write_server_data_to_EEPROM(struct common_net_info*, struct common_net_info*);


/*
                         Main application
 */
void main(void)
{
    wlan_ssid = (struct common_net_info){0, EEPROM_STRING_MAX_LENGTH, 0};
    wlan_passw = (struct common_net_info){wlan_ssid.start_pos + wlan_ssid.maxsize + 1, EEPROM_STRING_MAX_LENGTH, 0};
    server_ip = (struct common_net_info){wlan_passw.start_pos + wlan_passw.maxsize + 1, EEPROM_STRING_MAX_LENGTH, 0};
    server_port = (struct common_net_info){server_ip.start_pos + server_ip.maxsize + 1, EEPROM_SERVER_PORT_MAX_LENGTH, 0};

    memset(wlan_ssid.value, 0, wlan_ssid.maxsize);
    memset(wlan_passw.value, 0, wlan_passw.maxsize);
    memset(server_ip.value, 0, server_ip.maxsize);
    memset(server_port.value, 0, server_port.maxsize);

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


    ENGINE_initiate(EUSART1_Write, EUSART1_Read, EUSART1_is_rx_ready, SYSTEM_custom_delay_in_ms);
    init_atcmd_parser(default_ATCMD_response_parser, 100, handlers_store);
    save_atcmd_handler("+CWJAP_CUR", AT_CMD_TYPE_SET, 5000, check_WLAN_connection_callback);
    save_atcmd_handler("+CIPSTART", AT_CMD_TYPE_SET, 2000, check_server_conn_callback); 
    
    execute_atcmd("ATE0");
    execute_atcmd("AT+CWMODE_DEF=1");           // On power-up ESP01 sets into last configured Station/AP mode (even if not written to FLASH). This AT command defaults it to station mode, consuming less power.
    execute_atcmd("AT+CIPSNTPCFG=1,-4,\"0.pool.ntp.org\",\"1.pool.ntp.org\",\"3.pool.ntp.org\"");   // Configure device's location timezone and network time servers (to get time).
    execute_atcmd("AT+CWJAP_CUR?");

    if(wlan_conn_status == DISCONNECTED){
        if(fetch_WLAN_data_eeprom(&wlan_ssid, &wlan_passw) & fetch_server_data_eeprom(&server_ip, &server_port)){
            connect_to_WLAN(wlan_ssid.value, wlan_passw.value);

            execute_atcmd("AT+CWAUTOCONN=1");   // Configure ESP8266 to use uploaded info to connect automatically to AP on power-up
            connect_to_server(server_ip.value, server_port.value);
        }else{
            acquire_neccessary_info_for_server_connection_via_tcp_server();
        }
    }
    if(server_conn_status == DISCONNECTED){
        if(fetch_server_data_eeprom(&server_ip, &server_port)){
            connect_to_server(server_ip.value, server_port.value);
        }
    }

    __delay_ms(5000);
    dht11_process_measurements();

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

static uint8_t fetch_WLAN_data_eeprom(struct common_net_info* ap_ssid, struct common_net_info* ap_passw){
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

static uint8_t fetch_server_data_eeprom(struct common_net_info* server_ip, struct common_net_info* server_port){
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

static void dht11_process_measurements(){
    uint8_t bytes[5];
    uint8_t measurement_attempts = 2;
    uint8_t correct_measurement = 0;
    uint8_t tmp;
    uint16_t hum_tmp = 0;
    uint16_t temp_tmp = 0;
    uint16_t tmpdht11;
    uint8_t s = sizeof(bytes);
    memset(bytes, 0, sizeof(bytes));

    while(!correct_measurement && measurement_attempts--){
        tmp = 0;
        memset(bytes, 0, sizeof(bytes));

        dht11_start_measuring();
        __delay_ms(2000);
        volatile const uint16_t* dht_measurements = TMR1_list_pulsewidth_measurements();
        for(int b = 0; b < s; b++){
            for(int i = 0; i < 8; i++){
                tmpdht11 = dht_measurements[tmp++];
                if(((tmpdht11*62.5)/1000) > 50)
                    bytes[b] |= (1 << (7-i));
            }
        }
        if(bytes[0]+bytes[1]+bytes[2]+bytes[3] == bytes[4]){
            correct_measurement = 1;
        }
    }
    if(!correct_measurement){
        //Data integrity check failed. Log error status code in EEPROM.
    }


    //DHT-11 40-bit data is presented in Qm.n fixed point format.
    hum_tmp = (hum_tmp | (bytes[0]) << 8) | bytes[1];
    temp_tmp = (temp_tmp | (bytes[2] << 8)) | bytes[3];
    current_humidity = (float) hum_tmp/256;
    current_temperature = (float) temp_tmp/256;
}

static void dht11_start_measuring(void){
    T1G_SetDigitalOutput();
    PORTBbits.RB5 = 0;
    __delay_ms(20);
    T1G_SetDigitalInput();
    __delay_us(40);
    TMR1_StartTimer();
    TMR1_StartSinglePulseAcquisition();
}

static void acquire_neccessary_info_for_server_connection_via_tcp_server(void){
// If there are no AP and server info saved on EEPROM, put ESP01 in state to get info from ANDROID/IPHONE app (tcp client)
    uint8_t is_valid_ap_info = 0;
    uint8_t is_valid_server_info = 0;
    volatile uint8_t has_timer_run_out = 0;
    while((wlan_conn_status == DISCONNECTED || server_conn_status == DISCONNECTED) && !has_timer_run_out){
        boot_esp01_tcpserver();              
        do{
            if(!is_valid_ap_info || wlan_conn_status == DISCONNECTED){
                if(acquire_WLAN_info(&wlan_ssid, &wlan_passw) == 1){             // Retrieves AP info from first two strings sent through app
                    is_valid_ap_info = 1;                                                                             
                }else{
                //TODO: Implement function to return message to tcp client with type of error in AP info
                }
            }
            if(!is_valid_server_info || server_conn_status == DISCONNECTED){
                if(acquire_server_info(&server_ip, &server_port) == 1){    // Retrieves server info from last two strings sent through app
                    is_valid_server_info = 1;                                                     
                }else{
                    //TODO: Implement function to return message to tcp client with type of error in server info
                }
            }
        }while((!is_valid_ap_info || !is_valid_server_info) && !has_timer_run_out);
        execute_atcmd("AT+CIPSERVER=0");
        save_atcmd_timeout("+CWJAP_CUR", AT_CMD_TYPE_SET, 20000);
        connect_to_WLAN(wlan_ssid.value, wlan_passw.value);   
        save_atcmd_timeout("+CWJAP_CUR", AT_CMD_TYPE_SET, 5000);
        if(wlan_conn_status == CONNECTED){
            write_WLAN_data_to_EEPROM(&wlan_ssid, &wlan_passw);
        }else{
            // TODO: Implement function to send back message to tcp client that AP info wasn't accepted
            is_valid_ap_info = 0;
            memset(wlan_ssid.value, 0, sizeof(wlan_ssid.value));
            memset(wlan_passw.value, 0, sizeof(wlan_passw.value));
        }
        connect_to_server(server_ip.value, server_port.value);
        if(server_conn_status == CONNECTED){
            write_server_data_to_EEPROM(&server_ip, &server_port);
        }else{
            // TODO: Implement function to send back message to tcp client that server info wasn't accepted
            is_valid_server_info = 0;
            memset(server_ip.value, 0, sizeof(server_ip.value));
            memset(server_port.value, 0, sizeof(server_port.value));                
        }   
    }    
}

static uint8_t acquire_WLAN_info(struct common_net_info* ssid, struct common_net_info* passw){
    if(get_ipd_data(ssid->value, ssid->maxsize) == 0)
        return 0;
    if(get_ipd_data(passw->value, passw->maxsize) == 0)
        return 0;
    return 1;
}

static uint8_t acquire_server_info(struct common_net_info* ip, struct common_net_info* port){
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

static void write_WLAN_data_to_EEPROM(struct common_net_info* ssid, struct common_net_info* passw){
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

static void write_server_data_to_EEPROM(struct common_net_info* ip, struct common_net_info* port){
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
/*End of File
*/