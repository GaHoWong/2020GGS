#include "led.h" 

//��ʼ��PB0��PB1Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
	RCC->AHB1ENR|=1<<8;//ʹ��PORTIʱ�� 
	RCC->AHB1ENR|=1<<3;//ʹ��PORTDʱ�� 
	GPIO_Set(GPIOI,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PI3����
	GPIO_Set(GPIOD,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PD4����
	
	LED0=1;//LED0�ر�
	LED1=1;//LED1�ر�
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





