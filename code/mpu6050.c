#include "mpu6050.h"

void mpu6050_WriteMemReg(uint8_t MemAddress,uint8_t Data)
{
	HAL_I2C_Mem_Write(&hi2c2,mpu6050_Address,MemAddress,I2C_MEMADD_SIZE_8BIT,&Data,1,100);
}

void mpu6050_WriteMemRegs(uint8_t MemAddress,uint8_t *Data,uint8_t length)
{
	HAL_I2C_Mem_Write(&hi2c2,mpu6050_Address,MemAddress,I2C_MEMADD_SIZE_8BIT,Data,length,100);
}

HAL_StatusTypeDef mpu6050_ReadMemReg(uint8_t MemAddress,uint8_t* Data)
{
	return HAL_I2C_Mem_Read(&hi2c2,mpu6050_Address,MemAddress,I2C_MEMADD_SIZE_8BIT,Data,1,100);
}

HAL_StatusTypeDef mpu6050_ReadMemRegs(uint8_t MemAddress,uint8_t* Data,uint8_t length)
{
	return HAL_I2C_Mem_Read(&hi2c2,mpu6050_Address,MemAddress,I2C_MEMADD_SIZE_8BIT,Data,length,100);
}


void mpu6050_Init(void)
{
    mpu6050_WriteMemReg(PWR_MGMT_1, 0x01);   // X轴陀螺作时钟，退出睡眠
    mpu6050_WriteMemReg(PWR_MGMT_2, 0x00);   // 所有轴开启
    mpu6050_WriteMemReg(SMPRT_DIV, 0x09);   // 采样率约100Hz
    mpu6050_WriteMemReg(CONFIG, 0x06);       // DLPF低通滤波
    mpu6050_WriteMemReg(GYRO_CONFIG, 0x18);  // ±2000°/s
    mpu6050_WriteMemReg(ACCEL_CONFIG, 0x00); // ±2g

    HAL_Delay(100); // 给传感器稳定时间
}

void mpu6050_GetAccelData(int16_t* AX,int16_t* AY,int16_t* AZ)
{
	uint8_t Data[6];
	mpu6050_ReadMemRegs(ACCEL_XOUT_H,Data,6);
	*AX = (int16_t)(Data[0]<<8|Data[1]);
	*AY = (int16_t)(Data[2]<<8|Data[3]);
	*AZ = (int16_t)(Data[4]<<8|Data[5]);
}

void mpu6050_GetGyroData(int16_t* GX,int16_t* GY,int16_t* GZ)
{
	uint8_t Data[6];
	mpu6050_ReadMemRegs(GYRO_XOUT_H,Data,6);
	*GX = (int16_t)(Data[0]<<8|Data[1]);
	*GY = (int16_t)(Data[2]<<8|Data[3]);
	*GZ = (int16_t)(Data[4]<<8|Data[5]);
}

uint8_t mpu6050_Test(void)
{
	static int16_t last_AX;
	static int16_t last_AY;
	static int16_t last_AZ;
	int16_t AX,AY,AZ;
	int16_t dAX,dAY,dAZ;
	mpu6050_GetAccelData(&AX,&AY,&AZ);
	dAX = AX - last_AX;
	dAY = AY - last_AY;
	dAZ = AZ - last_AZ;
	last_AX = AX;
	last_AY = AY;
	last_AZ = AZ;
	if((dAX > 5000 || dAX < -5000) || (dAY > 5000 || dAY < -5000) || (dAZ > 5000 || dAZ < -5000))
	{
		return 1; // 检测到明显的加速度变化，可能是移动或震动
	}
	return 0; // 没有检测到明显的加速度变化
}
