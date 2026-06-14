#ifndef _MYI2C_H
#define _MYI2C_H




void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReadByte(void);
void MyI2C_SendAck(uint8_t Ack);
uint8_t MyI2C_ReadAck(void);


#endif

