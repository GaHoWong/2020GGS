#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"




#define TimerClkAPB1 45000000		//������ APB1ʱ�����ϵĶ�ʱ��ʱ��Ƶ��Ϊ 90M
#define TimerClkAPB2 90000000		//������ APB2 ʱ�����ϵĶ�ʱ��ʱ��Ƶ��Ϊ 90M







void TIM3_Int_Init(u16 arr,u16 psc);




#endif 
