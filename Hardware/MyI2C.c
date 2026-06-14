#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

  
  
  //将软件I2C封装为一个可调用函数，便于工程中修改

void MyI2C_W_SCL(uint8_t BitValue)                                //将写SCL封装为一个函数，便于后续编写时序
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)                               //封装写SDA函数
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)                                            //封装读SDA时序，用于读取从机数据
{
	uint8_t Temp;
	Temp=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return Temp;
}

void MyI2C_Init(void)                                               //I2C软件初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;               //分别控制SCL与SDA
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}


void MyI2C_Start(void)                             //开始时序，参考I2C时序图，开始后保证SDA与SCL皆为低电平
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)                               //停止时序，参考I2C时序图，停止时SDA与SCL置高电平
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)                     //发送一个字节时序
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte & (0x80>>i));                       //for循环连续发送8bit数据即一个字节
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReadByte(void)                                //接收一个字节
{
	uint8_t i,Temp=0x00;
	MyI2C_W_SDA(1);
	for(i=0;i<8;i++)                                     
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()==1){Temp |=(0x80>>i);}                //Temp=0x00，如果从机i位读取为1，则将i位或1
		MyI2C_W_SCL(0);
	}
	return Temp;
}

void MyI2C_SendAck(uint8_t Ack)                               //发送校验位，控制从机是否继续接收主机发送数据
{
	MyI2C_W_SDA(Ack);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}


uint8_t MyI2C_ReadAck(void)                                       //接受校验位，控制主机是否继续读取从机数据
{
	uint8_t Temp;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	Temp=MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return Temp;
}

