#include "breeze.h" 
#include "delay.h"

//��ʼ��PB0��PB1Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void BREEZE_Init(void)
{    	 
	RCC->AHB1ENR|=1<<7;//ʹ��PORTH ʱ�� 
	GPIO_Set(GPIOH,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PH3����
	BREEZE=1;//BREEZE��ʼ�������һ��
	delay_ms(600);                  //��ʱ�ȴ�
	BREEZE = 0;                     
}

/*
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 
*/




