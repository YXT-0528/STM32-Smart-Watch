#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include <stdlib.h>
#include <math.h>
#include "Delay.h"


int Score;
struct Object_Position{
	uint8_t minX,minY,maxX,maxY;
};
void Show_Score(void)
{
	OLED_ShowNum(98,0,Score,5,OLED_6X8);
}
uint16_t Ground_Pos;

void Show_Ground(void)
{
	if(Ground_Pos<128)
	{
		for(uint8_t i=0;i<128;i++)
		{
			OLED_DisplayBuf[7][i]=Ground[i+Ground_Pos];
		}
	}
	else
	{
		for(uint8_t i=0;i<255-Ground_Pos;i++)
		{
			OLED_DisplayBuf[7][i]=Ground[i+Ground_Pos];
		}
		for(uint8_t i=255-Ground_Pos;i<128;i++)
		{
			OLED_DisplayBuf[7][i]=Ground[i-(255-Ground_Pos)];
		}
	}
	
}



uint8_t barrier_flag;
uint8_t Barrier_Pos;
struct Object_Position barrier;
void Show_Barrier(void)
{
	if(Barrier_Pos>=143)
	{
		barrier_flag=rand()%3;
	}
	OLED_ShowImage(127-Barrier_Pos,44,16,18,Barrier[barrier_flag]);
	barrier.maxX=143-Barrier_Pos;
	barrier.minX=127-Barrier_Pos;
	barrier.maxY=62;
	barrier.minY=44;
}


uint8_t Cloud_Pos;
void Show_Cloud(void)
{
	OLED_ShowImage(127-Cloud_Pos,9,16,8,Cloud);
}



extern uint8_t KeyNum;
uint8_t Dino_jump_flag;//0:表示奔跑，1:表示跳跃
uint16_t jump_t;
uint8_t Jump_Pos;
extern double pi;
struct Object_Position dino;
void Show_Dion(void)
{
	KeyNum=KeyGetnum();
	if(KeyNum==1)Dino_jump_flag=1;
	Jump_Pos=28*sin((float)(pi*jump_t/1000));
	if(Dino_jump_flag==0)
	{
		if(Cloud_Pos%2==0)OLED_ShowImage(0,44,16,18,Dino[0]);		
		else OLED_ShowImage(0,44,16,18,Dino[1]);
	}
	else
	{
		OLED_ShowImage(0,44-Jump_Pos,16,18,Dino[2]);
	}
	dino.minX=0;
	dino.maxX=16;
	dino.minY=44-Jump_Pos;
	dino.maxY=62-Jump_Pos;
}


int isColliding(struct Object_Position *a,struct Object_Position *b)
{
	if(a->maxX>b->minX && a->minX<b->maxX && a->maxY>b->minY && a->minY<b->maxY)
	{
		OLED_Clear();
		OLED_ShowString(28,24,"Game Over",OLED_8X16);
		OLED_Update();
		Delay_s(1);
		OLED_Clear();
		OLED_Update();
		return 1;
	}
	else{return 0;}
}




int DinoGame_Animation(void)
{
	while(1)
	{
		int8_t return_flag;
		OLED_Clear();
		Show_Score();
		Show_Ground();
		Show_Cloud();
		Show_Barrier();
		Show_Dion();
		OLED_Update();
		return_flag=isColliding(&dino,&barrier);
		if(return_flag==1)
		{
			return 0;
		}
  }
}






void Score_Tick(void)
{
	static uint16_t Score_Count,Ground_Count,Cloud_Count;
	Score_Count++;
	Ground_Count++;
	Cloud_Count++;
	if(Score_Count>=1000)
	{
		Score_Count=0;
		Score++;
	}
	if(Ground_Count>=20)
	{
		Ground_Count=0;
		Ground_Pos++;
		Barrier_Pos++;
		if(Ground_Pos>=256)Ground_Pos=0;
		if(Barrier_Pos>=144)Barrier_Pos=0;
	}
	if(Cloud_Count>=50)
	{
		Cloud_Count=0;
		Cloud_Pos++;
		if(Cloud_Pos>=210)
		{
			Cloud_Pos=0;
		}
	}
	if(Dino_jump_flag==1)
	{
		jump_t++;
		if(jump_t>=1000)
		{
			jump_t=0;
			Dino_jump_flag=0;
		}
	}
}



void dino_game_Init(void)
{
	Score=Ground_Pos=Cloud_Pos=Jump_Pos=Barrier_Pos=0;
}
