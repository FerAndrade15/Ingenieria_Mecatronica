/*
 * UART0.h
 *
 * Created: 20/05/2024 1:00:36
 *  Author: ferch
 */ 


#ifndef UART0_H_
#define UART0_H_

void initUART09600(void);				//Settings UART
void writeUART(char character);			//Write function
void writeTextUART(char* text);			//Pointer

#endif /* UART0_H_ */