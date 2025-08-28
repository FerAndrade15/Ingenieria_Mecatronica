/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE3054: Electrónica Digital 2
/ Autores: Eber Alexander, María Andrade
/ Proyecto: Proyecto 1 - Master
/ Archivo: main.c
/ Hardware: ATMEGA328P
// ******************************************************************/

//Required Libraries
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
//Input Libraries
#include "I2C/I2C.h"
#include "UART/UART.h"
#include "MPU6050/MPU_6050.h"
//Output Libraries
#include "LCD/LCD.h"

/*//Slave Directions
#define addSlave1 0x20
#define addSlave2 0x21

//Modes
#define control		1
#define autonomous	2

//Movements
#define straight	0
#define reverse		1
#define left		2
#define	right		3



//Default values
#define delta_t		0.5

//Variables
float accel_x = 0;
float accel_y = 0;
float accel_z = 0;
float gyro_x = 0;
float gyro_y = 0;
float gyro_z = 0;
char send_info[100];
uint8_t mode = control;

//Variables to Show or Calculate
uint8_t pitch = 0;
uint8_t roll = 0;
float angle = 0;
float speed = 0;
uint8_t distance = 0;
uint8_t light = 0;

//Variables for Current Use
char temp[100];
char * temp2;
volatile uint32_t millis_counter = 0;
volatile uint32_t millis_counter2 = 0;

//Light reactions
#define lightson	0x40
#define lightsoff	0x41
//Servomotor positions
#define sright		0x42
#define sleft		0x43
#define scenter		0x44
//DC reactions
#define motoron		0x45
#define motoroff	0x46
#define	motorrev	0x47
//Control
uint8_t manual= 1;
uint8_t wifi=2;

//Motors variables
uint8_t movement1[] = {straight, right, straight, right, straight, right, straight, right};		//Simple Square Path Movement
uint8_t movement2[] = {straight, left, straight, right, straight, left, straight, right};		//Zig-Zag Movement
uint8_t movement3[] = {straight, straight, reverse, left, straight, right, straight};			//Reverse and Correct
uint8_t movement4[] = {straight, left, left, straight, right, right, straight};					//U-Turn Simulation

uint8_t rute = 1;
uint8_t valor = 1;
uint8_t direction = 0;

//Essential variables
volatile char bufferRX;
char bufferUART[8];
uint8_t indiceBuffer = 0;
char cadenaADC[32]; // Suficiente para "A" + valor ADC máximo (1023) + '\0'
int valorADC_A = 0, valorADC_B = 0, valorADC_C = 0;
uint8_t global_counter = 0;
uint8_t servo_low = 0;

//Functions prototypes
void Timer0_init(void);
*/
int main(void){
	cli();
	//Initialize communication protocols
	/*I2C_Master_Init(100000, 1);
	initUART0(Asynchronous_FM, Disable, _8bit, Disable, BR9600);	// UART Initialization
	//Initialize Gyroscope & Accelerometer Sensor
	MPU6050_Init(Address_AD0_0, GYRO_250DS, ACCEL_2G);
	//Initialize LCD 16x2*/
	init_8bits(_PortD2, _PortD3, _PortD4, _PortD5, _PortD6, _PortD7, _PortB0, _PortB1, _PortB2, _PortB3);
	//Timer0_init();
	/*DDRB |= (1<<PORTB5);
	_delay_ms(2);
	sei();*/
	//Default values
	/*direction = motoron;
	control = manual;*/
	while(1){/*
	accel_z = MPU6050_Az();
	//Gyroscope Sensors
	gyro_z = MPU6050_Gz();
	_delay_ms(20);
	//Speed and current angle
	speed = accel_z;
	//Slave Movement Data Received
	I2C_Master_Start();
	PORTB |= (1<<PORTB5);
	if(!I2C_Master_Write((addSlave1 << 1)|1)){
	I2C_Master_Stop();
	}
	else{
	distance = I2C_Master_Read(0);
	I2C_Master_Stop();
	_delay_ms(50);							//Visible Indicator
	if ((distance <= 20)&&(control==manual))	{
	direction = motoroff;
	}
	else if ((distance > 20)&&(control==manual)){
	direction = motoron;
	}
	I2C_Master_Start();
	I2C_Master_Write(addSlave1<<1);
	I2C_Master_Write(direction);
	I2C_Master_Stop();
	PORTB &=~ (1<<5);
	}
	_delay_ms(5);
	//Slave Movement Data Received
	I2C_Master_Start();
	PORTB |= (1<<PORTB5);
	if(!I2C_Master_Write((addSlave2 << 1)|1)){
	I2C_Master_Stop();
	}
	else{
	light = I2C_Master_Read(0);
	I2C_Master_Stop();
	_delay_ms(50);							//Visible Indicator
	PORTB &=~ (1<<5);
	//Reaction to Master
	if(light>=170){
	I2C_Master_Start();
	I2C_Master_Write(addSlave2<<1);
	I2C_Master_Write(lightson);
	I2C_Master_Stop();
	}
	else{
	I2C_Master_Start();
	I2C_Master_Write(addSlave2<<1);
	I2C_Master_Write(lightsoff);
	I2C_Master_Stop();
	}
	if (valor==0){
	I2C_Master_Start();
	I2C_Master_Write(addSlave2<<1);
	I2C_Master_Write(sleft);
	I2C_Master_Stop();
	if (control == manual){
	valor = 1;
	}
	}
	else if (valor==1){
	I2C_Master_Start();
	I2C_Master_Write(addSlave2<<1);
	I2C_Master_Write(sright);
	I2C_Master_Stop();
	if (control == manual)	{
	valor = 2;
	}
	}
	else {
	I2C_Master_Start();
	I2C_Master_Write(addSlave2<<1);
	I2C_Master_Write(scenter);
	I2C_Master_Stop();
	if (control==manual){
	valor = 0;
	}
	}
	}
	_delay_ms(5);*/
	//LCD Titles
	LCD_SetCursor_8bits(2,1);
	LCD_WriteString_8bits("W:");
	LCD_SetCursor_8bits(7,1);
	LCD_WriteString_8bits("d:");
	LCD_SetCursor_8bits(11,1);
	LCD_WriteString_8bits("Light:");
	//LCD Cleaning
	LCD_SetCursor_8bits(2,2);
	LCD_WriteString_8bits("       ");
	LCD_SetCursor_8bits(7,2);
	LCD_WriteString_8bits("      ");
	LCD_SetCursor_8bits(11,2);
	LCD_WriteString_8bits("     ");
	//LCD Readings
	/*
	itoa_change(speed, temp, 10);
	LCD_SetCursor_8bits(2, 2);
	LCD_WriteString_8bits(temp);
	itoa_change(distance, temp, 10);
	LCD_SetCursor_8bits(7,2);
	LCD_WriteString_8bits(temp);
	itoa_change(light, temp, 10);
	LCD_SetCursor_8bits(11,2);
	LCD_WriteString_8bits(temp);
	_delay_ms(50);
	global_counter++;
	if (global_counter==8)	{
	//Envío a Adafruit
	int new = accel_z;
	sprintf(cadenaADC, "A%d\nB%d\nC%d", new, light, distance); // Convierte el valor a string y agrega "A"
	writeTextUART(cadenaADC);
	global_counter = 0;
	}*/
	_delay_ms(500);
	}

	}

	ISR(TIMER0_COMPA_vect) {
	//millis_counter++;						// Incrementa cada 1 ms
	}

	void Timer0_init() {
	TIMSK0 = 0;
	TIMSK0 |= (1<<OCIE0A);											//Enable Interrupt Mask for Compare Match A Timer0
	TCCR0A = 0;
	TCCR0A |= (1<<OCIE0A);											//Timer0 mode >> CTC Mode
	OCR0A = 16;													//Max for CTC Mode Timer0 >> Generates interrrupt0
	TCCR0B = 0;
	TCCR0B |= (1<<CS02)|(1<<CS00);									//Timer0 clock selected >> Prescaler at 1024
	}

	ISR(USART_RX_vect){
	/*bufferRX = UDR0;
	while(!(UCSR0A & (1 << UDRE0)));	//if buffer is emptym, if it is not, it waits
	char caracter = UDR0;

	serialShow(bufferRX);

	if (caracter == '\n' || indiceBuffer  >= sizeof(bufferUART) - 1) {
	writeTextUART(bufferUART);
	bufferUART[indiceBuffer ] = '\0';
	indiceBuffer  = 0;

	char letra;
	int numero;

	sscanf(bufferUART, "%c%d", &letra, &numero);

	if ((letra == 'S')) {
	switch(numero){
	case 0:
	if (direction==motoroff){
	control = manual;
	}
	else{
	servo_low = 1;
	}
	break;
	case 1:
	control = wifi;
	valor = 0;
	break;
	case 2:
	control = wifi;
	valor = 1;
	break;
	case 3:
	control = wifi;
	valor = 2;
	break;
	default:
	break;
	}
	}else if (letra == 'D') {
	switch(numero){
	case 0:
	if (servo_low == 1){
	servo_low = 0;
	control = manual;
	direction = motoroff;
	}
	break;
	case 1:
	control = wifi;
	direction = motoron;
	break;
	case 2:
	control = wifi;
	direction = motorrev;
	break;
	default:
	break;
	}
	}

	} else {
	bufferUART[indiceBuffer ++] = caracter;
	}*/