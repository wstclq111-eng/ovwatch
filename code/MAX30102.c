#include "MAX30102.h"
uint32_t red_data_buf[32];
uint32_t ir_data_buf[32];


HAL_StatusTypeDef Health_WriteData(uint8_t REG_Address,uint8_t  Data)
{
	return HAL_I2C_Mem_Write(&hi2c3,WRITEADDRESS,REG_Address,I2C_MEMADD_SIZE_8BIT,&Data,1,100);
}
HAL_StatusTypeDef Health_WriteDatas(uint8_t REG_Address,uint8_t  *Data,uint8_t length)
{
	return HAL_I2C_Mem_Write(&hi2c3,WRITEADDRESS,REG_Address,I2C_MEMADD_SIZE_8BIT,Data,length,100);
}
HAL_StatusTypeDef Health_ReadData(uint8_t REG_Address,uint8_t  *Data)
{
	return HAL_I2C_Mem_Read(&hi2c3,READADDRESS,REG_Address,I2C_MEMADD_SIZE_8BIT,Data,1,100);
}
HAL_StatusTypeDef Health_ReadDatas(uint8_t REG_Address,uint8_t  *Data,uint8_t length)
{
	return HAL_I2C_Mem_Read(&hi2c3,READADDRESS,REG_Address,I2C_MEMADD_SIZE_8BIT,Data,length,100);
}
void Health_Init(void)
{
	if(Health_WriteData(REG_MODE_CONFIG,0x40)!=HAL_OK)
	{
		//处理
	}		//软复位
	HAL_Delay(100);
	printf("初始化成功\r\n");
	Health_WriteData(REG_INTR_ENABLE_1,0x80);
	
	Health_WriteData(REG_FIFO_WR_PTR,0x00);
	Health_WriteData(REG_OVF_COUNTER,0x00);
	Health_WriteData(REG_FIFO_RD_PTR,0x00);
	
	Health_WriteData(REG_FIFO_CONFIG,0x5F);	//FIFO配置4个数据平均计算，开启溢出覆盖，设置FIFO空间还剩15触发中断
	Health_WriteData(REG_MODE_CONFIG,0x03);	//spo2模式
	Health_WriteData(REG_SPO2_CONFIG,0x27);	//SpO2 配置：量程 4096nA, 采样率 100Hz, 脉宽 411us (18-bit)
	Health_WriteData(REG_LED1_PA,0x32);
	Health_WriteData(REG_LED2_PA,0x32);
	Health_WriteData(REG_PILOT_PA,0x7f);
	
}

void Health_ReadFIFO(void)
{
	uint8_t  wr_ptr=0;
	uint8_t	rd_ptr=0;
	int8_t num_samples=0;
	

	Health_ReadData(REG_FIFO_WR_PTR,&wr_ptr);
	Health_ReadData(REG_FIFO_RD_PTR,&rd_ptr);
	
	num_samples=wr_ptr - rd_ptr;
	if(num_samples<0)	num_samples=num_samples+32;
	if(num_samples>0)
	{
		uint8_t temp_buf[6]; // 用来接 6 个字节碎片的临时盒子
		for (int i = 0; i < num_samples; i++) 
        {
            // 从 0x07 这个唯一的出口，连续拿 6 个字节
            Health_ReadDatas(REG_FIFO_DATA, temp_buf, 6); 

			red_data_buf[i]=(uint32_t)temp_buf[0]<<16|(uint32_t)temp_buf[1]<<8|(uint32_t)temp_buf[2];
			red_data_buf[i]&=0x03FFFF;//红光数据

			ir_data_buf[i]=(uint32_t)temp_buf[3]<<16|(uint32_t)temp_buf[4]<<8|(uint32_t)temp_buf[5];
			ir_data_buf[i]&=0x03FFFF;//红外光数据
		printf("%u,%u\r\n", red_data_buf[i], ir_data_buf[i]);
        }
	}
	uint8_t temp;
    Health_ReadData(REG_INTR_STATUS_1, &temp);
	
}