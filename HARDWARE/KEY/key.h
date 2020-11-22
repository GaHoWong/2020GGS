#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

//Òý½Å¶¨ÒåPH2 and PH12
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOH
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOH
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_2
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOH
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource2
#define KEY1_INT_EXTI_LINE                EXTI_Line2
#define KEY1_INT_EXTI_IRQ                 EXTI2_IRQn

#define KEY1_IRQHandler                   EXTI2_IRQHandler

#define KEY2_INT_GPIO_PORT                GPIOH
#define KEY2_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOH
#define KEY2_INT_GPIO_PIN                 GPIO_Pin_12
#define KEY2_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOH
#define KEY2_INT_EXTI_PINSOURCE           EXTI_PinSource12
#define KEY2_INT_EXTI_LINE                EXTI_Line12
#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define KEY2_IRQHandler                   EXTI15_10_IRQHandler

/*******************************************************/


void KEY_Init(void);

#endif
