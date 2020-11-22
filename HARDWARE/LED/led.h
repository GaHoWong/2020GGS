#ifndef __LED_H
#define __LED_H	 
#include "sys.h" 

//LED端口定义

#define LED0 PDout(4)	// DS0
#define LED1 PIout(3)	// DS1	 

#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态
#define LED1_TOGGLE		digitalToggle(GPIOD,GPIO_Pin_4) //LED
#define LED2_TOGGLE		digitalToggle(GPIOI,GPIO_Pin_3)


void LED_Init(void);//初始化		 				    
#endif

















