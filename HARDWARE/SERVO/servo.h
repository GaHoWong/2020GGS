#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"


/*--------------------------------	IO���� ----------------------------------*/

#define	TIM12_CH1_PORT						GPIOB
#define	TIM12_CH1_CLK							RCC_AHB1Periph_GPIOB
#define  TIM12_CH1_PIN						GPIO_Pin_14
#define  TIM12_CH1_AF             GPIO_AF_TIM12
#define  TIM12_CH1_PinSource      GPIO_PinSource14

/*--------------------------------- �� -------------------------------------*/

#define TimerClkAPB1 45000000		//������ APB1 ʱ�����ϵĶ�ʱ��ʱ��Ƶ��Ϊ 90M

#define TIM_CH1 1   //��ʱ��ͨ��
#define TIM_CH2 2
#define TIM_CH3 3
#define TIM_CH4 4

/*------------------------------- �������� -----------------------------------*/

void TIM12_PWMinit(u16 period,u32 frequency);		// ��ʼ�� TIM12 ��������PWM������
void TIM12_PwmSetPulse (u8 TIM12_CH,u8 pulse);		// ����TIM12��PWMռ�ձ�
	
#endif

