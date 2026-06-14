#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "menu.h"
#include "OLED.h"
#include "Timer.h"
#include "dino.h"
#include "Key.h"



int main(void)
{
    OLED_Init();
	menu_init();
	int clkFlag1;
	Timer_Init();
    extern int Key_Presstime;
	extern uint8_t KeyNum;
 while(1)
 {
//	OLED_ShowNum(64,0,Key_Presstime,4,OLED_6X8);
//	OLED_ShowNum(64,8,KeyNum,1,OLED_6X8);
//	OLED_Update();
	clkFlag1=First_Page_Clock();
	if(clkFlag1==1){menu();}
	else if(clkFlag1==2){SettingPage();}
 }
}


void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
  {
      Key_Systic();
	  Stop_Tick();
	  Key_Tick();
	  Score_Tick();
	  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  }
}
