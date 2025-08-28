//*******************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programación de microcontroladores
// Autor: María Andrade
// Proyecto: Formula 1 - Remote Control Car
// Archivo: F1RCar.asm
// Hardware: ATMEGA328P
// Created: 07/05/2024 9:38:17
//*******************************************************************

//Standard libraries
#define F_CPU 16000000UL				//ATMEGA328p Oscillator frequency
#include <avr/io.h>						//IO definition
#include <avr/interrupt.h>				//Interrupts
#include <stdio.h>						//Constants, macros and types
#include <stdint.h>						//Standard int
#include <stdlib.h>						//Standard library
#include <util/delay.h>					//Delays
#include <string.h>
#include <avr/eeprom.h>

//Own libraries
#include "headers.h"					//Definitions and function's header
#include "ADC/ADC.h"					//ADC library
#include "PWM/PWM.h"					//PWM library
#include "UART/UART0.h"					//UART library
	

//Variables and constants
volatile char bufferRX;							//For reading or manipulating serial communication
uint8_t operating_mode = manual;				//Manual, EEPROM or UART
float	potentiometer_value;					//Temporal register to save reading;
uint8_t switch_settings;						//Buttons and switches configuration
uint8_t adc_channel=ADC4;						//ADC register to read
uint8_t reading_state=1;						//While reading equals 1;
uint8_t reenable_pinchange = 0;					//Debounce delay 
const float positionm90dutycycle = 2.5;			//Servo start position
const uint16_t period20ms = 0x9C40;				//Period of servo signal
const double servodivision = 0.0403137254902;	//Value of digital to angle conversion
char uartaddress[2]; 
char receiveuart[3];
char sendpotentiometer_value[3];
uint8_t countuartaddresssaved=0;
uint8_t uart_control=startuartsel;
uint8_t EEMEM _start[4]			= {220, 0, 0, 127};
uint8_t EEMEM _final[4]			= {0, 0, 0, 0};
uint8_t EEMEM _leftmiddle[4]	= {0, 100, 100, 0};
uint8_t EEMEM _rightfast[4]		= {0, 255, 255, 255};
uint8_t reproduceservos[4];
uint8_t selectedroutine = startposition;
uint8_t starteeprom = 0;

int main(){
	cli();
	portsettings();
	pcint_init();
	initUART09600();
	initPWM(OC0A, inverted, fastmode, until_top, prescaler1024, disable, 0);
	initPWM(OC0B, inverted, fastmode, until_top, prescaler1024, disable, 0);
	initPWM(OC1A, noninverted, fasticrmode, period20ms,prescaler8, disable, positionm90dutycycle);
	initPWM(OC1B, noninverted, fasticrmode, period20ms,prescaler8, disable, positionm90dutycycle);
	adc_init(avcc, left, adc_channel, enable, disable, disable, adcprescaler128);
	ADCSRA |= (1<<ADSC);
	sei();
	while(1){
		switch(operating_mode){
			case manual:	
				if(reenable_pinchange!=0){
					anti_debounce();
				}
				else if(reading_state==0){
					reading_state = 1;
					ADCSRA |= (1<<ADSC);
					sei();
				}
				PORTC |= (1<<DDD1);
				PORTC &= ~((1<<DDD2)|(1<<DDD3));
				break;
			case eeprom:
				PORTC |= (1<<DDD2);
				PORTC &= ~((1<<DDD1)|(1<<DDD3));
				if(reenable_pinchange==modebtn){
					anti_debounce();
					if(starteeprom==startposition){
						cli();
						starteeprom = 0;
						switch(selectedroutine){
							case startposition:
								eeprom_read_block(reproduceservos, &_start, 4);
								break;
							case finalposition:
								eeprom_read_block(reproduceservos, &_final, 4);
								break;
							case leftmiddle:
								eeprom_read_block(reproduceservos, &_leftmiddle, 4);
								break;
							case rightfast:
								eeprom_read_block(reproduceservos, &_rightfast, 4);
								break;
						}
						potentiometer_value = (reproduceservos[0]*servodivision)+positionm90dutycycle;
						updatedutycycle(OC1A, potentiometer_value);
						potentiometer_value = 0xFF - reproduceservos[1];
						updatedutycycle(OC0A, potentiometer_value);
						potentiometer_value = 0xFF - reproduceservos[2];
						updatedutycycle(OC0B, potentiometer_value);
						potentiometer_value = (reproduceservos[3]*servodivision)+positionm90dutycycle;
						updatedutycycle(OC1B, potentiometer_value);
					}
				}
				break;
			case uart:
				if(reenable_pinchange==modebtn){
					anti_debounce();
				}
				PORTC |= (1<<DDD3);
				PORTC &= ~((1<<DDD1)|(1<<DDD2));
				break;
		}
	}
}

void portsettings(void){
	DDRC = (1<<DDD1)|(1<<DDD2)|(1<<DDD3);										//Leds outputs	
	DDRD = 0;																	//All PortD as input
	PORTD |= (1<<DDD3)|(1<<DDD4);												//Buttons pull-up
	//PORTD |= (1<<DDD3)|(1<<DDD5);												//Buttons pull-up
}
void pcint_init(void){
	PCICR = (1<<PCIE2);															//Enable Pin Change Interrupt for PORTD
	PCMSK2 = (1<<PCINT23)|(1<<PCINT20)|(1<<PCINT19);							//Enable Pin Change on PD3, PD4 & PD7	
//	PCMSK2 = (1<<PCINT23)|(1<<PCINT22)|(1<<PCINT21)|(1<<PCINT20)|(1<<PCINT19);	//Enable Pin Change on PD3, PD4, PD5, PD6 & PD7
}
void anti_debounce(void){
	_delay_ms(10);								//Estimated debounce duration
	switch(reenable_pinchange){
		case modebtn:
			if((PIND&0x08)==0x08){
				PCMSK2 = (1<<PCINT23)|(1<<PCINT20)|(1<<PCINT19);
				reenable_pinchange = 0;
			}
			break;
		case select_routine:
			if((PIND&0x10)==0x10){
				PCMSK2 = (1<<PCINT23)|(1<<PCINT20)|(1<<PCINT19);
				reenable_pinchange = 0;
			}
		default:
			break;
	}
	
}
//ADC INTERRUPT	 -----------------------------------------------------//
ISR(ADC_vect){
	cli();
	switch(adc_channel){
		case ADC4:												//pot_direction
			potentiometer_value = (ADCH*servodivision)+positionm90dutycycle;
			updatedutycycle(OC1A, potentiometer_value);
			break;
		case ADC5:												//pot_speed
			potentiometer_value = (0xFF - ADCH)*(100/255);
			updatedutycycle(OC0A, potentiometer_value);
			break;
		case ADC6:												//pot_light
			potentiometer_value = (0xFF - ADCH)*(100/255);
			updatedutycycle(OC0B, potentiometer_value);
			break;
		case ADC7:												//pot_cabin
			potentiometer_value = (ADCH*servodivision)+positionm90dutycycle;
			updatedutycycle(OC1B, potentiometer_value);
			break;
		default:
			break;
	}
	adc_channel = (adc_channel==ADC7)?ADC4:(adc_channel+1);	
	changechannel(adc_channel);
	reading_state = 0;																					//Lecture of the two ADC registers
}
//USART INTERRUPT	 ---------------------------------------------------//
ISR(USART_RX_vect){
	bufferRX = UDR0;																//Reading everything send to the chip
	while(!(UCSR0A&(1<<UDRE0)));
	if((countuartaddresssaved<2)&&(uart_control==startuartsel)){
		uartaddress[countuartaddresssaved]=bufferRX;
		countuartaddresssaved++;
	}else if((countuartaddresssaved<4)&&(uart_control!=startuartsel)){
		receiveuart[countuartaddresssaved]=bufferRX;
		countuartaddresssaved++;
	}
	if((countuartaddresssaved==2)&&(uart_control==startuartsel)){
		if(uartaddress[0]==0x31){
			operating_mode = (uartaddress[1]==0x30)?manual:((uartaddress[1]==0x31)?eeprom:((uartaddress[1]==0x32)?uart:operating_mode));
		}else if((operating_mode==uart)){
			if(uartaddress[0]==0x32){
				uart_control = (uartaddress[1]==0x30)?changeservo1:((uartaddress[1]==0x31)?changeservo2:((uartaddress[1]==0x32)?changeservo3:((uartaddress[1]==0x33)?changeservo4:uart_control)));
			}else if(uartaddress[0]==0x33){
				uart_control = (uartaddress[1]==0x30)?changeswitch1:((uartaddress[1]==0x31)?changeswitch2:((uartaddress[1]==0x32)?changeswitch3:((uartaddress[1]==0x33)?changeswitch4:uart_control)));
			}
		}
		countuartaddresssaved=0;																					//Restart count to save values
	}
	if((countuartaddresssaved==3)&&(uart_control!=startuartsel)&&(uart_control<5)){
		switch(uart_control){
			case changeservo1:
				potentiometer_value = (atoi(receiveuart)*servodivision)+positionm90dutycycle;
				updatedutycycle(OC1A, potentiometer_value);
				break;
			case changeservo2:
				potentiometer_value = atoi(receiveuart);
				updatedutycycle(OC0A, potentiometer_value);
				break;
			case changeservo3:
				potentiometer_value = atoi(receiveuart);
				updatedutycycle(OC0B, potentiometer_value);
				break;
			case changeservo4:
				potentiometer_value = (atoi(receiveuart)*servodivision)+positionm90dutycycle;
				updatedutycycle(OC1B, potentiometer_value);
				break;
			default:
				break;
		}
		countuartaddresssaved=0;																					//Restart count to save values
		uart_control=startuartsel;
	}
}
//PIN CHANGE INTERRUPT	-----------------------------------------------//
ISR(PCINT2_vect){
	if((PIND&0x08)==0){
		reenable_pinchange = modebtn;
		operating_mode = (operating_mode==uart)?manual:operating_mode+1;
		if(operating_mode==manual){
			writeTextUART("10");
		}else if(operating_mode==eeprom){
			writeTextUART("11");
		}else {
			writeTextUART("12");
		}
		PCMSK2 = 0;															//Unable Pin Change Interrupt
	}
	else if((PIND&0x10)==0){
		PCMSK2 = 0;															//Unable Pin Change Interrupt
		reenable_pinchange = select_routine;
		selectedroutine = (selectedroutine<5)?(selectedroutine+1):(1);
		starteeprom = startposition;
	}
}
