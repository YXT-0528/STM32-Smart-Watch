#include "stm32f10x.h"                  // Device header
#include "time.h"


int16_t MyRTC_Time[]={2026,4,20,10,43,66};
void MyRTC_SetTime(void);


void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	if(BKP_ReadBackupRegister(BKP_DR1)!= 0xFFFF)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();	
		
		
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1,0XFFFF);
	}
	else
	{
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();	
	}
}



void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm MyRTC_Date;
	MyRTC_Date.tm_year=MyRTC_Time[0]-1900;
	MyRTC_Date.tm_mon=MyRTC_Time[1]-1;
	MyRTC_Date.tm_mday=MyRTC_Time[2];
	MyRTC_Date.tm_hour=MyRTC_Time[3];
	MyRTC_Date.tm_min=MyRTC_Time[4];
	MyRTC_Date.tm_sec=MyRTC_Time[5];

	
	time_cnt=mktime(&MyRTC_Date)-8*60*60;
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();
}

void MyRTC_GetTime(void)
{
	time_t time_cnt;
	struct tm MyRTC_Date;
	time_cnt=RTC_GetCounter()+8*60*60;
	MyRTC_Date=*localtime(&time_cnt);
	MyRTC_Time[0]=MyRTC_Date.tm_year+1900;
	MyRTC_Time[1]=MyRTC_Date.tm_mon+1;
	MyRTC_Time[2]=MyRTC_Date.tm_mday;
	MyRTC_Time[3]=MyRTC_Date.tm_hour;
	MyRTC_Time[4]=MyRTC_Date.tm_min;
	MyRTC_Time[5]=MyRTC_Date.tm_sec;
}
