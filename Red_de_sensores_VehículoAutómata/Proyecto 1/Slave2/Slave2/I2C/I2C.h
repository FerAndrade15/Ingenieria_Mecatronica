/*
 * I2C.h
 *
 * Created: 18/02/2025 16:04:34
 *  Author: ferch
 */ 


#ifndef I2C_H_
#define I2C_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>

void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Presaler);
void I2C_Master_Start(void);
void IC_Master_Stop(void);
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);
void I2C_Slave_Init(uint8_t address);


#endif /* I2C_H_ */