#ifndef __UI_H
#define __UI_H

#include "main.h"
#include "st7789.h"
#include "MYRTC.h"
#include "touch.h"
#include "stdlib.h"
#include "mpu6050.h"
#include "blood.h"
typedef enum
{
    UI_PAGE_HOME = 1,      
	UI_PAGE_MENU,			
    UI_PAGE_MPU6050,     	 	
    UI_PAGE_DRAW,         
    UI_PAGE_HEALTH,       	 
    UI_PAGE_TOUCH	      	

}UI_Page_t;

typedef enum
{
    UI_EVENT_CLICK= 0,		//单击
    UI_EVENT_SLIDE_LEFT,	//左滑
    UI_EVENT_SLIDE_RIGHT,	//右滑
    UI_EVENT_SLIDE_UP,		//上滑
    UI_EVENT_SLIDE_DOWN,	//下滑
	UI_EVENT_NONE			//无操作
}UI_Event_t;//触摸事件
void ui_DrawRectangleButton(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t Rectanglecolor,uint16_t Fillcolor);
void ui_ShowPage(UI_Page_t page);
void ui_touchhandle(uint16_t x, uint16_t y);
void ui_UpState(void);
uint8_t ui_SlideHandle(CST816T_Touch_t *tp);
void ui_DrawPageHandle(CST816T_Touch_t *tp);



#endif

