/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F45K22
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00
        MPLAB 	          :  MPLAB X 5.10	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set LINK_LED aliases
#define LINK_LED_TRIS                 TRISBbits.TRISB3
#define LINK_LED_LAT                  LATBbits.LATB3
#define LINK_LED_PORT                 PORTBbits.RB3
#define LINK_LED_WPU                  WPUBbits.WPUB3
#define LINK_LED_ANS                  ANSELBbits.ANSB3
#define LINK_LED_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define LINK_LED_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define LINK_LED_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define LINK_LED_GetValue()           PORTBbits.RB3
#define LINK_LED_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define LINK_LED_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define LINK_LED_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define LINK_LED_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define LINK_LED_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define LINK_LED_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set POWER_LED aliases
#define POWER_LED_TRIS                 TRISBbits.TRISB4
#define POWER_LED_LAT                  LATBbits.LATB4
#define POWER_LED_PORT                 PORTBbits.RB4
#define POWER_LED_WPU                  WPUBbits.WPUB4
#define POWER_LED_ANS                  ANSELBbits.ANSB4
#define POWER_LED_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define POWER_LED_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define POWER_LED_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define POWER_LED_GetValue()           PORTBbits.RB4
#define POWER_LED_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define POWER_LED_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define POWER_LED_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define POWER_LED_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define POWER_LED_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define POWER_LED_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set T1G aliases
#define T1G_TRIS                 TRISBbits.TRISB5
#define T1G_LAT                  LATBbits.LATB5
#define T1G_PORT                 PORTBbits.RB5
#define T1G_WPU                  WPUBbits.WPUB5
#define T1G_ANS                  ANSELBbits.ANSB5
#define T1G_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define T1G_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define T1G_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define T1G_GetValue()           PORTBbits.RB5
#define T1G_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define T1G_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define T1G_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define T1G_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define T1G_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define T1G_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set WIND_DIRECTION_INPUT aliases
#define WIND_DIRECTION_INPUT_TRIS                 TRISCbits.TRISC2
#define WIND_DIRECTION_INPUT_LAT                  LATCbits.LATC2
#define WIND_DIRECTION_INPUT_PORT                 PORTCbits.RC2
#define WIND_DIRECTION_INPUT_ANS                  ANSELCbits.ANSC2
#define WIND_DIRECTION_INPUT_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define WIND_DIRECTION_INPUT_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define WIND_DIRECTION_INPUT_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define WIND_DIRECTION_INPUT_GetValue()           PORTCbits.RC2
#define WIND_DIRECTION_INPUT_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define WIND_DIRECTION_INPUT_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define WIND_DIRECTION_INPUT_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define WIND_DIRECTION_INPUT_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set WIND_SPEED_INPUT aliases
#define WIND_SPEED_INPUT_TRIS                 TRISCbits.TRISC3
#define WIND_SPEED_INPUT_LAT                  LATCbits.LATC3
#define WIND_SPEED_INPUT_PORT                 PORTCbits.RC3
#define WIND_SPEED_INPUT_ANS                  ANSELCbits.ANSC3
#define WIND_SPEED_INPUT_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define WIND_SPEED_INPUT_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define WIND_SPEED_INPUT_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define WIND_SPEED_INPUT_GetValue()           PORTCbits.RC3
#define WIND_SPEED_INPUT_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define WIND_SPEED_INPUT_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define WIND_SPEED_INPUT_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define WIND_SPEED_INPUT_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set ESP01_CHPD_PIN aliases
#define ESP01_CHPD_PIN_TRIS                 TRISCbits.TRISC5
#define ESP01_CHPD_PIN_LAT                  LATCbits.LATC5
#define ESP01_CHPD_PIN_PORT                 PORTCbits.RC5
#define ESP01_CHPD_PIN_ANS                  ANSELCbits.ANSC5
#define ESP01_CHPD_PIN_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define ESP01_CHPD_PIN_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define ESP01_CHPD_PIN_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define ESP01_CHPD_PIN_GetValue()           PORTCbits.RC5
#define ESP01_CHPD_PIN_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define ESP01_CHPD_PIN_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define ESP01_CHPD_PIN_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define ESP01_CHPD_PIN_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

// get/set PRECIPITATION_INPUT aliases
#define PRECIPITATION_INPUT_TRIS                 TRISDbits.TRISD2
#define PRECIPITATION_INPUT_LAT                  LATDbits.LATD2
#define PRECIPITATION_INPUT_PORT                 PORTDbits.RD2
#define PRECIPITATION_INPUT_ANS                  ANSELDbits.ANSD2
#define PRECIPITATION_INPUT_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define PRECIPITATION_INPUT_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define PRECIPITATION_INPUT_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define PRECIPITATION_INPUT_GetValue()           PORTDbits.RD2
#define PRECIPITATION_INPUT_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define PRECIPITATION_INPUT_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define PRECIPITATION_INPUT_SetAnalogMode()      do { ANSELDbits.ANSD2 = 1; } while(0)
#define PRECIPITATION_INPUT_SetDigitalMode()     do { ANSELDbits.ANSD2 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/