#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"




#define TimerClkAPB1 45000000		//挂载在 APB1时钟线上的定时器时钟频率为 90M
#define TimerClkAPB2 90000000		//挂载在 APB2 时钟线上的定时器时钟频率为 90M







void TIM3_Int_Init(u16 arr,u16 psc);




#endif 
