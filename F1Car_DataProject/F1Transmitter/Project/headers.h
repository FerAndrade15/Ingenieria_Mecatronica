/*
 * headers.h
 *
 * Created: 20/05/2024 0:56:00
 *  Author: ferch
 */ 

#ifndef HEADERS_H_
#define HEADERS_H_

//General definitions
#define manual	1
#define eeprom	2
#define uart	3
#define modebtn			3
#define select_routine	4
//Uart definitions
#define startuartsel	0
#define changeservo1	1
#define changeservo2	2
#define changeservo3	3
#define changeservo4	4
#define changeswitch1	5
#define changeswitch2	6
#define changeswitch3	7
#define changeswitch4	8
//EEPROM
#define startposition	1
#define finalposition	2
#define leftmiddle		3
#define rightfast		4
//Functions
void portsettings(void);
void pcint_init(void);
void anti_debounce(void);


#endif /* HEADERS_H_ */