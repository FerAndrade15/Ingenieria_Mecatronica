/*
 * adc.h
 *
 * Created: 11/02/2025 05:39:48
 *  Author: Alex Cuxe
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


// Inicialización
void initADC(void);

/* Seleccionar pin para ADC
	Seleccionar del 0 al 5
*/
void pinADC(uint8_t a);


// Mostrar ADC, del 0 al 7
uint8_t read_channelADC(uint8_t channel);


//iniciar conversión
void convertADC(void);

// mapeo de voltaje
float mapingADC(uint8_t channel);


#endif /* ADC_H_ */