/*
 * UART0.c
 *
 * Created: 20/05/2024 1:00:21
 *  Author: ferch
 */ 

//Libraries
#define F_CPU 16000000UL //CLK Frequency
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

//Functions and variables&constants
volatile char bufferRX;

void initUART09600(void){
	DDRD &= ~(1<<DDD0);		//RX input
	DDRD |= (1<<DDD1);		//TX output
	UCSR0A = 0;
	UCSR0A |= (1<<U2X0);	//Fastmode U2X0 = 1
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0);	//ISR RX Interrupt
	UCSR0B |= (1<<RXEN0);	//RX Enable
	UCSR0B |= (1<<TXEN0);	//TX Enable
	UCSR0C = 0;
	UCSR0C = (1<<UCSZ01)| (1<<UCSZ00);
	UBRR0 = 207;
}

void writeUART(char character){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0 = character;
}

void writeTextUART(char* text){
	for (uint8_t i=0; text[i]!='\0'; i++){
		while(!(UCSR0A&(1<<UDRE0)));
		UDR0 = text[i];
	}
}