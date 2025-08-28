/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE3054: Electrónica Digital 2
/ Autores: Eber Alexander, María Andrade
/ Proyecto: Protocolo de comunicación I2C
/ Archivo: I2C.c
/ Hardware: ATMEGA328P
// ******************************************************************/

//Add header file
#include "I2C.h"

//Initialize I2C Communication
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
DDRC &= ((1<<DDC4)|(1<<DDC5));
switch(Prescaler){
case 1:
TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
break;
case 4:
TWSR  &= ~(1<<TWPS1);
TWSR |= (1<<TWPS0);
break;
case 16:
TWSR  &= ~(1<<TWPS0);
TWSR |= (1<<TWPS1);
break;
case 64:
TWSR  |= (1<<TWPS1)|(1<<TWPS0);
break;
default:
TWSR  &= ~((1<<TWPS1)|(1<<TWPS0));
Prescaler = 1;
break;
}
TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler);
TWCR |= (1<<TWEN);
}

//Start Signal
void I2C_Master_Start(void){
TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN);	//Start with control settings
while (!(TWCR & (1<<TWINT)));				//Wait for transmission
}

//Stop Signal
void I2C_Master_Stop(void){
TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO);		//Stop with control settings
while (TWCR & (1<<TWSTO));					//Wait for transmission
}

//Read from Slave
uint8_t I2C_Master_Read(uint8_t ack){
TWCR = (1<<TWINT) | (1<<TWEN) | (ack ? (1<<TWEA) : 0);	//Read settings of control register and return of ACK or NACK
while (!(TWCR & (1<<TWINT)));							//Wait for transmission
return TWDR;											//Return data read
}

//Write to Slave
uint8_t I2C_Master_Write(uint8_t dato){
uint8_t estado;
TWDR = dato;											//Value to send
TWCR = (1<<TWEN)|(1<<TWINT);							//Control for writing
while(!(TWCR &(1<<TWINT)));								//Wait for transmission
estado = TWSR & 0xF8;									//Check actual reactions of the MCU
if ((estado==0x18)||(estado==0x28)){					//Return ACK or NACK
return 1;											//Check output
}
else{
return estado;										//Return anything else
}
}

//Initialize Slave
void I2C_Slave_Init(uint8_t address){
DDRC &= ~((1<<DDC4)|(1<<DDC5));							//Set SDA and SCK as outputs
TWAR = address <<1;										//Set the device direction, to recognize communication
TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);					//Enable I2C interrupt and communication protocol
}