#ifndef  __ST7789_FONT_H
#define  __ST7789_FONT_H

#include "main.h"

typedef struct
{
    char Index[3];          // 汉字，2字节 + '\0'
    uint8_t Msk[32];        // 16x16 点阵，一个汉字32字节
} HZ16_TypeDef;//汉字结构体

typedef struct
{
    char Index[3];          // 汉字，2字节 + '\0'
    uint8_t Msk[72];        // 24x24 点阵，一个汉字72字节
} HZ24_TypeDef;//汉字结构体

extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F12x24[][36];
extern const uint8_t OLED_F16x24[][48];
extern const uint16_t chineseF16x16Num ;//数量
extern const uint16_t chineseF24x24Num ;
extern const HZ16_TypeDef chineseF16x16[] ;
extern const HZ24_TypeDef chineseF24x24[] ;
extern const  uint8_t NUM_F24x48[][144] ;
#endif
