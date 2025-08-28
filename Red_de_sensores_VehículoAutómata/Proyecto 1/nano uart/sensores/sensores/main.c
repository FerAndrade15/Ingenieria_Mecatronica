/*
 * sensores.c
 *
 * Created: 3/3/2025 2:19:16 AM
 * Author : aleks
 */ 




#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdlib.h>
#include "UART/uart.h"


volatile char bufferRX;
uint8_t counter = 0;

int main(void)
{
	cli();
	initUART9600();
	sei();
	
	char cadenaADC[32]; // Suficiente para "A" + valor ADC máximo (1023) + '\0'
	int valorADC_A = 0, valorADC_B = 0, valorADC_C = 0;
	
    while (1) 
    {
		sprintf(cadenaADC, "A%d\nB%d\nC%d", valorADC_A, valorADC_B, valorADC_C); // Convierte el valor a string y agrega "A"
		valorADC_A++; valorADC_B = 100; valorADC_C+=2;
		writeTextUART(cadenaADC);
		_delay_ms(1250);
    }
}




/*
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART/uart.h"

#define TRIG_PIN 2
#define ECHO_PIN 3

volatile unsigned long tiempoInicio = 0;
volatile unsigned long tiempoFin = 0;
volatile unsigned long duracionPulso = 0;
volatile int flancoSubida = 0;


void configurarUltrasonico() {
	DDRD |= (1 << TRIG_PIN); // TRIG como salida
	DDRD &= ~(1 << ECHO_PIN); // ECHO como entrada

	// Configurar interrupción por cambio de pin en ECHO_PIN
	PCICR |= (1 << PCIE2); // Habilitar interrupciones de cambio de pin en el puerto D
	PCMSK2 |= (1 << PCINT19); // Habilitar interrupción en ECHO_PIN (PCINT19)
}



int leerDistancia() {
	// Enviar pulso TRIG
	PORTD |= (1 << TRIG_PIN);
	_delay_us(10);
	PORTD &= ~(1 << TRIG_PIN);
	// esperar que la interrupcion ocurra
	_delay_ms(50);
	// Calcular distancia (en centímetros)
	return duracionPulso * 0.0034 *0.65 / 2;
}

int main(void) {
	configurarUltrasonico();
	initUART9600();
	// Configurar Timer1 para medir el tiempo
	TCCR1B |= (1 << CS10); // Prescaler 1 (sin prescaler)
	sei(); // Habilitar interrupciones globales

	char cadenaCompleta[16];

	while (1) {
		int distancia = leerDistancia();
		sprintf(cadenaCompleta, "D:%d cm\n", distancia);
		writeTextUART(cadenaCompleta);
		_delay_ms(50);
	}
}


ISR(PCINT2_vect) {
	if (PIND & (1 << ECHO_PIN)) { // Flanco de subida
		tiempoInicio = TCNT1; // Obtener valor del temporizador
		flancoSubida = 1;
		} else { // Flanco de bajada
		if (flancoSubida) {
			tiempoFin = TCNT1; // Obtener valor del temporizador
			duracionPulso = tiempoFin - tiempoInicio;
			flancoSubida = 0;
		}
	}
}*/