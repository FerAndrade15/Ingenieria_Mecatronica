/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE3054: Electrónica Digital 2
/ Autores: Eber Alexander, María Andrade
/ Proyecto: Protocolo de comunicación I2C
/ Archivo: I2C.h
/ Hardware: ATMEGA328P
// ******************************************************************/

#ifndef I2C_H_
#define I2C_H_

//Essential Libraries
#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>

//Functions prototypes
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Presaler);	//Initialize I2C Communication
void I2C_Master_Start(void);										//Start signal
void I2C_Master_Stop(void);											//Stop signal
uint8_t I2C_Master_Read(uint8_t ack);								//Read from Slave
void I2C_Slave_Init(uint8_t address);								//Initialize Slave
uint8_t I2C_Master_Write(uint8_t dato);								//Write to Slave

#endif /* I2C_H_ */