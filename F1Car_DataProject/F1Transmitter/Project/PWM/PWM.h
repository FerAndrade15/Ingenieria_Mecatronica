/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE2023: Programación de microcontroladores
/ Autor: María Andrade
/ Proyecto: Laboratorio 5
/ Archivo: PWM.h
/ Hardware: ATMEGA328P
/ Created: 15/04/2024 9:36:36
// ******************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define OC0A 1
#define OC0B 2
#define OC1A 3
#define OC1B 4
#define OC2A 5
#define OC2B 6
#define timer0 0
#define timer1 1
#define timer2 2
#define disable 0
#define until_top 0
#define inverted		1
#define noninverted		0
#define prescaler1		1
#define prescaler8		2
#define prescaler64		3
#define prescaler256	4
#define prescaler1024	5
#define prescaler2_1		1
#define prescaler2_8		2
#define prescaler2_32		3
#define prescaler2_64		4
#define prescaler2_128		5
#define prescaler2_256		6
#define prescaler2_1024		7
#define nointerrpts			0
#define overflowinterrupt	1
#define matchAinterrupt		2
#define icaptureinterrupt	3
#define matchBinterrupt		4
#define phasecorrectmode		1
#define fastmode				2
#define phasecorrecticrmode		3
#define phasecorrectocramode	4
#define fasticrmode				5
#define fastocramode			6

void initPWM(uint8_t sel_output, uint8_t sel_inverted, uint8_t pwmtype, uint16_t max, uint8_t prescaler, uint8_t enableinterrupt, float dutycycle);
void updatedutycycle(uint8_t sel_output, float dutycycle);
void initmanualPWM(uint8_t sel_timer);
#endif /* PWM_H_ */