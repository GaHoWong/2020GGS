#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"


/*--------------------------------	IO配置 ----------------------------------*/

#define	TIM12_CH1_PORT						GPIOB
#define	TIM12_CH1_CLK							RCC_AHB1Periph_GPIOB
#define  TIM12_CH1_PIN						GPIO_Pin_14
#define  TIM12_CH1_AF             GPIO_AF_TIM12
#define  TIM12_CH1_PinSource      GPIO_PinSource14

#define	TIM12_CH2_PORT						GPIOB
#define	TIM12_CH2_CLK						RCC_AHB1Periph_GPIOB
#define  TIM12_CH2_PIN						GPIO_Pin_15
#define  TIM12_CH2_AF               GPIO_AF_TIM12
#define  TIM12_CH2_PinSource        GPIO_PinSource15

#define	TIM9_CH1_PORT						GPIOE
#define	TIM9_CH1_CLK						RCC_AHB1Periph_GPIOE
#define  TIM9_CH1_PIN						GPIO_Pin_5
#define  TIM9_CH1_AF             		GPIO_AF_TIM9
#define  TIM9_CH1_PinSource      		GPIO_PinSource5

#define	TIM9_CH2_PORT						GPIOE
#define	TIM9_CH2_CLK						RCC_AHB1Periph_GPIOE
#define  TIM9_CH2_PIN						GPIO_Pin_6
#define  TIM9_CH2_AF             		GPIO_AF_ TIM9
#define  TIM9_CH2_PinSource      		GPIO_PinSource6






/*--------------------------------- 宏 -------------------------------------*/

#define TimerClkAPB1 45000000		//挂载在 APB1 时钟线上的定时器时钟频率为 90M

#define TIM_CH1 1   //定时器通道
#define TIM_CH2 2
#define TIM_CH3 3
#define TIM_CH4 4

/*------------------------------- 函数声明 -----------------------------------*/

void TIM12_PWMinit(u16 period,u32 frequency);		// 初始化 TIM12 ，并进行PWM的设置
void TIM12_PwmSetPulse (u8 TIM12_CH,u8 pulse);		// 设置TIM12的PWM占空比
	
#endif

