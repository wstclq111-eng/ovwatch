#ifndef __MAX30102_H
#define __MAX30102_H

#include "main.h"
#include "i2c.h"
#include "stdio.h"
#include "MAX30102_Reg.h"


#define WRITEADDRESS		0xAE
#define READADDRESS		0xAF
HAL_StatusTypeDef Health_WriteData(uint8_t REG_Address,uint8_t  Data);
HAL_StatusTypeDef Health_WriteDatas(uint8_t REG_Address,uint8_t  *Data,uint8_t length);
HAL_StatusTypeDef Health_ReadData(uint8_t REG_Address,uint8_t  *Data);
HAL_StatusTypeDef Health_ReadDatas(uint8_t REG_Address,uint8_t  *Data,uint8_t length);

void Health_Init(void);
void Health_ReadFIFO(void);
#endif

