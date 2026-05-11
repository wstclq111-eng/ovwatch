#include "ui.h"
extern  int16_t AX,AY,AZ;
extern  int16_t GX,GY,GZ;	
    char buf[32];

UI_Page_t ui_face = UI_PAGE_HOME;
uint8_t screen_on = 1;
uint32_t last_active_tick = 0;
/*画一个按键*/
void ui_DrawRectangleButton(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t Rectanglecolor,uint16_t Fillcolor)
{
	ST7789_DrawRectangle(x1,y1,x2,y2,Rectanglecolor);
	ST7789_fillArea(x1+1,y1+1,x2-1,y2-1,Fillcolor);

	
}
/*判断x，y 是否在给定范围内 
在：返回 1
不在：返回0
*/
uint8_t ui_InArea(uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(x>=x1&&x<=x2&&y>=y1&&y<=y2)
	{
		return 1;
	}
	return 0;
	
}

void ui_DrawHome(void)//主界面 
{	
	ST7789_FillColor(LCD_BLACK);
	ST7789_ShowRTC();
	
}

void ui_DrawMenu(void)//菜单界面
{
	ST7789_FillColor(LCD_BLACK);
    ST7789_ShowString(88,20,"MENU",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);

	ui_DrawRectangleButton(20,50 ,220,100,LCD_WHITE,LCD_BLACK);
	ui_DrawRectangleButton(20,105,220,155,LCD_WHITE,LCD_BLACK);
	ui_DrawRectangleButton(20,160,220,210,LCD_WHITE,LCD_BLACK);
	ui_DrawRectangleButton(20,215,220,265,LCD_WHITE,LCD_BLACK);
    ST7789_ShowString(72,64,"HEALTH",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
    ST7789_ShowString(88,119,"DRAW",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
    ST7789_ShowString(40,174,"TOUCH TEST",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
    ST7789_ShowString(64,229,"MPU6050",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
	
}


/*心率血氧界面*/
void ui_DrawHealth(void)
{
	ST7789_FillColor(LCD_BLACK);
	
	ST7789_ShowString(72,30,"HEALTH",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
	ST7789_ShowString(20,120,"HEART:",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
	ST7789_ShowString(20,160,"SPO2:",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);
}

/*画图界面*/
void ui_DrawPicture(void)
{
	ST7789_FillColor(LCD_BLACK);
	ui_DrawRectangleButton(10,240 ,90,270,LCD_WHITE,LCD_BLACK);
	ui_DrawRectangleButton(159,240 ,239,270,LCD_WHITE,LCD_BLACK);
	ST7789_ShowString(26,243,"BACK",FONT_SIZE_12X24,LCD_BLACK,LCD_RED);
	ST7789_ShowString(174,243,"CLEAR",FONT_SIZE_12X24,LCD_BLACK,LCD_GREEN);

}

/*触摸坐标界面*/
void ui_DrawTouchCoord(void)
{
	ST7789_FillColor(LCD_BLACK);
	ST7789_ShowString(40,30,"TOUCH TEST",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);	
	
}

void ui_mpu6050_DrawData(void)
{
	ST7789_FillColor(LCD_BLACK);

	ST7789_ShowString(64,30,"MPU6050",FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);	
	

}
/*画画界面处理*/
void ui_DrawPageHandle(CST816T_Touch_t *tp)
{
    static uint8_t drawing = 0;
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;

    if(tp->pressed == 1)
    {
        // 先判断是不是点到了按钮区域
        if(ui_InArea(tp->x,tp->y,10,240 ,90,270)==1)
        {
            drawing = 0;
            ui_ShowPage(UI_PAGE_MENU);
            return;
        }
        else if(ui_InArea(tp->x,tp->y,159,240 ,239,270)==1)
        {
            drawing = 0;
            ui_DrawPicture();
            return;
        }

        if(drawing == 0)
        {
            drawing = 1;
            last_x = tp->x;
            last_y = tp->y;
            ST7789_DrawPoint(tp->x,tp->y,LCD_WHITE);
        }
        else
        {
            ST7789_DrawLine(last_x,last_y,tp->x,tp->y,LCD_WHITE);
            last_x = tp->x;
            last_y = tp->y;
        }
    }
    else
    {
        drawing = 0;
    }
}

/*对触摸坐标进行判定 并进行相应处理*/
void ui_touchhandle(uint16_t x, uint16_t y)
{
	switch(ui_face)
	{
		case UI_PAGE_HOME:
		{
			

		}	
		break;
		case UI_PAGE_MPU6050:
		{
			
			
		}
			
		
		break;
		
		case UI_PAGE_DRAW:
		{
			
				
		}
		break;
		
		case UI_PAGE_HEALTH:
		{
			
		}
		break;
		
		case UI_PAGE_TOUCH:
		{
			char buff[30];
			
			
				sprintf(buff,"X:%-3d",x);
				ST7789_ShowString(70,80,buff,FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);	
				sprintf(buff,"Y:%-3d",y);
				ST7789_ShowString(70,110,buff,FONT_SIZE_16X24,LCD_BLACK,LCD_WHITE);	

			
		}
		break;
		
		case UI_PAGE_MENU:
		{

				if		(ui_InArea(x,y,20,50 ,220,100)==1) 		ui_ShowPage(UI_PAGE_HEALTH);	
				else if (ui_InArea(x,y,20,105,220,155)==1)		ui_ShowPage(UI_PAGE_DRAW);	
				else if (ui_InArea(x,y,20,160,220,210)==1) 		ui_ShowPage(UI_PAGE_TOUCH);		
				else if (ui_InArea(x,y,20,215,220,265)==1) 		ui_ShowPage(UI_PAGE_MPU6050);		
		}
		default :
		break;
	}
}


/*对滑动进行判断*/
uint8_t ui_SlideHandle(CST816T_Touch_t *tp)
{
	static uint8_t touch_down = 0;
	static uint16_t start_x=0;
	static uint16_t start_y=0;
	static uint16_t end_x=0;
	static uint16_t end_y=0;
		int16_t dx,dy;
	
	if(tp->pressed==1&&touch_down==0)
	{
		touch_down=1;
		start_x=tp->x;
		start_y=tp->y;
		
	}
	else if(tp->pressed==0&&touch_down==1)
	{
		touch_down=0;
		end_x=tp->x;
		end_y=tp->y;
		
		dx=end_x-start_x;
		dy=end_y-start_y;
	
		if(abs(dx)<30&&abs(dy)<30)
		{
			return UI_EVENT_CLICK;
		}
	if(abs(dx)>abs(dy))
	{
		if(dx>0)
		{
			//右滑
			printf("右滑一次");		
			return UI_EVENT_SLIDE_RIGHT;
		}
		else
		{
			//左滑
			printf("左滑一次");
			return UI_EVENT_SLIDE_LEFT;

		}
	}
	else if(abs(dy)>abs(dx))
	{
		if(dy>0)
		{
			//下滑
			printf("下滑一次");
			return UI_EVENT_SLIDE_DOWN;
		}
		else
		{
			//上滑
			printf("上滑一次");
			return UI_EVENT_SLIDE_UP;
		}
	}
	
	
	end_x=start_x;
	end_y=start_y;
	
	}

		return UI_EVENT_NONE;
}

/*界面切换*/
void ui_ShowPage(UI_Page_t page)
{
	ui_face=page;
	switch(ui_face)
	{
		case UI_PAGE_HOME:	//首页 显示实时时间
		{
		ui_DrawHome();
		}
		break;
		
		case UI_PAGE_MPU6050:
		{
		ui_mpu6050_DrawData();
			
		}
		break;
		
		case UI_PAGE_DRAW:
		{
		ui_DrawPicture();
		}
		break;
		
		case UI_PAGE_HEALTH:
		{
		ui_DrawHealth();
		}
		break;
		
		case UI_PAGE_TOUCH:
		{
		ui_DrawTouchCoord();
		}
		break;
		
		case UI_PAGE_MENU:
		{
			ui_DrawMenu();
		}
		default :
		break;

	}
}


/*界面更新*/
void ui_UpState(void)
{
	switch(ui_face)
	{
		case UI_PAGE_HOME:	
		{
			ST7789_ShowRTC();
		}
		break;
		
		case UI_PAGE_MPU6050:	
		{
		mpu6050_GetAccelData(&AX,&AY,&AZ);
		mpu6050_GetGyroData(&GX,&GY,&GZ);

		sprintf(buf,"AX %-6d",AX);
		ST7789_ShowString(30,150,buf,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
		sprintf(buf,"AY %-6d",AY);
		ST7789_ShowString(30,180,buf,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
		sprintf(buf,"AZ %-6d",AZ);
		ST7789_ShowString(30,210,buf,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
		sprintf(buf,"GX %-6d",GX);
		ST7789_ShowString(150,150,buf,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
		sprintf(buf,"GY %-6d",GY);
		ST7789_ShowString(150,180,buf,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
		sprintf(buf,"GZ %-6d",GZ);
		ST7789_ShowString(150,210,buf,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
			
			
			
			
		}
		break;
		
		case UI_PAGE_DRAW:
		{
			
		}
		break;
		
		case UI_PAGE_HEALTH:
		{
			int hr = Get_HeartRate();
			float spo2 = Get_SpO2();
			
			// 2. 格式化并显示心率
			// 注意：我们在 "%3d BPM   " 后面多加了几个空格，是为了覆盖掉之前留下的残影
			if(hr > 0) {
				sprintf(buf, "%3d BPM   ", hr);
			} else {
				sprintf(buf, "--- BPM   "); // 没测出心率时，显示横线
			}
			// 你的 "HEART:" 坐标是 (50, 120)，字母宽16，大概占了100像素。
			// 所以数值的 X 坐标从 150 开始刚好合适
			ST7789_ShowString(120, 120, buf, FONT_SIZE_16X24, LCD_BLACK, LCD_GREEN); // 用绿色显示心率

			// 3. 格式化并显示血氧
			if(spo2 > 0.0) {
				sprintf(buf, "%5.2f %%  ", spo2);
			} else {
				sprintf(buf, "--.-- %%  ");
			}
			ST7789_ShowString(120, 160, buf, FONT_SIZE_16X24, LCD_BLACK, LCD_CYAN); // 用青色显示血氧
		}
	
		break;
		
		case UI_PAGE_TOUCH:
		{
			
		}
		break;
		
		case UI_PAGE_MENU:
		{
			
		}
		break;
		
	default :
	break;

	}
}
void UI_ScreenOn(void)
{
   HAL_GPIO_WritePin(GPIOB,  GPIO_PIN_2,  GPIO_PIN_SET); //开启背光
    screen_on = 1;
    last_active_tick = HAL_GetTick();


}

void UI_ScreenOff(void)
{
    HAL_GPIO_WritePin(GPIOB,  GPIO_PIN_2,  GPIO_PIN_RESET);//关闭背光
    screen_on = 0;
}
void UI_ActiveUpdate(void)
{
    last_active_tick = HAL_GetTick();
}