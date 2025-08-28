/*******************************************************************
/ Universidad del Valle de Guatemala
/ IE3054: Electrónica Digital 2
/ Autores: Eber Alexander, María Andrade
/ Proyecto: Comunicación I2C, sensor MPU6050
/ Archivo: MPU_6050.c
/ Hardware: ATMEGA328P
// ******************************************************************/

#include "MPU6050.h"

//Global variables
uint8_t addr = 0;
uint8_t gyroscale = 0;
uint8_t accelscale = 0;

void MPU6050_Init(AD0_Connection AD0_address, Gyro_FullScaleRange GFRangeScale, Accel_FullScaleRange AFRangeScale){
addr = AD0_address;
gyroscale = GFRangeScale;
accelscale = AFRangeScale;
//Out of Sleep Mode
MPU6050_WriteRegister(MPU6050_PWR_MGMT_1, 0x00);
_delay_ms(100);
//Secure I2C of module as slave
MPU6050_WriteRegister(MPU6050_USER_CONTROL, 0x00);
_delay_ms(100);
}

uint8_t MPU6050_WriteRegister(uint8_t Register, uint8_t Value){
uint8_t check1; uint8_t check2; uint8_t check3;
I2C_Master_Start();
check1 = I2C_Master_Write(addr<<1);
check2 = I2C_Master_Write(Register);
check3 = I2C_Master_Write(Value);
I2C_Master_Stop();
if((check1==check2)&&(check2==check3)&&(check1==1)){
return 1;
}
else{
return 0;
}
}
uint8_t MPU6050_ReadRegister(uint8_t Register){
uint8_t read_value = 0;
I2C_Master_Start();
I2C_Master_Write(addr<<1);
I2C_Master_Write(Register);
I2C_Master_Start();
I2C_Master_Write((addr<<1)|1);
read_value = I2C_Master_Read(0);
I2C_Master_Stop();
return read_value;
}

uint16_t MPU6050_Ax(void){
uint8_t high; uint8_t low; float ax; float factor;
high = MPU6050_ReadRegister(ACCEL_XOUT_H);
low = MPU6050_ReadRegister(ACCEL_XOUT_L);
ax = ((high<<8)| low);
factor = ((accelscale==ACCEL_2G)?16384:((accelscale==ACCEL_4G)?8192:((accelscale==ACCEL_8G)?4096:2048)));
//Conversion for acceleration based on gravitation
ax = (ax*9.72) / factor ;				//Acceleration in m/s^2
return(ax);
}

uint16_t MPU6050_Ay(void){
uint8_t high; uint8_t low; float ay; float factor;
high = MPU6050_ReadRegister(ACCEL_YOUT_H);
low = MPU6050_ReadRegister(ACCEL_YOUT_L);
ay = ((high<<8)| low);
factor = ((accelscale==ACCEL_2G)?16384:((accelscale==ACCEL_4G)?8192:((accelscale==ACCEL_8G)?4096:2048)));
//Conversion for acceleration based on gravitation
ay = (ay*9.72) / factor ;					//Acceleration in m/s^2
return(ay);
}
uint16_t MPU6050_Az(void){
uint8_t high; uint8_t low; float az; float factor;
high = MPU6050_ReadRegister(ACCEL_ZOUT_H);
low = MPU6050_ReadRegister(ACCEL_ZOUT_L);
az = ((high<<8)| low);
factor = ((accelscale==ACCEL_2G)?16384:((accelscale==ACCEL_4G)?8192:((accelscale==ACCEL_8G)?4096:2048)));
//Conversion for acceleration based on gravitation
az = (az*9.72) / factor ;					//Acceleration in m/s^2
return(az);
}
float MPU6050_Gx(void){
uint8_t high; uint8_t low; float gx; float factor;
high = MPU6050_ReadRegister(GYRO_XOUT_H);
low = MPU6050_ReadRegister(GYRO_XOUT_L);
gx = ((high<<8)| low);
factor = ((gyroscale==GYRO_250DS)?131:((gyroscale==GYRO_500DS)?65.5:((gyroscale==GYRO_1000DS)?32.8:16.4)));
//Conversion for acceleration based on gravitation
gx = (M_PI * gx) / (factor * 180) ;		//Gyroscope in rad/s
return(gx);
}
float MPU6050_Gy(void){
uint8_t high; uint8_t low; float gy; float factor;
high = MPU6050_ReadRegister(GYRO_YOUT_H);
low = MPU6050_ReadRegister(GYRO_YOUT_L);
gy = ((high<<8)| low);
factor = ((gyroscale==GYRO_250DS)?131:((gyroscale==GYRO_500DS)?65.5:((gyroscale==GYRO_1000DS)?32.8:16.4)));
//Conversion for acceleration based on gravitation
gy = (M_PI * gy) / (factor * 180) ;
return(gy);
}
float MPU6050_Gz(void){
uint8_t high; uint8_t low; float gz; float factor;
high = MPU6050_ReadRegister(GYRO_ZOUT_H);
low = MPU6050_ReadRegister(GYRO_ZOUT_L);
gz = ((high<<8)| low);
factor = ((gyroscale==GYRO_250DS)?131:((gyroscale==GYRO_500DS)?65.5:((gyroscale==GYRO_1000DS)?32.8:16.4)));
//Conversion for acceleration based on gravitation
gz = (M_PI * gz) / (factor * 180) ;
return(gz);
}