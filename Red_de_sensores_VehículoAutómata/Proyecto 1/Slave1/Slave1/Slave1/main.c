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
#include "UART/uart.h"

//Directions
#define address 0x20

//Prototipos de función
void pins_init(void);
void pcint_init(void);

//Variables
uint8_t counter = 0;
uint8_t distanciaR = 0;
char buffer;
volatile unsigned long tiempoInicio = 0;
volatile unsigned long tiempoFin = 0;
volatile unsigned long duracionPulso = 0;
volatile int flancoSubida = 0;

// Sensor pins
#define TRIG_PIN 2
#define ECHO_PIN 3


//Movements
#define straight	0
#define reverse		1
#define left		2
#define	right		3

//Bits for H-Bridge
#define HB1	(1 << 4)
#define HB2	(1 << 5)
#define HB3	(1 << 6)
#define HB4	(1 << 7)

//Motor Variables
float positionm90dutycycle = 2.3;
const uint16_t period20ms = 0x9C40;
const double servodivision = 0.0403137254902;
const double leddivision = 0.6093750;
float PWMdutycycle = 0;
uint8_t received = 0;

void configurarUltrasonico() {
	DDRD |= (1 << TRIG_PIN); // TRIG como salida
	DDRD |= HB1|HB2|HB3|HB4;
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
	return duracionPulso * 0.0034 * 0.65 / 2;
}

int main(void) {
	DDRB |= (1 << PORTB5);
	PORTB |= (1 << PORTB5);
	configurarUltrasonico();
	I2C_Slave_Init(address);
	initUART9600();
	// Configurar Timer1 para medir el tiempo
	TCCR1B |= (1 << CS10); // Prescaler 1 (sin prescaler)
	sei(); // Habilitar interrupciones globales
	char cadenaCompleta[16];
	while (1) {
		int distancia = leerDistancia();
		sprintf(cadenaCompleta, "D:%d cm\n", distancia);
		writeTextUART(cadenaCompleta);
		if (distancia > 200) distanciaR = distanciaR;
		else distanciaR = distancia;
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
			case 0x45:
				PORTD |= HB2|HB4;
				PORTD &= ~(HB1|HB3);
				break;
			case 0x46:
				PORTB &= ~(HB1|HB2|HB3|HB4);
				break;
			case 0x47:
				PORTD |= HB1|HB3;
				PORTD &= ~(HB2|HB4);
				break;
			default:
				break;
		}
		break;
		case 0xA8:
		case 0xB8:
		TWDR = distanciaR;											//Send info
		TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
		PORTB ^= (1<< PORTB5);
		break;

		default:
		TWCR |= (1<<TWINT)|(1<<TWSTO);
		break;
		
	}
}