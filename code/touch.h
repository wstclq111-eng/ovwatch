#ifndef __TOUCH_H
#define __TOUCH_H

#include "main.h"

#define CST816T_ADDR        (0x15 << 1)   // 7位地址0x15，传给HAL要左移1位

#define CST816T_REG_FINGER  0x02
#define CST816T_REG_XPOS_H  0x03
#define CST816T_REG_XPOS_L  0x04
#define CST816T_REG_YPOS_H  0x05
#define CST816T_REG_YPOS_L  0x06

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint8_t  pressed;   // 1=有触摸，0=无触摸
} CST816T_Touch_t;

HAL_StatusTypeDef Touch_ReadMemReg(uint8_t Memaddress,uint8_t * Data);
HAL_StatusTypeDef  Touch_ReadMemRegs(uint8_t Memaddress, uint8_t *buf, uint16_t len);
uint8_t Touch_Scan(CST816T_Touch_t *touch);
#endif
