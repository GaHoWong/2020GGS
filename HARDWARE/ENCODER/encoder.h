#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"


#define ENCODER_TIM_PERIOD (u16)(65535)



void EncoderInit(void);//±àÂëÆ÷³õÊ¼»¯º¯Êý
void TIM2_ENC_Init(void);
void TIM3_ENC_Init(void);
void TIM4_ENC_Init(void);
void TIM5_ENC_Init(void);



void Encoder_Init_TIM5(void);

void Caculate_Encoder(void);
int Read_Encoder(int TIMX);



#endif

