#include "stm32f10x.h"                  // Device header



void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);           //开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                            //选择ADC转换的时钟分频，要求AD转换在14MHZ以下
	
	
	GPIO_InitTypeDef  GPIO_InitStruct;                             //选择IO口
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);      //设置规则组的通道
	
	ADC_InitTypeDef  ADC_InitStruct;                                 //初始化ADC
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;                   //选择转换模式是连续转换还是单次转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;                  //选择数据左对齐还是右对齐
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;       //外部触发源，本实验采用软件控制，所以不需要外部触发源
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;                       //选择双ADC模式还是单ADC模式
	ADC_InitStruct.ADC_NbrOfChannel=1;                               //指定规则组转换通道的数目
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;                          //扫描模式：扫描还是不扫描
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1,ENABLE);                       //启动
	
	ADC_ResetCalibration(ADC1);                                //校准代码，固定
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
}


uint16_t AD_GetValue(void)                                        //获取转换值
{ 
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);                      //软件触发ADC转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);             //等待转化完成
     
	return ADC_GetConversionValue(ADC1);
  
}



