/**
  TMR1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated driver implementation file for the TMR1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR1.
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
#include "tmr1.h"
#include "utilities.h"

/**
  Section: Global Variable Definitions
*/
volatile uint16_t timer1ReloadVal;
volatile uint8_t amount_measurements;
static volatile uint16_t pulse_width_measurements[PULSE_WIDTH_MEASUREMENTS_SIZE];
static volatile uint8_t measurement_number = 0;
/**
  Section: TMR1 APIs
*/

void TMR1_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T1GSS T1G_pin; TMR1GE enabled; T1GTM disabled; T1GPOL high; T1GGO done; T1GSPM enabled; 
    T1GCON = 0xD0;

    //TMR1H 0; 
    TMR1H = 0x00;

    //TMR1L 0; 
    TMR1L = 0x00;

    // Load the TMR value to reload variable
    timer1ReloadVal=TMR1;

    // Clearing IF flag.
    PIR1bits.TMR1IF = 0;

    // Clearing IF flag before enabling the interrupt.
    PIR3bits.TMR1GIF = 0;

    // Enabling TMR1 interrupt.
    PIE3bits.TMR1GIE = 1;

    // T1CKPS 1:1; T1OSCEN disabled; T1SYNC do_not_synchronize; TMR1CS FOSC; TMR1ON disabled; T1RD16 enabled; 
    T1CON = 0x46;
}

void TMR1_StartTimer(void)
{
    // Start the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 1;
}

void TMR1_StopTimer(void)
{
    // Stop the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 0;
}

uint16_t TMR1_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    
    T1CONbits.T1RD16 = 1;
	
    readValLow = TMR1L;
    readValHigh = TMR1H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR1_WriteTimer(uint16_t timerVal)
{
    if (T1CONbits.T1SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = (uint8_t) timerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON =1;
    }
    else
    {
        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = (uint8_t) timerVal;
    }
}

void TMR1_Reload(void)
{
    TMR1_WriteTimer(timer1ReloadVal);
}

void TMR1_StartSinglePulseAcquisition(void)
{
    T1GCONbits.T1GGO = 1;
}

uint8_t TMR1_CheckGateValueStatus(void)
{
    return T1GCONbits.T1GVAL;
}

bool TMR1_HasOverflowOccured(void)
{
    // check if  overflow has occurred by checking the TMRIF bit
    return(PIR1bits.TMR1IF);
}

volatile const uint16_t* TMR1_list_pulsewidth_measurements(void){
    return pulse_width_measurements;
}

void TMR1_GATE_ISR(void)
{
    T1CONbits.TMR1ON = 0;
    // Skip first measurement as this is a pulse produced by DHT-11 that doesn't encode humidity measurement.
    if(measurement_number > 0 && (measurement_number <= PULSE_WIDTH_MEASUREMENTS_SIZE))
        pulse_width_measurements[measurement_number-1] = TMR1_ReadTimer();
    TMR1_Reload();
    T1CONbits.TMR1ON = 1;
    T1GCONbits.T1GGO = 1;
    measurement_number++;
            
    // Clear the TMR1 interrupt flag
    PIR3bits.TMR1GIF = 0;
}

void TMR1_prepare_new_measurements(void){
    measurement_number = 0;
    memset((void*)pulse_width_measurements, 0, amount_measurements);
}
/**
 End of File
*/
