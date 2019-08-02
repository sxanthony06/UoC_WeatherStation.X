/* 
 * File:   i2c.h
 * Author: sxant
 *
 * Created on August 2, 2019, 12:25 AM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>    

int8_t I2C_write(uint8_t dev_id, uint8_t reg_addr, uint8_t reg_data, uint8_t len);
int8_t I2C_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint8_t len);
void I2C_bus_collision_ISR(void);
void I2C2_Initialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

