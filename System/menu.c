#include "stm32f10x.h"                  // Device header
#include "MyRTC.h" 
#include "OLED.h" 
#include "Key.h" 
#include "Led.h" 
#include "SetTime.h" 
#include "menu.h" 
#include "MPU6050.h" 
#include "Delay.h" 
#include "dino.h"
#include <math.h>
#include "AD.h"


uint8_t KeyNum;
/*-----------------------------------------菜单UI---------------------------------*/

void menu_init(void)
{
	MyRTC_Init();
	Key_Init();
	Led_Init();
	MPU6050_Init();
	AD_Init();
}

uint16_t ADVal;
float VBAT;
int Battery_Capacity;
void Show_Battery(void)
{
	int sum;
	for(uint16_t i=0;i<3000;i++)
	{
		ADVal=AD_GetValue();
		sum+=ADVal;
	}
   	ADVal=sum/3000;
	VBAT=(float)ADVal/4095*3.3;
	Battery_Capacity=(ADVal-3276)*100/819;
	if(Battery_Capacity<0)Battery_Capacity=0;
//	OLED_ShowNum(64,0,ADVal,4,OLED_6X8);
//	OLED_Printf(64,9,OLED_6X8,"VBAT:%.2f",VBAT);
	OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8);
	OLED_ShowChar(103,4,'%',OLED_6X8);
	if(Battery_Capacity==100)
	{
		OLED_ShowImage(110,0,16,16,Battery);
	}
	else if(Battery_Capacity>10&&Battery_Capacity<100)
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea(112+Battery_Capacity/10,5,(10-Battery_Capacity/10),6);
		OLED_ClearArea(85,4,6,8);
	}
	else{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea(112,5,10,6);
		OLED_ClearArea(85,4,12,8);
	}
}


void ShowClock_UI(void)
{
	Show_Battery();
	MyRTC_GetTime();
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_10X20,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
}

int clkFlag=1;

int First_Page_Clock(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		
		if(KeyNum==1) //上一项
		{
			clkFlag--;
			if(clkFlag<=0){clkFlag=2;}
		}
		else if(KeyNum==2)//下一项
		{
			clkFlag++;
			if(clkFlag>=3){clkFlag=1;}
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			return clkFlag;
		}
		else if(KeyNum==4)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	        GPIO_SetBits(GPIOB,GPIO_Pin_12);
		}
		
		switch(clkFlag)
		{
			case 1:
				ShowClock_UI();
			    OLED_ReverseArea(0,48,32,16);
			    OLED_Update();
			break;
			case 2:
				ShowClock_UI();
			    OLED_ReverseArea(96,48,32,16);
			    OLED_Update();
			break;
		}
	}
}


/*-----------------------------------------设置界面UI---------------------------------*/

void Show_SettingPage_UI(void)
{
	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}

int setflag=1;
int SettingPage(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		uint8_t setflagTemp=0;
		if(KeyNum==1) //上一项
		{
			setflag--;
			if(setflag<=0){setflag=2;}
		}
		else if(KeyNum==2)//下一项
		{
			setflag++;
			if(setflag>=3){setflag=1;}
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			setflagTemp=setflag;
		}
		if(setflagTemp==1){return 0;}
		else if(setflagTemp==2){SettingTime();}
		switch(setflag)
		{
			case 1:
				Show_SettingPage_UI();
			    OLED_ReverseArea(0,0,16,16);
			    OLED_Update();
			break;
			case 2:
				Show_SettingPage_UI();
			    OLED_ReverseArea(0,16,96,16);
			    OLED_Update();
			break;
		}
	}
}
/*-----------------------------------------滑动菜单UI---------------------------------*/
uint8_t PreSelection;//上次图标选项
uint8_t TargetSelection;//目标图标选项
uint8_t move_flag;//移动标志位，1：开始，0：停止
uint8_t Speed=4;   //移动速度
uint8_t x_Pre=48;   //上次图标的x轴坐标



void Slip_Animation(void)
{
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);
	
	if(PreSelection<TargetSelection)
	{
		x_Pre-=Speed;
		if(x_Pre==0)
		{
			PreSelection++;
			move_flag=0;
			x_Pre=48;
		}
	}
	
	if(PreSelection>TargetSelection)
	{
		x_Pre+=Speed;
		if(x_Pre==96)
		{
			PreSelection--;
			move_flag=0;
			x_Pre=48;
		}
	}
	
	if(PreSelection>=1)
	{
		OLED_ShowImage(x_Pre-48,16,32,32,Menu_Graph[PreSelection-1]);
	}
	
	if(PreSelection>=2)
	{
		OLED_ShowImage(x_Pre-96,16,32,32,Menu_Graph[PreSelection-2]);
	}
	OLED_ShowImage(x_Pre,16,32,32,Menu_Graph[PreSelection]);
	OLED_ShowImage(x_Pre+48,16,32,32,Menu_Graph[PreSelection+1]);
	OLED_ShowImage(x_Pre+96,16,32,32,Menu_Graph[PreSelection+2]);
	
	OLED_Update();
}

void Set_Flag(uint8_t move_flag,uint8_t pre_selection,uint8_t target_selection)
{
	if(move_flag==1)
	{
		PreSelection=pre_selection;
		TargetSelection=target_selection;
	}
	Slip_Animation();
}

void Menu_To_Function(void)
{
	for(uint8_t i=0;i<=6;i++)
	{
		OLED_Clear();
		if(PreSelection>=1)
		{
			OLED_ShowImage(x_Pre-48,16+8*i,32,32,Menu_Graph[PreSelection-1]);
		}
		
		OLED_ShowImage(x_Pre,16+8*i,32,32,Menu_Graph[PreSelection]);
		OLED_ShowImage(x_Pre+48,16+8*i,32,32,Menu_Graph[PreSelection+1]);
		
		OLED_Update();
	}
}



int menu_slip_flag=1;
int menu(void)
{
	move_flag=1;
	uint8_t Direct_flag=2;  //置1：上一项，置2：下一项
	while(1)
	{
		KeyNum=KeyGetnum();
		uint8_t menuflagTemp=0;
		if(KeyNum==1) //上一项
		{
			Direct_flag=1;
			move_flag=1;
			menu_slip_flag--;
			if(menu_slip_flag<=0){menu_slip_flag=7;}
		}
		else if(KeyNum==2)//下一项
		{
			Direct_flag=2;
			move_flag=1;
			menu_slip_flag++;
			if(menu_slip_flag>=8){menu_slip_flag=1;}
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menuflagTemp=menu_slip_flag;
		}
		if(menuflagTemp==1){return 0;}
		else if(menuflagTemp==2){Menu_To_Function();Stop_Watch();}
		else if(menuflagTemp==3){Menu_To_Function();LED();}
		else if(menuflagTemp==4){Menu_To_Function();MPU6050();}
		else if(menuflagTemp==5){Menu_To_Function();Game();}
		else if(menuflagTemp==6){Menu_To_Function();Emoji();}
		else if(menuflagTemp==7){Menu_To_Function();Gradienter();}
        if(menu_slip_flag==1)
		{
			if(Direct_flag==1)Set_Flag(move_flag,1,0);
			else if(Direct_flag==2)Set_Flag(move_flag,0,0);
		}
		else{
			if(Direct_flag==1)Set_Flag(move_flag,menu_slip_flag,menu_slip_flag-1);
			else if(Direct_flag==2)Set_Flag(move_flag,menu_slip_flag-2,menu_slip_flag-1);
		}
	}
}

/*-----------------------------------------秒表---------------------------------*/
uint8_t hour,min,sec;
uint8_t Stopwatch_timflag;


void Show_Stopwatch(void)
{
		OLED_ShowImage(0,0,16,16,Return);
		OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",hour,min,sec);
		OLED_ShowString(8,44,"开始",OLED_8X16);
		OLED_ShowString(48,44,"停止",OLED_8X16);
		OLED_ShowString(88,44,"重置",OLED_8X16);
}

void Stop_Tick(void)
{
	static uint16_t Count=0;
	Count++;
	if(Count>=1000)
	{
		Count=0;
		if(Stopwatch_timflag==1)
		{
			sec++;
			if(sec>=60)
			{
				sec=0;
				min++;
				if(min>=60)
				{
					min=0;
					hour++;
					if(hour==99)hour=0;
				}
			}
		}
	}
	
}


uint8_t stopwatch_flag=1;
int  Stop_Watch(void)
{
	uint8_t stopwatch_flag_Temp=0;
	while(1)
	{
		KeyNum=KeyGetnum();
		
		if(KeyNum==1) //上一项
		{
			stopwatch_flag--;
			if(stopwatch_flag<=0){stopwatch_flag=4;}
		}
		else if(KeyNum==2)//下一项
		{
			stopwatch_flag++;
			if(stopwatch_flag>=5){stopwatch_flag=1;}
		}
		else if(KeyNum==3)//确认
		{
			stopwatch_flag_Temp=stopwatch_flag;
		}
		if(stopwatch_flag_Temp==1){return 0;}
		switch(stopwatch_flag)
		{
			case 1:
				Show_Stopwatch();
			    OLED_ReverseArea(0,0,16,16);
			    OLED_Update();
			break;
			case 2:
				Show_Stopwatch();
			    if(stopwatch_flag_Temp==2)
			    Stopwatch_timflag=1;
			    OLED_ReverseArea(8,44,32,16);
			    OLED_Update();
			break;
			case 3:
				Show_Stopwatch();
			if(stopwatch_flag_Temp==3)
			    Stopwatch_timflag=0;
			    OLED_ReverseArea(48,44,32,16);
			    OLED_Update();
			break;
			case 4:
				Show_Stopwatch();
			if(stopwatch_flag_Temp==4)
			{ Stopwatch_timflag=0;
			    hour=min=sec=0;}
			    OLED_ReverseArea(88,44,32,16);
			    OLED_Update();
			break;
		}
	}
}

/*-----------------------------------------手电筒---------------------------------*/
void LED_ShowUI(void)
{
	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(20,20,"ON",OLED_10X20);
	OLED_ShowString(72,20,"OFF",OLED_10X20);
}

uint8_t LED_flag=1;
int LED(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		uint8_t LED_flag_Temp=0;
		if(KeyNum==1) //上一项
		{
			LED_flag--;
			if(LED_flag<=0){LED_flag=3;}
		}
		else if(KeyNum==2)//下一项
		{
			LED_flag++;
			if(LED_flag>=4){LED_flag=1;}
		}
		else if(KeyNum==3)//确认
		{
			LED_flag_Temp=LED_flag;
		}
		if(LED_flag_Temp==1){return 0;}
		switch(LED_flag)
		{
			case 1:
				LED_ShowUI();
			    OLED_ReverseArea(0,0,16,16);
			    OLED_Update();
			break;
			case 2:
				LED_ShowUI();
			    if(LED_flag_Temp==2)
			    Led_ON();
			    OLED_ReverseArea(20,20,36,24);
			    OLED_Update();
			break;
			case 3:
				LED_ShowUI();
			    if(LED_flag_Temp==3)
					Led_OFF();
			    OLED_ReverseArea(72,20,24,24);
			    OLED_Update();
			break;
		}
	}
}

/*-----------------------------------------MPU6050---------------------------------*/
int16_t ax,ay,az,gx,gy,gz;        //MPU6050获取数据存放
float yaw_g,pitch_g,roll_g;       //陀螺仪获取欧拉角
float pitch_a,roll_a;       //加速度计获取欧拉角
float Roll,Pitch,Yaw;              //互补滤波后欧拉角
float Delta_t=0.005;                  //采样周期
float a=0.4;                        //互补滤波器系数
double pi=3.14159265;

void MPU6050_Caulation(void)
{
	Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	//通过陀螺仪计算欧拉角
	roll_g=Roll+(float)gx*Delta_t;
	pitch_g=Pitch+(float)gy*Delta_t;
	yaw_g=Yaw=+(float)gz*Delta_t;
	
	//通过加速度计计算欧拉角
	pitch_a=atan2((-1)*ax,az)*180/pi;
	roll_a=atan2(ay,az)*180/pi;
	
	//互补滤波后欧拉角
	Roll=roll_g*a+(1-a)*roll_a;
	Pitch=pitch_g*a+(1-a)*pitch_a;
	Yaw=yaw_g*a;
}


void Show_MPU6050_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16 ,"Roll: %.2f",Roll);
	OLED_Printf(0,32,OLED_8X16 ,"Pitch:%.2f",Pitch);
	OLED_Printf(0,48,OLED_8X16 ,"Yaw:  %.2f",Yaw);
}

int MPU6050(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		MPU6050_Caulation();
		Show_MPU6050_UI();
		OLED_ReverseArea(0,0,16,16);
		OLED_Update();
	}
}
	
/*-----------------------------------------游戏---------------------------------*/


void Show_Game_UI(void)
{
	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"谷歌小恐龙",OLED_8X16);
}

uint8_t game_flag=1;

int Game(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		uint8_t game_flag_Temp=0;
		if(KeyNum==1) //上一项
		{
			game_flag--;
			if(game_flag<=0){game_flag=2;}
		}
		else if(KeyNum==2)//下一项
		{
			game_flag++;
			if(game_flag>=3){game_flag=1;}
		}
		else if(KeyNum==3)//确认
		{
			game_flag_Temp=game_flag;
		}
		if(game_flag_Temp==1){return 0;}
		else if(game_flag_Temp==2){dino_game_Init();DinoGame_Animation();}
		switch(game_flag)
		{
			case 1:
				Show_Game_UI();
			    OLED_ReverseArea(0,0,16,16);
			    OLED_Update();
			break;
			case 2:
				Show_Game_UI();
			    OLED_ReverseArea(0,16,80,16);
			    OLED_Update();
			break;
		}
	}
}

/*-----------------------------------------表情包---------------------------------*/

void Show_Emoji_UI(void)
{
	for(uint8_t i=0;i<3;i++)
	{
		OLED_Clear();
		OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);
		OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);
		OLED_DrawEllipse(40,32,6,6-i,1);//左眼
		OLED_DrawEllipse(88,32,6,6-i,1);//右眼
		OLED_ShowImage(54,40,20,20,mouth);
		OLED_Update();
		Delay_ms(100);
	}
	
	for(uint8_t i=0;i<3;i++)
	{
		OLED_Clear();
		OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);
		OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);
		OLED_DrawEllipse(40,32,6,4+i,1);//左眼
		OLED_DrawEllipse(88,32,6,4+i,1);//右眼
		OLED_ShowImage(54,40,20,20,mouth);
		OLED_Update();
		Delay_ms(100);
	}
	Delay_ms(500);
}

int Emoji(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		Show_Emoji_UI();
	}
}

/*-----------------------------------------水平仪---------------------------------*/
void Show_Gradienter_UI(void)
{
	MPU6050_Caulation();
	OLED_DrawCircle(64,32,30,0);
	OLED_DrawCircle(64-Roll,32+Pitch,4,1);
}

int Gradienter(void)
{
	while(1)
	{
		KeyNum=KeyGetnum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		Show_Gradienter_UI();
		OLED_Update();
	}
}
