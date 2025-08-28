/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE3054: Electrónica Digital 2
/ Autores: Eber Alexander, María Andrade
/ Proyecto: Comunicación I2C, sensor MPU6050
/ Archivo: MPU_6050.h
/ Hardware: ATMEGA328P
// ******************************************************************/

#ifndef MPU6050_H_
#define MPU6050_H_

//Required Libraries
//Essential Libraries

//Required Libraries
#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//Communication libraries
#include "../I2C/I2C.h"
typedef enum{
Address_AD0_0 = 0x68,
Address_AD0_1 = 0x69
} AD0_Connection;

typedef enum{
GYRO_250DS  = 0x00,				// Full Scale Range = 250 °/s
GYRO_500DS  = (1<<3),			// Full Scale Range = 500 °/s
GYRO_1000DS = (1<<4),			// Full Scale Range = 1000 °/s
GYRO_2000DS = (1<<3)|(1<<4)		// Full Scale Range = 2000 °/s
} Gyro_FullScaleRange;

typedef enum{
ACCEL_2G  = 0x00,				// Full Scale Range = 2g
ACCEL_4G  = (1<<3),				// Full Scale Range = 4g
ACCEL_8G  = (1<<4),				// Full Scale Range = 8g
ACCEL_16G = (1<<3)|(1<<4)		// Full Scale Range = 16g
} Accel_FullScaleRange;

//Self test factory calibrated values register
#define MPU6050_SELF_TEST_X		0x0D
#define MPU6050_SELF_TEST_Y		0x0E
#define MPU6050_SELF_TEST_Z		0x0F
#define MPU6050_SELF_TEST_A		0x10
//General configuration register
#define MPU6050_CONFIG			0x1A
//Specific configuration registers
#define MPU6050_GYRO_CONFIG		0x1B
#define MPU6050_ACCEL_CONFIG	0x1C
//Interrupt pin configuration register
#define MPU6050_INT_PIN_CONFIG	0x37
#define MPU6050_INT_ENABLE		0x38
#define MPU6050_INT_STATUS		0x3A
//Temperature registers
#define MPU6050_TEMP_H			0x41
#define MPU6050_TEMP_L			0x42
//Acceleration registers
#define ACCEL_XOUT_H			0x3B
#define ACCEL_XOUT_L			0x3C
#define ACCEL_YOUT_H			0x3D
#define ACCEL_YOUT_L			0x3E
#define ACCEL_ZOUT_H			0x3F
#define ACCEL_ZOUT_L			0x40
//Gyroscope registers
#define GYRO_XOUT_H				0x43
#define GYRO_XOUT_L				0x44
#define GYRO_YOUT_H				0x45
#define GYRO_YOUT_L				0x46
#define GYRO_ZOUT_H				0x47
#define GYRO_ZOUT_L				0x48
//Device ID Register
#define MPU6050_WHO_AM_I		0x75
//FIFO and CTRLl of registers
#define MPU6050_SIGNAL_PATH_RESET	0x68	//Signal path reset register
#define MPU6050_USER_CTRL			0x6A	//FIFO and I2C Master control register
//Power/sleep control registers
#define MPU6050_PWR_MGMT_1		0x6B
#define MPU6050_PWR_MGMT_2		0x6C
//Control Registers
#define MPU6050_USER_CONTROL	0x6A
#define MPU6050_MOT_THR			0x1F
#define MPU6050_MOT_DUR			0x20

//Functions Prototypes
void MPU6050_Init(AD0_Connection AD0_address, Gyro_FullScaleRange GFRangeScale, Accel_FullScaleRange AFRangeScale);
uint8_t MPU6050_WriteRegister(uint8_t Register, uint8_t Value);
uint8_t MPU6050_ReadRegister(uint8_t Register);
uint16_t MPU6050_Ax(void);
uint16_t MPU6050_Ay(void);
uint16_t MPU6050_Az(void);
float MPU6050_Gx(void);
float MPU6050_Gy(void);
float MPU6050_Gz(void);


#endif /* MPU6050_H_ */