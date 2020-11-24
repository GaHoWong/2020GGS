#include "breeze.h" 
#include "delay.h"

//初始化PB0和PB1为输出口.并使能这两个口的时钟		    
//LED IO初始化
void BREEZE_Init(void)
{    	 
	RCC->AHB1ENR|=1<<7;//使能PORTH 时钟 
	GPIO_Set(GPIOH,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PH3设置
	BREEZE=1;//BREEZE初始化完成响一声
	delay_ms(600);                  //延时等待
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




