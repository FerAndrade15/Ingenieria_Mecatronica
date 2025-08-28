/*
 * main.c
 *
 * Created: 2/18/2025 4:59:20 PM
 *  Author: ferch
*/ 
//Librerías
#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"
#include "ADCal/adc.h"
#include "PWM1/PWM1.h"
#include "UART/uart.h"

//Directions
#define address 0x21

//Leds
#define led1	(1<<2)
#define led2	(1<<3)

//Variables
uint8_t conversion = 0;
uint8_t delay1 = 0;
uint8_t delay2 = 0;
char buffer;
uint8_t registro = 0;
uint8_t fotores = 0;
//uint8_t selectedadc = ADC3;

//Servomotor positions
#define right	0xF0
#define left	0xFF
#define center	0xFA

//Function prototypes
void init_pins(void);

	char * temp;


int main(void){
	cli();
	DDRC = 0;
	DDRB |= (1 << 5) | (1 << 1) | (1 << 2);
	I2C_Slave_Init(address);
	initUART9600();
	initADC();
	pinADC(3);
	init_pins();
	initFastPWM1(settedUp, 8);
	channel(channelA, nop);
	topValue(39999);
	initUART9600();
	sei();
	while(1) {
		convertADC();
	}
}

void init_pins(){
	DDRB |= (1<<PORTB5);
	DDRD |= led1 | led2;
}

ISR(ADC_vect){
	ADCSRA |= (1 << ADIF);	//turn on flag
	fotores = read_channelADC(3);//read_changechannel(selectedadc);
	convertServo(registro, channelA);
	if(fotores >= 150) PORTB &= ~(1 << 5);							// turn off lights
	else PORTB |= (1 << 5);											// turn on lights
	sprintf(temp, "%d", fotores);
	writeTextUART(temp);
}

ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xF8;
	switch(estado){
		case 0X60:
		case 0x70:
		TWCR |= (1<<TWINT);
		break;
		case 0x80:
		case 0x90:
		buffer = TWDR;											//Receive info
		TWCR |= (1<<TWINT)|(1<<TWEA);
		switch(buffer){
			case 0x40:
				PORTD |= (led1|led2);
				break;
			case 0x41:
				PORTD &= ~(led1|led2);
				break;
			case 0x42:
				registro = 157;
				break;
			case 0x43:
				registro = 15;
				break;
			case 0x44:
				registro = 70;
				break;	
			default:
				break;
		}
		break;
		case 0xA8:
		case 0xB8:
			TWDR = fotores;											//Send info
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
			PORTB ^= (1<< PORTB5);
		break;

		default:
		TWCR |= (1<<TWINT)|(1<<TWSTO);
		break;
		
	}
}

