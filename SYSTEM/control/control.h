#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.14159265
int  TIM3_IRQHandler(void); 
int myabs(int a);
void Set_Pwm(int moto);
void Xianfu_Pwm(void);
int Incremental_PI (int Encoder,int Target);
int Read_Encoder(u8 TIMX);



#define PWMA   TIM8->CCR3
#define AIN2   PCout(8)
#define AIN1   PCout(9)
#define BIN1   PBout(13)
#define BIN2   PBout(12)
#define PWMB   TIM8->CCR4



#endif

