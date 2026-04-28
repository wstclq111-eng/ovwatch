#include "MYRTC.h"

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sData;


void MYRTC_Init(void)
{
	if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1)!=0xA5A5)
	{
	sTime.Hours=15;
	sTime.Minutes=53;
	sTime.Seconds=30;
	HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	
	sData.Year=26;
	sData.Month=4;
	sData.Date=5;
	sData.WeekDay=7;
	HAL_RTC_SetDate(&hrtc,&sData,RTC_FORMAT_BIN);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0xA5A5);
	}
}
