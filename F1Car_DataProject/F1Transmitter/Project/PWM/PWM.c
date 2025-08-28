/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE2023: Programación de microcontroladores
/ Autor: María Andrade
/ Proyecto: Laboratorio 5
/ Archivo: PWM.c
/ Hardware: ATMEGA328P
/ Created: 15/04/2024 11:26:42
// ******************************************************************/
#include "PWM.h"

uint8_t maxfromdutycycle = 0;
uint16_t maxfromdutycycle1 = 0;

void initPWM(uint8_t sel_output, uint8_t sel_inverted, uint8_t pwmtype, uint16_t max, uint8_t prescaler, uint8_t enableinterrupt, float dutycycle){
	//Check conditions of input variables
	if((sel_output>=1)&&(sel_output<=6)&&(sel_inverted<=1)&&(pwmtype>=1)&&(pwmtype<=6)&&(max>=0)&&(max<=0xFFFF)&&(prescaler>=1)&&(prescaler<=7)&&(enableinterrupt<=4)&&(dutycycle<=100)){
		switch(sel_output){
			case OC0A:
				DDRD |= (1<<DDD6);															//Set PIND6 as output > OC0A
				TCCR0A = 0;
				TCCR0A |= (sel_inverted==inverted)?((1<<COM0A1)|(1<<COM0A0)):(1<<COM0A1);	//Inverted or non-inverted signal
				TCCR0A |= (pwmtype==fastmode)?((1<<WGM01)|(1<<WGM00)):(1<<WGM00);			//Fast PWM Mode or Phase Correct PWM Mode
				TCCR0B = 0;
				TCCR0B |= (prescaler<=5)?(prescaler):(0x00);								//Prescaler selected
				if((max!=until_top)&&(max<0xFF)){
					TCCR0B |= (1<<WGM02);													//Top on OCRA
					OCR0A = max;
				}else{
					maxfromdutycycle = ((0xFF)*(dutycycle/100));
					OCR0A = maxfromdutycycle;
				}
				if((enableinterrupt!=0)&&(enableinterrupt!=3)){
					TIMSK0 |= enableinterrupt;												//Enable timer interrupts
				}
				break;
			case OC0B:
				DDRD |= (1<<DDD5);															//Set PIND5 as output > OC0B
				PORTD |= (1<<DDD5);	
				TCCR0A |= (sel_inverted==inverted)?((1<<COM0B1)|(1<<COM0B0)):(1<<COM0B1);	//Inverted or non-inverted signal
				TCCR0A |= (pwmtype==fastmode)?((1<<WGM01)|(1<<WGM00)):(1<<WGM00);			//Fast PWM Mode or Phase Correct PWM Mode
				TCCR0B |= (prescaler<=5)?(prescaler):(0x00);								//Prescaler selected
				if((max!=until_top)&&(max<0xFF)){
					TCCR0B |= (1<<WGM02);													//Top on OCRA
					OCR0A = max;
				}
				maxfromdutycycle = ((0xFF)*(dutycycle/100));
				OCR0B = maxfromdutycycle;
				if((enableinterrupt!=0)&&(enableinterrupt!=3)){
					TIMSK0 |= enableinterrupt;												//Enable timer interrupts
				}
				break;
						
			case OC1A:
				DDRB |= (1<<DDB1);															//Set PINB1 as output > OC1A
				TCCR1A |= (sel_inverted==inverted)?((1<<COM1A1)|(1<<COM1A0)):(1<<COM1A1);	//Inverted or non-inverted signal
				TCCR1B |= (prescaler<=5)?(prescaler):(0x00);								//Prescaler selected
				switch(pwmtype){
					case phasecorrectmode:
						TCCR1A |= (max==0xFF)?(1<<WGM10):((max==0x1FF)?(1<<WGM11):((max==0x3FF)?((1<<WGM11)|(1<<WGM10)):0));
						uint8_t OCR1Avaluepc = (max==0xFF)?((0xFF)*(dutycycle/100)):((max==0x1FF)?((0x1FF)*(dutycycle/100)):((max==0x3FF)?((0x3FF)*(dutycycle/100)):0));
						OCR1A = OCR1Avaluepc;
						break;
					case fastmode:
						TCCR1A |= (max==0xFF)?(1<<WGM10):((max==0x1FF)?(1<<WGM11):((max==0x3FF)?((1<<WGM11)|(1<<WGM10)):0));
						TCCR1B |= (1<<WGM12);
						uint8_t OCR1Avaluef = (max==0xFF)?((0xFF)*(dutycycle/100)):((max==0x1FF)?((0x1FF)*(dutycycle/100)):((max==0x3FF)?((0x3FF)*(dutycycle/100)):0));
						OCR1A = OCR1Avaluef;
						break;
					case phasecorrecticrmode:
						TCCR1A |= (1<<WGM11);
						TCCR1B |= (1<<WGM13);
						ICR1 = max;
						OCR1A = dutycycle;
						break;
					case phasecorrectocramode:
						TCCR1A |= (1<<WGM11)|(1<<WGM10);
						TCCR1B |= (1<<WGM13);
						OCR1A = max;
						break;
					case fasticrmode:
						TCCR1A |= (1<<WGM11);
						TCCR1B |= (1<<WGM13)|(1<<WGM12);
						ICR1 = max;
						maxfromdutycycle1 = ((max*dutycycle)/100);
						OCR1A = maxfromdutycycle1;
						break;
					case fastocramode:
						TCCR1A |= (1<<WGM11)|(1<<WGM10);
						TCCR1B |= (1<<WGM13)|(1<<WGM12);
						OCR1A = max;
						break;
					default:
						break;
				}
				if((enableinterrupt!=0)&&(enableinterrupt!=3)){
					TIMSK1 |= enableinterrupt;												//Enable timer interrupts
				}if(enableinterrupt==3){
					TIMSK1 |= (1<<ICIE1);													//Input Capture Interrupt
				}
				break;
			case OC1B:
				DDRB |= (1<<DDB2);															//Set PINB2 as output > OC1B
				TCCR1A |= (sel_inverted==inverted)?((1<<COM1B1)|(1<<COM1B0)):(1<<COM1B1);	//Inverted or non-inverted signal
				TCCR1B |= (prescaler<=5)?(prescaler):(0x00);
				switch(pwmtype){
					case phasecorrectmode:
						TCCR1A |= (max==0xFF)?(1<<WGM10):((max==0x1FF)?(1<<WGM11):((max==0x3FF)?((1<<WGM11)|(1<<WGM10)):0));
						uint8_t OCR1Bvaluepc1 = (max==0xFF)?((0xFF)*(dutycycle/100)):((max==0x1FF)?((0x1FF)*(dutycycle/100)):((max==0x3FF)?((0x3FF)*(dutycycle/100)):0));
						OCR1B = OCR1Bvaluepc1;
						break;
					case fastmode:
						TCCR1A |= (max==0xFF)?(1<<WGM10):((max==0x1FF)?(1<<WGM11):((max==0x3FF)?((1<<WGM11)|(1<<WGM10)):0));
						TCCR1B |= (1<<WGM12);
						uint8_t OCR1Bvaluef1 = (max==0xFF)?((0xFF)*(dutycycle/100)):((max==0x1FF)?((0x1FF)*(dutycycle/100)):((max==0x3FF)?((0x3FF)*(dutycycle/100)):0));
						OCR1B = OCR1Bvaluef1;
						break;

					case phasecorrecticrmode:
						TCCR1A |= (1<<WGM11);
						TCCR1B |= (1<<WGM13);
						ICR1 = max;
						uint8_t OCR1Bvaluepci1 = (max*(dutycycle/100));
						OCR1B = OCR1Bvaluepci1;						
						break;
					case phasecorrectocramode:
						TCCR1A |= (1<<WGM11)|(1<<WGM10);
						TCCR1B |= (1<<WGM13);
						OCR1A = max;
						uint8_t OCR1Bvaluepco1 = (max*(dutycycle/100));
						OCR1B = OCR1Bvaluepco1;						
						break;
					case fasticrmode:
						TCCR1A |= (1<<WGM11);
						TCCR1B |= (1<<WGM13)|(1<<WGM12);
						ICR1 = max;
						maxfromdutycycle1 = ((max*dutycycle)/100);
						OCR1B = maxfromdutycycle1;
						break;
					case fastocramode:
						TCCR1A |= (1<<WGM11)|(1<<WGM10);
						TCCR1B |= (1<<WGM13)|(1<<WGM12);
						OCR1A = max;
						uint8_t OCR1Bvalue = (max*(dutycycle/100));
						OCR1B = OCR1Bvalue;
						break;
					default:
						break;
				}
				if((enableinterrupt!=0)&&(enableinterrupt!=3)){
					TIMSK1 |= enableinterrupt;												//Enable timer interrupts
				}else if(enableinterrupt==3){
					TIMSK1 |= (1<<ICIE1);													//Input Capture Interrupt
				}
				break;
			case OC2A:
				DDRB |= (1<<DDB3);												//Set PINB3 as output > OC2A
				TCCR2A |= (sel_inverted==inverted)?((1<<COM2A1)|(1<<COM2A0)):(1<<COM2A1);	//Inverted or non-inverted signal
				TCCR2A |= (pwmtype==fastmode)?((1<<WGM21)|(1<<WGM20)):(1<<WGM20);			//Fast PWM Mode or Phase Correct PWM Mode
				if(((max!=until_top)||(max!=0xFF))&&(max<0xFF)){
					TCCR2B |= (1<<WGM22);													//Top on OCRA
					OCR2A = max;
				}else if(max==0xFF){
					maxfromdutycycle = ((0xFF)*(dutycycle/100));
					OCR2A = maxfromdutycycle;
				}
				if((enableinterrupt!=0)&&(enableinterrupt!=3)){
					TIMSK2 |= enableinterrupt;												//Enable timer interrupts
				}
				break;
			case OC2B:
				DDRD |= (1<<DDD3);															//Set PIND3 as output > OC2B
				TCCR2A |= (sel_inverted==inverted)?((1<<COM2A1)|(1<<COM2A0)):(1<<COM2A1);	//Inverted or non-inverted signal
				TCCR2A |= (pwmtype==fastmode)?((1<<WGM21)|(1<<WGM20)):(1<<WGM20);			//Fast PWM Mode or Phase Correct PWM Mode
				TCCR2B = (prescaler<=7)?(prescaler):(0x00);									//Prescaler selected
				if(((max!=until_top)||(max!=0xFF))&&(max<0xFF)){
					TCCR2B |= (1<<WGM22);													//Top on OCRB
					OCR2A = max;
					maxfromdutycycle = (max*(dutycycle/100));
					OCR2B = maxfromdutycycle;
				} else if(max<0xFF){
					maxfromdutycycle = ((0xFF)*(dutycycle/100));
					OCR2B = maxfromdutycycle;
				}
				if((enableinterrupt!=0)&&(enableinterrupt!=3))	{
					TIMSK0 |= enableinterrupt;												//Enable timer interrupts
				}
				break;
			default:
				break;
		}	
	}
}

void updatedutycycle(uint8_t sel_output, float dutycycle){
	switch(sel_output){
		case OC0A:
			DDRD |= (1<<DDD6);															//Set PIND6 as output > OC0A
			if(((TCCR0B&(1<WGM02))==0)&&((TCCR0A&(1<WGM00))==1)){
				maxfromdutycycle = ((0xFF*dutycycle)/100);
				OCR0A = maxfromdutycycle;
			}
			break;
		case OC0B:
			DDRD |= (1<<DDD5);															//Set PIND5 as output > OC0B
			maxfromdutycycle = ((0xFF*dutycycle)/100);
			OCR0B = maxfromdutycycle;
			break;
		case OC1A:
			DDRB |= (1<<DDB1);															//Set PINB1 as output > OC1A
			if(((TCCR1B&((1<<WGM13)|(1<<WGM12)))==0)&&((TCCR1A&0x03)>0)){
				maxfromdutycycle1 = ((TCCR1A&0x03)==1)?((0xFF)*(dutycycle/100)):(((TCCR1A&0x03)==2)?((0x1FF)*(dutycycle/100)):(((TCCR1A&0x03)==3)?((0x3FF)*(dutycycle/100)):0));
				OCR1A = maxfromdutycycle1;
			}else if(((TCCR1B&(1<<WGM13))==0)&&((TCCR1A&0x03)>0)){
				maxfromdutycycle1 = ((TCCR1A&0x03)==1)?((0xFF)*(dutycycle/100)):(((TCCR1A&0x03)==2)?((0x1FF)*(dutycycle/100)):(((TCCR1A&0x03)==3)?((0x3FF)*(dutycycle/100)):0));
				OCR1A = maxfromdutycycle1;
			}else if(((TCCR1B&(1<<WGM13))==(1<<WGM13))&&((TCCR1A&0x03)==2)){
				maxfromdutycycle1 = ((ICR1*dutycycle)/100);
				OCR1A = maxfromdutycycle1;	
			}else if(((TCCR1B&((1<<WGM13)|(1<<WGM12)))==((1<<WGM13)|(1<<WGM12)))&&((TCCR1A&0x03)==2)){
				maxfromdutycycle1 = ((ICR1*dutycycle)/100);
				OCR1A = maxfromdutycycle1;
			}break;
		case OC1B:
			DDRB |= (1<<DDB2);															//Set PINB2 as output > OC1B
			if(((TCCR1B&((1<<WGM13)|(1<<WGM12)))==0)&&((TCCR1A&0x03)>0)){
				maxfromdutycycle1 = ((TCCR1A&0x03)==1)?((0xFF)*(dutycycle/100)):(((TCCR1A&0x03)==2)?((0x1FF)*(dutycycle/100)):(((TCCR1A&0x03)==3)?((0x3FF)*(dutycycle/100)):0));
				OCR1B = maxfromdutycycle1;
			}else if(((TCCR1B&(1<<WGM13))==0)&&((TCCR1A&0x03)>0)){
				maxfromdutycycle1 = ((TCCR1A&0x03)==1)?((0xFF)*(dutycycle/100)):(((TCCR1A&0x03)==2)?((0x1FF)*(dutycycle/100)):(((TCCR1A&0x03)==3)?((0x3FF)*(dutycycle/100)):0));
				OCR1B = maxfromdutycycle1;
			}else if(((TCCR1B&(1<<WGM13))==(1<<WGM13))&&((TCCR1A&0x03)==2)){
				maxfromdutycycle1 = (ICR1*(dutycycle/100));
				OCR1B = maxfromdutycycle1;
			}else if(((TCCR1B&(1<<WGM13))==(1<<WGM13))&&((TCCR1A&0x03)==3)){
				maxfromdutycycle1= (OCR1A*(dutycycle/100));
				OCR1B = maxfromdutycycle1;
			}else if(((TCCR1B&((1<<WGM13)|(1<<WGM12)))==((1<<WGM13)|(1<<WGM12)))&&((TCCR1A&0x03)==2)){
				maxfromdutycycle1 = ((ICR1*dutycycle)/100);
				OCR1B = maxfromdutycycle1;
			}else if(((TCCR1B&((1<<WGM13)|(1<<WGM12)))==((1<<WGM13)|(1<<WGM12)))&&((TCCR1A&0x03)==3)){
				maxfromdutycycle1 = (OCR1A*(dutycycle/100));
				OCR1B = maxfromdutycycle1;
			}break;
		case OC2A:
			DDRB |= (1<<DDB3);												//Set PINB3 as output > OC2A
			if(((TCCR2B&(1<WGM02))==0)&&((TCCR2A&(1<WGM00))==1)){
				maxfromdutycycle = ((0xFF*dutycycle)/100);
				OCR2A = maxfromdutycycle;
			}break;
		case OC2B:
			DDRD |= (1<<DDD3);												//Set PIND3 as output > OC2B
			if(OCR2A!=0xFF){
				maxfromdutycycle = ((OCR2A*dutycycle)/100);
				OCR0B = maxfromdutycycle;
			} else{
				maxfromdutycycle = ((0xFF*dutycycle)/100);
				OCR2B = maxfromdutycycle;
			}
			break;
		default:
			break;	
		}		
}