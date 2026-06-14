#ifndef _KEY_H
#define _KEY_H

void Key_Init(void);
uint8_t KeyGetnum(void);
uint8_t Key_GetState(void);
void Key_Systic(void);
void Key_Tick(void);
extern int Key_Presstime;
extern uint8_t KeyNum; 

#endif

