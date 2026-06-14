#include "stm32f10x.h"                  // Device header
#include "MyRTC.h" 
#include "Key.h" 
#include "OLED.h" 



void Show_SetTime_First_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%2d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%2d",MyRTC_Time[2]);
}

void Show_SetTime_Second_UI(void)
{
	OLED_Printf(0,0,OLED_8X16,"时:%2d",MyRTC_Time[3]);
	OLED_Printf(0,16,OLED_8X16,"分:%2d",MyRTC_Time[4]);
	OLED_Printf(0,32,OLED_8X16,"秒:%2d",MyRTC_Time[5]);
}

extern uint8_t KeyNum;
int set_time_flag=1;

void Change_Set_Time(uint8_t i,uint8_t Flag)
{
	if(Flag==1){MyRTC_Time[i]++;}
	else {MyRTC_Time[i]--;}
	MyRTC_SetTime();
}


int SetYear(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==1)       //数值加1
		{
			Change_Set_Time(0,1);
		}
		else if(KeyNum==2)   //数值减1
		{
			Change_Set_Time(0,0);
		}
		else if(KeyNum==3)   //确认保存
		{
			return 0;
		}
		Show_SetTime_First_UI();
		OLED_ReverseArea(24,16,32,16);
		OLED_Update();
	}		
}

int SetMon(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==1)       //数值加1
		{
			Change_Set_Time(1,1);
			if(MyRTC_Time[1]>12){MyRTC_Time[1]=1;MyRTC_SetTime();}
		}
		else if(KeyNum==2)   //数值减1
		{
			Change_Set_Time(1,0);
			if(MyRTC_Time[1]<1){MyRTC_Time[1]=12;MyRTC_SetTime();}
		}
		else if(KeyNum==3)   //确认保存
		{
			return 0;
		}
		Show_SetTime_First_UI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}		
}
int SetDay(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==1)       //数值加1
		{
			Change_Set_Time(2,1);
			if(MyRTC_Time[2]>31){MyRTC_Time[2]=1;MyRTC_SetTime();}
		}
		else if(KeyNum==2)   //数值减1
		{
			Change_Set_Time(2,0);
			if(MyRTC_Time[2]<=0){MyRTC_Time[2]=31;MyRTC_SetTime();}
		}
		else if(KeyNum==3)   //确认保存
		{
			return 0;
		}
		Show_SetTime_First_UI();
		OLED_ReverseArea(24,48,16,16);
		OLED_Update();
	}		
}
int SetHour(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==1)       //数值加1
		{
			Change_Set_Time(3,1);
			if(MyRTC_Time[3]>=24){MyRTC_Time[3]=0;MyRTC_SetTime();}
		}
		else if(KeyNum==2)   //数值减1
		{
			Change_Set_Time(3,0);
			if(MyRTC_Time[3]<0){MyRTC_Time[3]=23;MyRTC_SetTime();}
		}
		else if(KeyNum==3)   //确认保存
		{
			return 0;
		}
		Show_SetTime_Second_UI();
		OLED_ReverseArea(24,0,16,16);
		OLED_Update();
	}		
}
int SetMin(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==1)       //数值加1
		{
			Change_Set_Time(4,1);
			if(MyRTC_Time[4]>59){MyRTC_Time[4]=0;MyRTC_SetTime();}
		}
		else if(KeyNum==2)   //数值减1
		{
			Change_Set_Time(4,0);
			if(MyRTC_Time[4]<0){MyRTC_Time[4]=59;MyRTC_SetTime();}
		}
		else if(KeyNum==3)   //确认保存
		{
			return 0;
		}
		Show_SetTime_Second_UI();
		OLED_ReverseArea(24,16,16,16);
		OLED_Update();
	}		
}
int SetSec(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==1)       //数值加1
		{
			Change_Set_Time(5,1);
			if(MyRTC_Time[5]>59){MyRTC_Time[5]=0;MyRTC_SetTime();}
		}
		else if(KeyNum==2)   //数值减1
		{
			Change_Set_Time(5,0);
			if(MyRTC_Time[5]<0){MyRTC_Time[5]=59;MyRTC_SetTime();}
		}
		else if(KeyNum==3)   //确认保存
		{
			return 0;
		}
		Show_SetTime_Second_UI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}		
}
int SettingTime(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		uint8_t settime_flag_Temp=0;
		if(KeyNum==1) //上一项
		{
			set_time_flag--;
			if(set_time_flag<=0){set_time_flag=7;}
		}
		else if(KeyNum==2)//下一项
		{
			set_time_flag++;
			if(set_time_flag>=8){set_time_flag=1;}
		}
		else if(KeyNum==3)//确认
		{
			settime_flag_Temp=set_time_flag;
		}
		if(settime_flag_Temp==1){return 0;} //返回上一级
		else if(settime_flag_Temp==2){SetYear();}//设置年
		else if(settime_flag_Temp==3){SetMon();}//设置月
		else if(settime_flag_Temp==4){SetDay();}//设置日
	    else if(settime_flag_Temp==5){SetHour();}//设置时
		else if(settime_flag_Temp==6){SetMin();}//设置分
		else if(settime_flag_Temp==7){SetSec();}//设置秒
		switch(set_time_flag)
		{
			case 1:
				OLED_Clear();
				Show_SetTime_First_UI();
			    OLED_ReverseArea(0,0,16,16);
			    OLED_Update();
			break;
			case 2:
				OLED_Clear();
				Show_SetTime_First_UI();
			    OLED_ReverseArea(0,16,16,16);
			    OLED_Update();
			break;
			case 3:
				OLED_Clear();
				Show_SetTime_First_UI();
			    OLED_ReverseArea(0,32,16,16);
			    OLED_Update();
			break;
			case 4:
				OLED_Clear();
				Show_SetTime_First_UI();
			    OLED_ReverseArea(0,48,16,16);
			    OLED_Update();
			break;
			case 5:
				OLED_Clear();
				Show_SetTime_Second_UI();
			    OLED_ReverseArea(0,0,16,16);
			    OLED_Update();
			break;
			case 6:
				OLED_Clear();
				Show_SetTime_Second_UI();
			    OLED_ReverseArea(0,16,16,16);
			    OLED_Update();
			break;
			case 7:
				OLED_Clear();
				Show_SetTime_Second_UI();
			    OLED_ReverseArea(0,32,16,16);
			    OLED_Update();
			break;
		}
	}
}

