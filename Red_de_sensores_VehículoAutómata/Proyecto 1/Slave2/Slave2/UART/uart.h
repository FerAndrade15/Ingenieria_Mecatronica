/*
 * uart.h
 *
 * Created: 11/02/2025 08:36:14
 *  Author: Alex Cuxe
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


// Inicialización del módulo UART
void initUART9600(void);


// Escribir una cadena
void writeTextUART(char* text);


// Muestra en el monitor serie
void serialShow(volatile char buffer);



#endif /* UART_H_ */