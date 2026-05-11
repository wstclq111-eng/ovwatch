#ifndef __ST7789_H
#define __ST7789_H

#include "main.h"
#include "spi.h"
#include "ST7789_Font.h"
#include "MYRTC.h"
#include <stdio.h>
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sData;

#define ST7789_WIDTH   240
#define ST7789_HEIGHT  280

#define SLEPP_IN    0x10
#define SLEPP_OUT    0x11
#define INVOFF      0x20
#define INVON       0x21
#define DISOFF       0x28
#define DISON       0x29
#define CASET       0x2A
#define RASET       0x2B
#define RAMWR		0x2C
#define COLMOD      0x3A	//设置像素接口
#define MADCTL		0x36
#define WRDISBV    0x51
#define RDDISBV    0x52	//读显示亮度

#define FONT_SIZE_8X16    16
#define FONT_SIZE_12X24   36
#define FONT_SIZE_16X24   48
#define NUM_FONT_SIZE_24x48       144

#define CHINESE_SIZE_16X16   16
#define CHINESE_SIZE_24X24   72


#define LCD_WHITE   0xFFFF
#define LCD_BLACK   0x0000
#define LCD_RED     0xF800
#define LCD_GREEN   0x07E0
#define LCD_BLUE    0x001F
#define LCD_YELLOW  0xFFE0
#define LCD_CYAN    0x07FF
#define LCD_MAGENTA 0xF81F

void ST7789_Write_ArrayData(uint8_t *num,uint16_t len);
void ST7789_Write_ArrayData(uint8_t *num,uint16_t len);
void SY7789_RESET(void);
 void ST7789_WindowWrite(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ST7789_DrawColor(uint16_t color);
void ST7789_Init(void);

void ST7789_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void ST7789_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ST7789_DrawRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ST7789_FillColor(uint16_t color);
void ST7789_fillArea(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ST7789_ShowChar(uint16_t x,uint16_t y,uint8_t ch,uint16_t size,uint16_t backcolor,uint16_t textcolor);
void ST7789_ShowString(uint16_t x, uint16_t y, char *str, uint16_t size, uint16_t backcolor,uint16_t textcolor);
void ST7789_ShowRTC(void);
void ST7789_ShowChinesechar(uint16_t x, uint16_t y, char *str,uint8_t size ,uint16_t backcolor,uint16_t textcolor);
void ST7789_ShowChinese(uint16_t x, uint16_t y, char *str, uint8_t size, uint16_t backcolor,uint16_t textcolor);
#endif
