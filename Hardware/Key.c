#include "stm32f10x.h"                  // Device header
#include "Delay.h"


uint8_t Key_Num;

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}


uint8_t KeyGetnum(void)
{
	uint8_t Temp;
	if(Key_Num)
	{
		Temp=Key_Num;
		Key_Num=0;
		return Temp;
	}
	else{
		return 0;
	}
}

int Key_Presstime;
void Key_Tick(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
	{
		Key_Presstime++;
	}
	if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1))
	{
		Key_Presstime=0;
	}
};


uint8_t Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
		return 1;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)
	{
		return 2;
	}
	else if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)&&Key_Presstime>1000)
	{
		return 4;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
	{
		return 3;
	}
	else{return 0;}
}


void Key_Systic(void)
{
	static uint8_t Count;
	static uint8_t PreState,NowState;
	Count++;
	if(Count>=20)
	{
		Count=0;
		PreState=NowState;
		NowState=Key_GetState();
		if(PreState!=0&&NowState ==0)
		{
			Key_Num=PreState;
		}
	}
}

