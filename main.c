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
#include <stdio.h>
#include "utilities.h"

__EEPROM_DATA(0x0C, 0x4C, 0x69, 0x6E, 0x6B, 0x73, 0x79,	0x73);
__EEPROM_DATA(0x33,	0x31, 0x36, 0x39, 0x35, 0xFF, 0xFF, 0xFF);

__EEPROM_DATA(0x0C, 0x32, 0x30, 0x31, 0x31, 0x49, 0x73, 0x4D);
__EEPROM_DATA(0x79, 0x59, 0x65, 0x61, 0x72, 0xFF, 0xFF, 0xFF);

__EEPROM_DATA(0xC0, 0xA8, 0x02, 0x8E, 0x04, 0x39, 0x39, 0x39);
__EEPROM_DATA(0x39, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

__EEPROM_DATA(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
__EEPROM_DATA(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

#define testbit(var, bit) ((var) & (1 <<(bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))
#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

#define SUCCESS 1
#define FAILURE 2
#define ERROR   3

static T_AT_storage handlers_store[10];
static uint8_t last_cmd_result, connected_to_ap, connected_to_server;
static uint8_t parsed_resp[45];

struct common_net_info{
    uint8_t start_pos;
    uint8_t maxsize;
    uint8_t len;
    char value[16];
}static ap_ssid, ap_passw, server_ip, server_port;

static float current_temperature, current_humidity;



void esp8266_default_atcmd_handler(const char*, unsigned char );
void esp8266_ap_conn_handler(const char*, unsigned char );
uint8_t fetch_AP_data_eeprom(struct common_net_info*, struct common_net_info*);
uint8_t fetch_server_data_eeprom(struct common_net_info*, struct common_net_info*);
void clear_buffer(uint8_t, size_t,...);
static void dht11_start_measuring(void);
void dht11_process_measurements(void);
/*
                         Main application
 */
void main(void)
{
    ap_ssid = (struct common_net_info){0, EEPROM_MAX_STRING_LENGTH, 0};
    ap_passw = (struct common_net_info){ap_ssid.start_pos + ap_ssid.maxsize + 1, EEPROM_MAX_STRING_LENGTH, 0};
    server_ip = (struct common_net_info){ap_passw.start_pos + ap_passw.maxsize + 1, EEPROM_SERVER_IP_LENGTH, 4};
    server_port = (struct common_net_info){server_ip.start_pos + server_ip.maxsize, EEPROM_SERVER_PORT_LENGTH, 0};

    memset(ap_ssid.value, 0, ap_ssid.maxsize);
    memset(ap_passw.value, 0, ap_passw.maxsize); 
    memset(server_ip.value, 0, server_ip.maxsize);
    memset(server_port.value, 0, server_port.maxsize);
    
    // Configure and initialize the device. POWER_LED is turned on during PIN_MANAGER_Initialize().)
    SYSTEM_Initialize();
    __delay_ms(1000);
    
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    

    init_engine(EUSART1_Write, EUSART1_Read);
    init_atcmd_parser(esp8266_default_atcmd_handler, 100, handlers_store);
    save_atcmd_handler("+CWJAP", 100, esp8266_ap_conn_handler);
    
    //execute_atcmd("AT+RESTORE");
    execute_atcmd("ATE0");
    execute_atcmd("AT+CWMODE=3");
    execute_atcmd("AT+CIPSNTPCFG=1,-4,\"0.pool.ntp.org\",\"1.pool.ntp.org\",\"3.pool.ntp.org\"");
    execute_atcmd("AT+CWJAP?");
    if(!connected_to_ap){
        if(fetch_AP_data_eeprom(&ap_ssid, &ap_passw) & fetch_server_data_eeprom(&server_ip, &server_port)){
            char x[45] = "";
            execute_atcmd("AT+CWMODE_DEF=1");
            execute_atcmd("AT+CWDHCP_DEF=0,1");
            
            sprintf(x, "AT+CWJAP_CUR=\"%s\",\"%s\"", ap_ssid.value, ap_passw.value);
            execute_atcmd(x);
            
            memset(x, 0, sizeof(x));
            __delay_ms(3000);
            connected_to_ap = last_cmd_result;
            
            execute_atcmd("AT+CWAUTOCONN=1");
            execute_atcmd("AT+CIPDOMAIN=\"8.8.8.8\"");
            
            sprintf(x, "AT+CIPSTART=\"TCP\",\"%s\",%s", server_ip.value, server_port.value);
            execute_atcmd(x);
            __delay_ms(3000);
            connected_to_server = last_cmd_result;
        }else{
            execute_atcmd("AT+CWMODE_CUR=2");
            execute_atcmd("AT+CWDHCP_DEF=1,0");
            //TODO: Configure timer and build Android app to load custom network info. See program flowchart v2.0
        }
    }
    if(!connected_to_server){
        if(fetch_server_data_eeprom(&server_ip, &server_port)){
            char tmp[45] = "";
            sprintf(tmp, "AT+CIPSTART=\"TCP\",\"%s\",%s", server_ip.value, server_port.value);
            execute_atcmd(tmp);
            
        }
    }
    __delay_ms(5000);
    dht11_process_measurements();

//    EUSART1_WRITE_STRING("AT+CWJAP=\"Guests Wifi\",\"nj4933k3rm\"");
//    EUSART1_WRITE_STRING("AT+CWJAP=\"Linksys31695\",\"2011IsMyYear\"");
//    EUSART1_WRITE_STRING("AT+CIPSTART=\"TCP\",\"192.168.2.142\",9999");

    while (1)
    {
        __delay_ms(1000);
    }
}

/* Callback function specifically for handling (ESP8266) execute commands that 
 return "OK" on success or "ERROR".*/
void esp8266_default_atcmd_handler(const char* atcmd_resp, unsigned char type){
    char * tmp;

    tmp = strstr(atcmd_resp, "OK");
    last_cmd_result = (tmp != 0)? SUCCESS : ERROR;        
}

void esp8266_ap_conn_handler(const char* atcmd_resp, unsigned char type){   
    if(type == 0){
    }else if(type == 1){
        //TODO: strstr is not caseinsensitive. Implement a caseinsensitive comparison.
        char * x;
        char * y;
        x = strstr(atcmd_resp, "No AP");
        y = strstr(atcmd_resp, "ERROR");
        if(y != 0)
            last_cmd_result = FAILURE;
        else if(x == 0){
            last_cmd_result = SUCCESS;
            connected_to_ap = 1;        
        }else {
            last_cmd_result = SUCCESS;
            connected_to_ap = 0;
            connected_to_server = 0;
        }
    }else if(type == 2){
    }else{
//        esp8266_default_atcmd_handler(atcmd_resp, type);
    }
}

uint8_t fetch_AP_data_eeprom(struct common_net_info* ap_ssid, struct common_net_info* ap_passw){
    uint8_t i;
    
    ap_ssid->len = DATAEE_ReadByte(ap_ssid->start_pos);
    ap_passw->len = DATAEE_ReadByte(ap_passw->start_pos);
    
    if(ap_ssid->len == 0 || ap_passw->len == 0)
        return 0;  
    for(i = 0; i < ap_ssid->len; i++){
        ap_ssid->value[i] = DATAEE_ReadByte(i + 1 + ap_ssid->start_pos);
    }   
    for(i = 0; i < ap_passw->len; i++){
        ap_passw->value[i] = DATAEE_ReadByte(i + ap_passw->start_pos + 1);
    }       
    return 1;
}

uint8_t fetch_server_data_eeprom(struct common_net_info* server_ip, struct common_net_info* server_port){
    uint8_t i, ip_in_bytes[4];
    server_port->len = DATAEE_ReadByte(server_port->start_pos);
    
    if(server_ip->len == 0 || server_port->len == 0)
        return 0;    
    for(i = 0; i < server_ip->len; i++){
        ip_in_bytes[i] = DATAEE_ReadByte(i + server_ip->start_pos);
    }
    sprintf(server_ip->value, "%d.%d.%d.%d", ip_in_bytes[0], ip_in_bytes[1], ip_in_bytes[2], ip_in_bytes[3]);
    for(i = 0; i < server_port->len; i++){
        server_port->value[i] = DATAEE_ReadByte(i + server_port->start_pos + 1);
    }        
    return 1;
}

void clear_buffer(uint8_t filler, size_t size,...){
    
}

void dht11_process_measurements(){
    uint8_t bytes[5];
    uint8_t measurement_attempts = 2;
    uint8_t correct_measurement = 0;
    uint8_t tmp;
    uint16_t hum_tmp = 0;
    uint16_t temp_tmp = 0;
    uint8_t s = sizeof(bytes);
    memset(bytes, 0, sizeof(bytes));
    
    while(!correct_measurement && measurement_attempts--){
        dht11_start_measuring();
        __delay_ms(2000);
        volatile const uint16_t* dht_measurements = TMR1_list_pulsewidth_measurements();
        uint16_t tmpdht11;
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
/*End of File
*/