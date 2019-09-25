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

// get/set WIND_DIR_315 aliases
#define WIND_DIR_315_TRIS                 TRISAbits.TRISA0
#define WIND_DIR_315_LAT                  LATAbits.LATA0
#define WIND_DIR_315_PORT                 PORTAbits.RA0
#define WIND_DIR_315_ANS                  ANSELAbits.ANSA0
#define WIND_DIR_315_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define WIND_DIR_315_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define WIND_DIR_315_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define WIND_DIR_315_GetValue()           PORTAbits.RA0
#define WIND_DIR_315_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define WIND_DIR_315_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define WIND_DIR_315_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define WIND_DIR_315_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set WIND_DIR_270 aliases
#define WIND_DIR_270_TRIS                 TRISAbits.TRISA1
#define WIND_DIR_270_LAT                  LATAbits.LATA1
#define WIND_DIR_270_PORT                 PORTAbits.RA1
#define WIND_DIR_270_ANS                  ANSELAbits.ANSA1
#define WIND_DIR_270_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define WIND_DIR_270_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define WIND_DIR_270_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define WIND_DIR_270_GetValue()           PORTAbits.RA1
#define WIND_DIR_270_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define WIND_DIR_270_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define WIND_DIR_270_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define WIND_DIR_270_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set WIND_DIR_225 aliases
#define WIND_DIR_225_TRIS                 TRISAbits.TRISA2
#define WIND_DIR_225_LAT                  LATAbits.LATA2
#define WIND_DIR_225_PORT                 PORTAbits.RA2
#define WIND_DIR_225_ANS                  ANSELAbits.ANSA2
#define WIND_DIR_225_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define WIND_DIR_225_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define WIND_DIR_225_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define WIND_DIR_225_GetValue()           PORTAbits.RA2
#define WIND_DIR_225_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define WIND_DIR_225_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define WIND_DIR_225_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define WIND_DIR_225_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set WIND_DIR_180 aliases
#define WIND_DIR_180_TRIS                 TRISAbits.TRISA3
#define WIND_DIR_180_LAT                  LATAbits.LATA3
#define WIND_DIR_180_PORT                 PORTAbits.RA3
#define WIND_DIR_180_ANS                  ANSELAbits.ANSA3
#define WIND_DIR_180_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define WIND_DIR_180_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define WIND_DIR_180_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define WIND_DIR_180_GetValue()           PORTAbits.RA3
#define WIND_DIR_180_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define WIND_DIR_180_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define WIND_DIR_180_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define WIND_DIR_180_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set WIND_DIR_135 aliases
#define WIND_DIR_135_TRIS                 TRISAbits.TRISA4
#define WIND_DIR_135_LAT                  LATAbits.LATA4
#define WIND_DIR_135_PORT                 PORTAbits.RA4
#define WIND_DIR_135_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define WIND_DIR_135_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define WIND_DIR_135_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define WIND_DIR_135_GetValue()           PORTAbits.RA4
#define WIND_DIR_135_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define WIND_DIR_135_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)

// get/set WIND_DIR_90 aliases
#define WIND_DIR_90_TRIS                 TRISAbits.TRISA5
#define WIND_DIR_90_LAT                  LATAbits.LATA5
#define WIND_DIR_90_PORT                 PORTAbits.RA5
#define WIND_DIR_90_ANS                  ANSELAbits.ANSA5
#define WIND_DIR_90_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define WIND_DIR_90_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define WIND_DIR_90_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define WIND_DIR_90_GetValue()           PORTAbits.RA5
#define WIND_DIR_90_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define WIND_DIR_90_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define WIND_DIR_90_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define WIND_DIR_90_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set WIND_SPEED_SENSOR aliases
#define WIND_SPEED_SENSOR_TRIS                 TRISBbits.TRISB4
#define WIND_SPEED_SENSOR_LAT                  LATBbits.LATB4
#define WIND_SPEED_SENSOR_PORT                 PORTBbits.RB4
#define WIND_SPEED_SENSOR_WPU                  WPUBbits.WPUB4
#define WIND_SPEED_SENSOR_ANS                  ANSELBbits.ANSB4
#define WIND_SPEED_SENSOR_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define WIND_SPEED_SENSOR_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define WIND_SPEED_SENSOR_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define WIND_SPEED_SENSOR_GetValue()           PORTBbits.RB4
#define WIND_SPEED_SENSOR_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define WIND_SPEED_SENSOR_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define WIND_SPEED_SENSOR_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define WIND_SPEED_SENSOR_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define WIND_SPEED_SENSOR_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define WIND_SPEED_SENSOR_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RAIN_GAUGE_SENSOR aliases
#define RAIN_GAUGE_SENSOR_TRIS                 TRISBbits.TRISB5
#define RAIN_GAUGE_SENSOR_LAT                  LATBbits.LATB5
#define RAIN_GAUGE_SENSOR_PORT                 PORTBbits.RB5
#define RAIN_GAUGE_SENSOR_WPU                  WPUBbits.WPUB5
#define RAIN_GAUGE_SENSOR_ANS                  ANSELBbits.ANSB5
#define RAIN_GAUGE_SENSOR_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RAIN_GAUGE_SENSOR_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RAIN_GAUGE_SENSOR_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RAIN_GAUGE_SENSOR_GetValue()           PORTBbits.RB5
#define RAIN_GAUGE_SENSOR_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RAIN_GAUGE_SENSOR_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RAIN_GAUGE_SENSOR_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define RAIN_GAUGE_SENSOR_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define RAIN_GAUGE_SENSOR_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define RAIN_GAUGE_SENSOR_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)

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

// get/set WIND_DIR_45 aliases
#define WIND_DIR_45_TRIS                 TRISEbits.TRISE0
#define WIND_DIR_45_LAT                  LATEbits.LATE0
#define WIND_DIR_45_PORT                 PORTEbits.RE0
#define WIND_DIR_45_ANS                  ANSELEbits.ANSE0
#define WIND_DIR_45_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define WIND_DIR_45_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define WIND_DIR_45_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define WIND_DIR_45_GetValue()           PORTEbits.RE0
#define WIND_DIR_45_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define WIND_DIR_45_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define WIND_DIR_45_SetAnalogMode()      do { ANSELEbits.ANSE0 = 1; } while(0)
#define WIND_DIR_45_SetDigitalMode()     do { ANSELEbits.ANSE0 = 0; } while(0)

// get/set WIND_DIR_0 aliases
#define WIND_DIR_0_TRIS                 TRISEbits.TRISE1
#define WIND_DIR_0_LAT                  LATEbits.LATE1
#define WIND_DIR_0_PORT                 PORTEbits.RE1
#define WIND_DIR_0_ANS                  ANSELEbits.ANSE1
#define WIND_DIR_0_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define WIND_DIR_0_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define WIND_DIR_0_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define WIND_DIR_0_GetValue()           PORTEbits.RE1
#define WIND_DIR_0_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define WIND_DIR_0_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define WIND_DIR_0_SetAnalogMode()      do { ANSELEbits.ANSE1 = 1; } while(0)
#define WIND_DIR_0_SetDigitalMode()     do { ANSELEbits.ANSE1 = 0; } while(0)

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


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCB4 pin functionality
 * @Example
    IOCB4_ISR();
 */
void IOCB4_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCB4 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCB4 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCB4_SetInterruptHandler(MyInterruptHandler);

*/
void IOCB4_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCB4 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCB4_SetInterruptHandler() method.
    This handler is called every time the IOCB4 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB4_SetInterruptHandler(IOCB4_InterruptHandler);

*/
extern void (*IOCB4_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCB4 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCB4_SetInterruptHandler() method.
    This handler is called every time the IOCB4 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB4_SetInterruptHandler(IOCB4_DefaultInterruptHandler);

*/
void IOCB4_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCB5 pin functionality
 * @Example
    IOCB5_ISR();
 */
void IOCB5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCB5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCB5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCB5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCB5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCB5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCB5_SetInterruptHandler() method.
    This handler is called every time the IOCB5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB5_SetInterruptHandler(IOCB5_InterruptHandler);

*/
extern void (*IOCB5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCB5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCB5_SetInterruptHandler() method.
    This handler is called every time the IOCB5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB5_SetInterruptHandler(IOCB5_DefaultInterruptHandler);

*/
void IOCB5_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/