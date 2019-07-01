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

#define testbit(var, bit) ((var) & (1 <<(bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

static T_AT_storage handlers_store[10];

void esp8266_default_atcmd_handler(const char* atcmd_resp, unsigned char type);

/*
                         Main application
 */
void main(void)
{    
    // Initialize the device
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
    

    init_engine(EUSART1_Write);
    init_atcmd_parser(esp8266_default_atcmd_handler, 100, handlers_store);
    save_atcmd_handler("+CWMODE", 100, esp8266_default_atcmd_handler);
    execute_atcmd("ATE0");
    execute_atcmd("AT+CIPSTATUS");
//    EUSART1_Read_Buffer(eusart1RxBuf);
//    a1 = makeHash("ATE0\r\r\n\r\nOK\r\n");
//    a2 = makeHash(eusart1RxBuf);
    
//    EUSART1_WRITE_STRING("AT+CWMODE=3");
//    EUSART1_WRITE_STRING("AT+CWJAP=\"Guests Wifi\",\"nj4933k3rm\"");
//    EUSART1_WRITE_STRING("AT+CWJAP=\"Linksys31695\",\"2011IsMyYear\"");
//    EUSART1_WRITE_STRING("AT+CIPSTART=\"TCP\",\"192.168.2.142\",9999");
//    EUSART1_WRITE_STRING("AT+CIPMODE=1");
//    EUSART1_WRITE_STRING("AT+CIPSEND");
//    EUSART1_WRITE_STRING("Esaki ta un test!");


    while (1)
    {     
    }
}

/* Callback function specifically for handling (ESP8266) execute commands that 
 return "OK" on success or "ERROR".*/
void esp8266_default_atcmd_handler(const char* atcmd_resp, unsigned char type){

}

/*End of File
*/