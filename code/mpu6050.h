#ifndef __MPU6050
#define __MPU6050

#include "main.h"
#include "i2c.h"
#include "mpu6050_Reg.h"
#define mpu6050_Address	 0xD0



void mpu6050_Init(void);
void mpu6050_GetAccelData(int16_t* AX,int16_t* AY,int16_t* AZ);
void mpu6050_GetGyroData(int16_t* GX,int16_t* GY,int16_t* GZ);
void mpu6050_WriteMemReg(uint8_t MemAddress,uint8_t Data); 
void mpu6050_WriteMemRegs(uint8_t MemAddress,uint8_t *Data,uint8_t length);
HAL_StatusTypeDef mpu6050_ReadMemReg(uint8_t MemAddress,uint8_t* Data);
HAL_StatusTypeDef mpu6050_ReadMemRegs(uint8_t MemAddress,uint8_t* Data,uint8_t length); 
uint8_t mpu6050_Test(void);
#endif


