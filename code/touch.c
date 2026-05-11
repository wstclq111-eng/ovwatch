#include "touch.h"
#include "i2c.h"


HAL_StatusTypeDef Touch_ReadMemReg(uint8_t Memaddress,uint8_t * Data)
{
	return HAL_I2C_Mem_Read(&hi2c1,CST816T_ADDR,Memaddress,I2C_MEMADD_SIZE_8BIT,Data,1,100);
	
}
HAL_StatusTypeDef  Touch_ReadMemRegs(uint8_t Memaddress, uint8_t *buf, uint16_t len)
{
     return HAL_I2C_Mem_Read(&hi2c1,CST816T_ADDR, Memaddress, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
}

uint8_t Touch_Scan(CST816T_Touch_t *touch)
{
	uint8_t fingers = 0;
    uint8_t buf[4];

    if(touch == NULL)
    {
        return 0;
    }
	
	if(Touch_ReadMemReg(CST816T_REG_FINGER,&fingers)!=HAL_OK)
	{
		touch->pressed = 0;
		 return 0;
	}
	if((fingers%0xFF)==0)
	{
		touch->pressed = 0;
		return 0;
	}
    if(Touch_ReadMemRegs(CST816T_REG_XPOS_H, buf, 4) != HAL_OK)
    {
        touch->pressed = 0;
        touch->x = 0;
        touch->y = 0;
        return 0;
    }

    touch->x = ((uint16_t)(buf[0] & 0x0F) << 8) | buf[1];
    touch->y = ((uint16_t)(buf[2] & 0x0F) << 8) | buf[3];
	
    touch->pressed = 1;

    return 1;
}


