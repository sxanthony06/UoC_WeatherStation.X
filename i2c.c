/**
  MSSP2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    i2c2.c

  @Summary
    This is the generated header file for the MSSP2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for I2C2.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F45K22
        Driver Version    :  2.02
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

#include "i2c.h"
#include <xc.h>
#include <stdlib.h>
#include "device_config.h"

static uint8_t I2C_retrieve_ACK(void);
static void I2C_assert_condition(uint8_t);

/**
 Section: Macro Definitions
*/

#define I2C_TRANSMIT_REG                       SSP2BUF                 // Defines the transmit register used to send data.
#define I2C_RECEIVE_REG                        SSP2BUF                 // Defines the receive register used to receive data.

// The following control bits are used in the I2C state machine to manage
// the I2C module and determine next states.
#define I2C2_WRITE_COLLISION_STATUS_BIT         SSP2CON1bits.WCOL     // Defines the write collision status bit.
#define I2C2_MODE_SELECT_BITS                   SSP2CON1bits.SSPM     // I2C Master Mode control bit.
#define I2C2_MASTER_ENABLE_CONTROL_BITS         SSP2CON1bits.SSPEN    // I2C port enable control bit.

#define I2C_TRANSMISSION_IN_PROGRESS_BIT        SSP2STATbits.R_nW2    // Defines if module is transmitting in I2C MASTER MODE 
#define I2C_BUFFER_FULL_BIT                     SSP2STATbits.BF2      // Defines status of SSP2BUF (full = 1, empty = 0)

#define I2C_START_CONDITION_ENABLE_BIT         SSP2CON2bits.SEN      // I2C START control bit.
#define I2C_REPEAT_START_CONDITION_ENABLE_BIT  SSP2CON2bits.RSEN     // I2C Repeated START control bit.
#define I2C_RECEIVE_ENABLE_BIT                 SSP2CON2bits.RCEN     // I2C Receive enable control bit.
#define I2C_STOP_CONDITION_ENABLE_BIT          SSP2CON2bits.PEN      // I2C STOP control bit.
#define I2C_ACKNOWLEDGE_ENABLE_BIT             SSP2CON2bits.ACKEN    // I2C ACK start control bit.
#define I2C_ACKNOWLEDGE_DATA_BIT               SSP2CON2bits.ACKDT    // I2C ACK data control bit.
#define I2C_ACKNOWLEDGE_STATUS_BIT             SSP2CON2bits.ACKSTAT  // I2C ACK status bit.

#define I2C_REPEAT_START                        1
#define I2C_START                               0
#define I2C_STOP                                2

/**
  Section: Driver Interface
*/

void I2C2_Initialize(void)
{

    // R_nW write_noTX; P stopbit_notdetected; S startbit_notdetected; BF RCinprocess_TXcomplete; SMP Standard Speed; UA dontupdate; CKE disabled; D_nA lastbyte_address; 
    SSP2STAT = 0x80;
    // SSPEN disabled; WCOL no_collision; CKP Idle:Low, Active:High; SSPM FOSC/4_SSPxADD_I2C; SSPOV no_overflow; 
    SSP2CON1 = 0x08;
    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 300ns; AHEN disabled; 
    SSP2CON3 = 0x08;
    // SSPADD 39; 
    SSP2ADD = 0x27;
    
    // clear the interrupt flags
    PIR3bits.SSP2IF = 0;
    PIR3bits.BCL2IF = 0;
	
    // enable the interrupts
    PIE3bits.SSP2IE = 0;
    PIE3bits.BCL2IE = 0;
    
}

int8_t I2C_write(uint8_t dev_id, uint8_t reg_addr, uint8_t reg_data, uint8_t len){
    int8_t rslt = -1;
    
    while((SSP2STAT & 0x1F) | I2C_TRANSMISSION_IN_PROGRESS_BIT);           // wait for I2C MASTER idle mode
    I2C_assert_condition(I2C_START);
    I2C_TRANSMIT_REG = (dev_id<<1);                    // make bit0 '1' to signal transmission to corresponding device, than load address into SSPBUG to start transmission
    if(!I2C_retrieve_ACK()){
        I2C_TRANSMIT_REG = reg_addr;
        if(!I2C_retrieve_ACK()){
            I2C_TRANSMIT_REG = reg_data;
            if(!I2C_retrieve_ACK())
                rslt = 0;                                    // calling function expects 0 for success and negative numbers for failure
        }
    }
    I2C_assert_condition(I2C_STOP);
    return rslt;                         
}

int8_t I2C_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint8_t len){
    int8_t rslt = -1;
    
    while((SSP2STAT & 0x1F) | I2C_TRANSMISSION_IN_PROGRESS_BIT);           // wait for I2C MASTER idle mode
    I2C_assert_condition(I2C_START);
    I2C_TRANSMIT_REG = (dev_id<<1);
    if(!I2C_retrieve_ACK()){
        I2C_TRANSMIT_REG = reg_addr;
        if(!I2C_retrieve_ACK()){
            I2C_assert_condition(I2C_REPEAT_START);
            I2C_TRANSMIT_REG = (dev_id<<1) + 0x01;
            if(!I2C_retrieve_ACK()){
                for(uint8_t i = 0; i < len; i++){
                    I2C_RECEIVE_ENABLE_BIT = 1;
                    while(PIR3bits.SSP2IF == 0 || I2C_BUFFER_FULL_BIT == 1)
                        ;
                        if(reg_data++ != 0){
                            *reg_data = I2C_RECEIVE_REG;
                            PIR3bits.SSP2IF = 0;
                            I2C_ACKNOWLEDGE_DATA_BIT = 0;
                            I2C_ACKNOWLEDGE_ENABLE_BIT = 1;
                        }
                }
                I2C_ACKNOWLEDGE_DATA_BIT = 1;
                I2C_ACKNOWLEDGE_ENABLE_BIT = 1;
                I2C_assert_condition(I2C_STOP);
                rslt = 0;
            }
            
        }
    }
    return rslt;
}

static uint8_t I2C_retrieve_ACK(void){
    while(I2C_BUFFER_FULL_BIT == 1 || PIR3bits.SSP2IF == 0);    // wait till byte has been sent and ACK or NACK received
    PIR3bits.SSP2IF = 0;
    return I2C_ACKNOWLEDGE_STATUS_BIT;
}

static void I2C_assert_condition(uint8_t type_condition){

    switch(type_condition){
    
        case I2C_START:
            do{
                I2C_START_CONDITION_ENABLE_BIT = 1;
                while(PIR3bits.SSP2IF == 0 || PIR3bits.BCL2IF == 0);    // wait for START condition detection while checking for bus collision
                if(PIR3bits.BCL2IF == 1){
                    PIR3bits.BCL2IF = 0;            
                    __delay_us(100);
                    continue;
                }
            }while(!(PIR3bits.SSP2IF == 1 || SSP2STATbits.S2));
            break;
        case I2C_REPEAT_START:
            do{
                I2C_REPEAT_START_CONDITION_ENABLE_BIT = 1;
                while(PIR3bits.SSP2IF == 0 || PIR3bits.BCL2IF == 0);    // wait for START condition detection while checking for bus collision
                if(PIR3bits.BCL2IF == 1){
                    PIR3bits.BCL2IF = 0;            
                    __delay_us(100);
                    continue;
                }
            }while(!(PIR3bits.SSP2IF == 1 || SSP2STATbits.S2));
            break;
        case I2C_STOP:
            I2C_STOP_CONDITION_ENABLE_BIT = 1;                          // commence I2C STOP condition sequence
            while(!(PIR3bits.SSP2IF == 1 || SSP2STATbits.P2));          // wait for detection of STOP condition
            break;
    }
    PIR3bits.SSP2IF = 0;   
}


void I2C_bus_collision_ISR( void )
{
    // enter bus collision handling code here
	PIR3bits.BCL2IF = 0;
}        
        
        
/**
 End of File
*/