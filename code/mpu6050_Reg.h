#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H


#define GYRO_CONFIG		0x1B	//陀螺仪配置  可对陀螺仪xyz轴进行自检 并配置陀螺仪量程
#define ACCEL_CONFIG		0x1C	//加速度计配置 可对加速度计xyz轴进行自检 并配置加速度量程
#define FF_THR			0x1D	//设置检测自由落体的检测阈值
#define FF_DUR			0x1E	//设置自由落体事件检测的持续时间计数器的阈值
#define SMPRT_DIV		0x19
#define CONFIG			0x1A

#define ACCEL_XOUT_H	0x3B 	//加速度计x轴高八位
#define ACCEL_XOUT_L	0x3C 	//加速度计x轴低八位
#define ACCEL_YOUT_H	0x3D 
#define ACCEL_YOUT_L	0x3E 
#define ACCEL_ZOUT_H	0x3F 
#define ACCEL_ZOUT_L	0x40 

#define GYRO_XOUT_H 		0x43 	//陀螺仪x轴高八位
#define GYRO_XOUT_L		0x44	//陀螺仪x轴低八位
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46 
#define GYRO_ZOUT_H		0x47 
#define GYRO_ZOUT_L		0x48 


#define PWR_MGMT_1		0x6B	//设置电源模式以及时钟源  上电为睡眠模式		
#define PWR_MGMT_2		0x6C	//寄存器使用户能够设置在仅加速度计低功耗模式下的唤醒频率。该寄存器还允许用户将加速度计和陀螺仪的各个轴置于待机模式。

#define WHO_AM_I		0x75	id
#endif

