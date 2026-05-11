#ifndef __BLOOD_H
#define __BLOOD_H

#include "main.h" // 确保能用到 uint8_t 这种类型
#include "MAX30102.h"

// 对外提供的接口
void Blood_App_Init(void);           // 初始化
void Blood_Collect_Data_ISR(void);   // 放进外部中断里收数据
void Blood_Process_Task(void);       // 放进 while(1) 里算心率

int Get_HeartRate(void);             // 获取心率值
float Get_SpO2(void);                // 获取血氧值

#endif