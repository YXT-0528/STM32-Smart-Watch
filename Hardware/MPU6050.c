#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"  
#include "MPU6050_RegAddress.h" 

#define MPU6050_Address     0xD0                   //宏定义MPU6050地址

void MPU6050_Write(uint8_t RegAddress,uint8_t Data)          //向MPU6050发送数据，便于后续函数梳理时序
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address);
	MyI2C_ReadAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReadAck();
	MyI2C_SendByte(Data);
	MyI2C_ReadAck();
	MyI2C_Stop();
}

uint8_t MPU6050_Read(uint8_t RegAddress)                   //读取MPU6050指定地址数据，便于后续函数梳理时序
{
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address);
	MyI2C_ReadAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReadAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address|0X01);
	MyI2C_ReadAck();
	Data=MyI2C_ReadByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	return Data;
}

uint8_t MPU6050_R_ID(void)                              //读取设备ID号
{
	return MPU6050_Read(MPU6050_WHO_AM_I);
}



void MPU6050_Init(void)                     //MPU6050初始化，参考数据手册时序
{
	MyI2C_Init();
	MPU6050_Write(MPU6050_PWR_MGMT_1,0X01);
	MPU6050_Write(MPU6050_PWR_MGMT_2,0X00);
	MPU6050_Write(MPU6050_SMPLRT_DIV,0X04);
	MPU6050_Write(MPU6050_CONFIG,0X06);
	MPU6050_Write(MPU6050_GYRO_CONFIG,0X18);
	MPU6050_Write(MPU6050_ACCEL_CONFIG,0X18);
}

void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)//读取数据
{
	uint8_t Data_H,Data_L;
	Data_H=MPU6050_Read(MPU6050_ACCEL_XOUT_H);
	Data_L=MPU6050_Read(MPU6050_ACCEL_XOUT_L);
	*AccX=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_Read(MPU6050_ACCEL_YOUT_H);
	Data_L=MPU6050_Read(MPU6050_ACCEL_YOUT_L);
	*AccY=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_Read(MPU6050_ACCEL_ZOUT_H);
	Data_L=MPU6050_Read(MPU6050_ACCEL_ZOUT_L);
	*AccZ=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_Read(MPU6050_GYRO_XOUT_H);
	Data_L=MPU6050_Read(MPU6050_GYRO_XOUT_L);
	*GyroX=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_Read(MPU6050_GYRO_YOUT_H);
	Data_L=MPU6050_Read(MPU6050_GYRO_YOUT_L);
	*GyroY=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_Read(MPU6050_GYRO_ZOUT_H);
	Data_L=MPU6050_Read(MPU6050_GYRO_ZOUT_L);
	*GyroZ=(Data_H<<8)|Data_L;
}                                                //定义指针来接收函数返回值，便于显示，分别代表加速度和角速度的6个方向值



