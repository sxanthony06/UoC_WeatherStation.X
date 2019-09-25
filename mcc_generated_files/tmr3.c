/**
  TMR3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr3.c

  @Summary
    This is the generated driver implementation file for the TMR3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR3.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F45K22
        Driver Version    :  2.01
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

/**
  Section: Included Files
*/


#include <xc.h>
#include <string.h>
#include "tmr3.h"
#include "utilities.h"

/**
  Section: Global Variable Definitions
*/
static volatile uint16_t timer3ReloadVal;
static volatile uint16_t pulse_width_measurements[PULSE_WIDTH_MEASUREMENTS_SIZE];
static volatile uint8_t measurement_number = 0;

/**
  Section: TMR3 APIs
*/

void TMR3_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T3GSS T3G_pin; TMR3GE enabled; T3GTM disabled; T3GPOL high; T3GGO done; T3GSPM enabled; 
    T3GCON = 0xD0;

    //TMR3H 0; 
    TMR3H = 0x00;

    //TMR3L 0; 
    TMR3L = 0x00;

    // Load the TMR value to reload variable
    timer3ReloadVal=TMR3;

    // Clearing IF flag.
    PIR2bits.TMR3IF = 0;

    // Clearing IF flag before enabling the interrupt.
    PIR3bits.TMR3GIF = 0;

    // Enabling TMR3 interrupt.
    PIE3bits.TMR3GIE = 1;

    // T3CKPS 1:1; T3OSCEN disabled; nT3SYNC do_not_synchronize; TMR3CS FOSC; TMR3ON disabled; T3RD16 enabled; 
    T3CON = 0x46;
}

void TMR3_StartTimer(void)
{
    // Start the Timer by writing to TMRxON bit
    T3CONbits.TMR3ON = 1;
}

void TMR3_StopTimer(void)
{
    // Stop the Timer by writing to TMRxON bit
    T3CONbits.TMR3ON = 0;
}

uint16_t TMR3_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    
    T3CONbits.T3RD16 = 1;
	
    readValLow = TMR3L;
    readValHigh = TMR3H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR3_WriteTimer(uint16_t timerVal)
{
    if (T3CONbits.nT3SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T3CONbits.TMR3ON = 0;

        // Write to the Timer3 register
        TMR3H = (timerVal >> 8);
        TMR3L = (uint8_t) timerVal;

        // Start the Timer after writing to the register
        T3CONbits.TMR3ON =1;
    }
    else
    {
        // Write to the Timer3 register
        TMR3H = (timerVal >> 8);
        TMR3L = (uint8_t) timerVal;
    }
}

void TMR3_Reload(void)
{
    TMR3_WriteTimer(timer3ReloadVal);
}

void TMR3_StartSinglePulseAcquisition(void)
{
    T3GCONbits.T3GGO = 1;
}

uint8_t TMR3_CheckGateValueStatus(void)
{
    return T3GCONbits.T3GVAL;
}

bool TMR3_HasOverflowOccured(void)
{
    // check if  overflow has occurred by checking the TMRIF bit
    return(PIR2bits.TMR3IF);
}

volatile const uint16_t* TMR3_retrieve_pw_measurements(void){
    return pulse_width_measurements;
}

void TMR3_GATE_ISR(void)
{
    T3CONbits.TMR3ON = 0;
    // Skip first measurement as this is a pulse produced by DHT-11 that doesn't encode humidity measurement.
    if(measurement_number > 0 && (measurement_number < PULSE_WIDTH_MEASUREMENTS_SIZE))
        pulse_width_measurements[measurement_number-1] = TMR3_ReadTimer();
    TMR3H = 0x00;
    TMR3L = 0x00;
    T3CONbits.TMR3ON = 1;
    T3GCONbits.T3GGO = 1;
    measurement_number++;
            
    // Clear the TMR1 interrupt flag
    PIR3bits.TMR3GIF = 0;
}

void TMR3_clear_array_pw_measurements(void){
    measurement_number = 0;
    memset((void*)pulse_width_measurements, 0, sizeof(pulse_width_measurements));
}
/**
 End of File
*/
