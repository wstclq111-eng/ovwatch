#include "st7789.h"


/*==================== 引脚定义 ====================*/
#define LCD_CS_PORT     GPIOA
#define LCD_CS_PIN      GPIO_PIN_4

#define LCD_DC_PORT     GPIOB
#define LCD_DC_PIN      GPIO_PIN_0

#define LCD_RST_PORT    GPIOB
#define LCD_RST_PIN     GPIO_PIN_1

#define LCD_BL_PORT     GPIOB
#define LCD_BL_PIN      GPIO_PIN_2


/*==================== 基本控制 ====================*/
static void LCD_CS_0(void)   { HAL_GPIO_WritePin(LCD_CS_PORT,  LCD_CS_PIN,  GPIO_PIN_RESET); }
static void LCD_CS_1(void)   { HAL_GPIO_WritePin(LCD_CS_PORT,  LCD_CS_PIN,  GPIO_PIN_SET);   }

static void LCD_DC_0(void)   { HAL_GPIO_WritePin(LCD_DC_PORT,  LCD_DC_PIN,  GPIO_PIN_RESET); }	//0未命令
static void LCD_DC_1(void)   { HAL_GPIO_WritePin(LCD_DC_PORT,  LCD_DC_PIN,  GPIO_PIN_SET);   }	//1为数据

static void LCD_RST_0(void)  { HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET); }
static void LCD_RST_1(void)  { HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);   }

static void LCD_BL_0(void)   { HAL_GPIO_WritePin(LCD_BL_PORT,  LCD_BL_PIN,  GPIO_PIN_RESET); }
static void LCD_BL_1(void)   { HAL_GPIO_WritePin(LCD_BL_PORT,  LCD_BL_PIN,  GPIO_PIN_SET);   }	//开启背光

//发送命令
static void ST7789_WriteCommand(uint8_t cmd)
{
	LCD_CS_0();
	LCD_DC_0();
	HAL_SPI_Transmit(&hspi1,&cmd,1,200);
	LCD_CS_1();
}

//发送数据
static void ST7789_WriteData(uint8_t cmd)
{
	LCD_CS_0();
	LCD_DC_1();
	HAL_SPI_Transmit(&hspi1,&cmd,1,200);
	LCD_CS_1();
}

void ST7789_Write_ArrayData(uint8_t *num,uint16_t len)
{
	LCD_CS_0();
	LCD_DC_1();
	HAL_SPI_Transmit(&hspi1,num,len,200);
	LCD_CS_1();
}
//复位ST7789
void SY7789_RESET(void)
{
	LCD_RST_0();
	HAL_Delay(10);
	LCD_RST_1();
	HAL_Delay(120);
	
}

//写入的窗口 将传入的16位数据拆分为8位并写入数据
 void ST7789_WindowWrite(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	uint8_t num[4];
	num[0]=x1>>8;		//将数据右移八位 读取高八位 
	num[1]=x1 & 0xFF;	//读取低八位
	num[2]=x2>>8;		//将数据右移八位 读取高八位 
	num[3]=x2 & 0xFF;	//读取低八位	
	ST7789_WriteCommand(CASET);	//发送命令设置列地址集
	ST7789_Write_ArrayData(num,4);
	
	num[0]=y1>>8;		 
	num[1]=y1 & 0xFF;	
	num[2]=y2>>8;		
	num[3]=y2 & 0xFF;	
	ST7789_WriteCommand(RASET);	
	ST7789_Write_ArrayData(num,4);
	
	ST7789_WriteCommand(RAMWR);//将数据写入 
}

void ST7789_DrawColor(uint16_t color)
{
	uint8_t num[2];
	num[0]=color>>8;
	num[1]=color;
	ST7789_Write_ArrayData(num,2);

}

void ST7789_Init(void)
{
	LCD_BL_1();
	LCD_CS_1();
	LCD_DC_1();
	SY7789_RESET();

	ST7789_WriteCommand(SLEPP_OUT);
	HAL_Delay(5);
	HAL_Delay(120);//多给一点时间 厂家推荐
	ST7789_WriteCommand(0x21); //启动显示反转
	
	ST7789_WriteCommand(COLMOD);
	ST7789_WriteData(0x55);//65k的RGB接口 16位/像素
	
	ST7789_WriteCommand(MADCTL);
	ST7789_WriteData(0x00);
	
	ST7789_WriteCommand(DISON);
	
}


void ST7789_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	
	ST7789_WindowWrite(x,y,x,y);
	ST7789_DrawColor(color);
}

void ST7789_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    int xerr=0,yerr=0;
	int distance_x,distance_y;
    int incx,incy,distance,uRow,uCol;
    uRow=x1;
    uCol=y1;
    distance_x=x2-x1;
    distance_y=y2-y1;
    if(distance_x>0) incx=1;
    else if(distance_x==0) incx=0;
    else {incx=-1;distance_x=-distance_x;}
    if(distance_y>0) incy=1;
    else if(distance_y==0) incy=0;
    else {incy=-1;distance_y=-distance_y;}
    if(distance_x>distance_y) distance=distance_x;
    else distance=distance_y;
    for(int i=0;i<=distance+1;i++)
    {
        ST7789_DrawPoint(uRow,uCol,color);
        xerr+=distance_x;
		yerr+=distance_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
    }
}

void ST7789_DrawRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    ST7789_DrawLine(x1,y1,x2,y1,color);
    ST7789_DrawLine(x1,y1,x1,y2,color);
    ST7789_DrawLine(x2,y1,x2,y2,color);
    ST7789_DrawLine(x1,y2,x2,y2,color);
}

void ST7789_FillColor(uint16_t color)
{
    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = color & 0xFF;
    uint8_t temp[2] = {hi, lo};

    ST7789_WindowWrite(0, 0, ST7789_WIDTH-1, ST7789_HEIGHT-1);   

    for(uint32_t i = 0; i < ST7789_WIDTH * ST7789_HEIGHT; i++)
    {
        ST7789_Write_ArrayData(temp, 2);
    }
}

void ST7789_fillArea(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint32_t width  = x2 - x1 + 1;
    uint32_t height = y2 - y1 + 1;
    uint32_t total  = width * height;

    ST7789_WindowWrite(x1, y1, x2, y2);

    for(uint32_t i = 0; i < total; i++)
    {
        ST7789_DrawColor(color);
	}
}

void ST7789_ShowChar(uint16_t x,uint16_t y,uint8_t ch,uint16_t size,uint16_t backcolor,uint16_t textcolor)
{
	uint8_t page,bit,index;
	uint8_t col,row;
	uint8_t temp;
	const uint8_t *font = NULL;
	switch(size)
	{
		case FONT_SIZE_8X16:
		{
			col=8;
			row=16;
			font=OLED_F8x16[ch-32];
		}
		break;
		case FONT_SIZE_12X24:
		{
			col=12;
			row=24;
			font=OLED_F12x24[ch-32];
		}
		break;
		case FONT_SIZE_16X24:
		{
			col=16;
			row=24;
			font=OLED_F16x24[ch-32];
		} 
		break;
		case NUM_FONT_SIZE_24x48:
		{
			col=24;
			row=48;
			if(ch >= '0' && ch <= '9')	font = NUM_F24x48[ch - '0'];
			else if(ch == ':')			font = NUM_F24x48[10];
		}
		break;
		default:
		{
			return;
		}
	}
	    ST7789_WindowWrite(x, y, x+col-1, y+row-1);
	for(uint8_t i=0;i<row;i++)
	{
		page=i/8;
		bit=i%8;
		for(uint8_t j=0;j<col;j++)
		{
			index=page*col+j;	//计算字模数据的索引
			temp=font[index];
			if(temp&0x01<<bit)		//判断对应位是否为1
			{
				ST7789_DrawColor(textcolor);	//绘制文本颜色
			}
			else
			{
				ST7789_DrawColor(backcolor);	//绘制背景颜色
		}
		}

	}
	
	}
	

void ST7789_ShowString(uint16_t x, uint16_t y, char *str, uint16_t size, uint16_t backcolor,uint16_t textcolor)
{
    uint8_t width;

    if(str == NULL)
    {
        return;
    }

    if(size == FONT_SIZE_8X16)
    {
        width = 8;
    }
    else if(size == FONT_SIZE_12X24)
    {
        width = 12;
    }
	else if(size == FONT_SIZE_16X24)
	{
		width = 16;
	}
    else if(size == NUM_FONT_SIZE_24x48)
	{
		width = 24;
	}
	else
    {
        return;
    }

    while(*str != '\0')
    {
        ST7789_ShowChar(x, y, *str, size,backcolor,textcolor);
        x += width;
        str++;
    }
}
	
void ST7789_ShowRTC(void)
{
	char buff[30];
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	sprintf(buff,"%02d:%02d",sTime.Hours,sTime.Minutes);
	ST7789_ShowString(60,50,buff,NUM_FONT_SIZE_24x48,LCD_BLACK,LCD_WHITE);
	HAL_RTC_GetDate(&hrtc,&sData,RTC_FORMAT_BIN);
	sprintf(buff,"20%02d-%02d-%02d",sData.Year,sData.Month,sData.Date);
	ST7789_ShowString(60,240,buff,FONT_SIZE_12X24,LCD_BLACK,LCD_WHITE);
}

void ST7789_ShowChinesechar(uint16_t x, uint16_t y, char *str,uint8_t size ,uint16_t backcolor,uint16_t textcolor)
{
	uint8_t Row,Col;
	uint8_t temp;
	if(str == NULL)
	{
		return;
	}
	switch (size)
	{
	case CHINESE_SIZE_16X16:
	{
	for(uint8_t i=0;i<chineseF16x16Num;i++)
	{
		if(chineseF16x16[i].Index[0]==str[0]&&chineseF16x16[i].Index[1]==str[1])
		{
			Col=16;
			Row=16;
			ST7789_WindowWrite(x,y,x+Col-1,y+Row-1);
			for(uint8_t m=0;m<Row;m++)
			{
				for(uint8_t n=0;n<Col;n++)
				{
					if(m<=8)
					{
						temp=chineseF16x16[i].Msk[n];
						if(temp&(0x01<<m))
						{
							ST7789_DrawColor(textcolor);
						}
						else
						{
							ST7789_DrawColor(backcolor);
						}
					}
					else if(m<16)
					{
						temp=chineseF16x16[i].Msk[n+Col];
						if(temp&(0x01<<(m-8)))
						{
							ST7789_DrawColor(textcolor);
						}
						else
						{
							ST7789_DrawColor(backcolor);
						}
					}
					
				}
			
			}
		
		
		}
	
	}
		break;
}
	case CHINESE_SIZE_24X24:
	{
	for(uint8_t i=0;i<chineseF24x24Num;i++)
	{
		if(chineseF16x16[i].Index[0]==str[0]&&chineseF16x16[i].Index[1]==str[1])
		{
			Col=24;
			Row=24;
			ST7789_WindowWrite(x,y,x+Col-1,y+Row-1);
			for(uint8_t m=0;m<Row;m++)
			{
				for(uint8_t n=0;n<Col;n++)
				{
					if(m<=8)
					{
						temp=chineseF24x24[i].Msk[n];
						if(temp&(0x01<<m))
						{
							ST7789_DrawColor(textcolor);
						}
						else
						{
							ST7789_DrawColor(backcolor);
						}
					}
					else if(m<=16)
					{
						temp=chineseF24x24[i].Msk[n+Col];
						if(temp&(0x01<<(m-8)))
						{
							ST7789_DrawColor(textcolor);
						}
						else
						{
							ST7789_DrawColor(backcolor);
						}
					}
					else
					{
						temp=chineseF24x24[i].Msk[n+2*Col];
						if(temp&(0x01<<(m-16)))
						{
							ST7789_DrawColor(textcolor);
						}
						else
						{
							ST7789_DrawColor(backcolor);
						}
					}
					
				}
			
			}
		
		
		}
	}
		break;
	}
	
}
}

void ST7789_ShowChinese(uint16_t x, uint16_t y, char *str, uint8_t size, uint16_t backcolor,uint16_t textcolor)
{
	  uint8_t width;
	if(str == NULL)
	{
		return;
	}
	if(size == CHINESE_SIZE_16X16)
	{
		width = 16;
	}
	else if(size == CHINESE_SIZE_24X24)
	{
		width = 24;
	}
	else
	{
		return;
	}
	while(*str != '\0')
	{
		ST7789_ShowChinesechar(x, y, str, size, backcolor,textcolor);
		x += width;
		str += 2;
	}
}







