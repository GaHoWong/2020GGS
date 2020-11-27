#ifndef __TFMINI_H
#define __TFMINI_H

#include "stdio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "sys.h"


/*----------------------USART配置宏 ------------------------*/

#define  USART1_BaudRate  115200

//移植修改区start
//全局替换.h  .c 的串口USART3为移植的串口，并修改下面代码

//USART3   y
#define USART3_TX_RCC_APB2Periph_GPIOx      RCC_AHB1Periph_GPIOB
#define USART3_TX_GPIOx                     GPIOB
#define USART3_TX_GPIO_Pin_x                GPIO_Pin_10
#define USART3_TX_GPIO_PinSourcex           GPIO_PinSource10

#define USART3_RX_RCC_APB2Periph_GPIOx      RCC_AHB1Periph_GPIOB
#define USART3_RX_GPIOx                     GPIOB
#define USART3_RX_GPIO_Pin_x                GPIO_Pin_11
#define USART3_RX_GPIO_PinSourcex           GPIO_PinSource11

#define RCC_AHBxPeriph_DMAy                 RCC_AHB1Periph_DMA1

#define USART3_RX_DMAx_Streamy_IRQHandler   DMA1_Stream1_IRQHandler
#define USART3_RX_DMAx_Streamy_IRQn         DMA1_Stream1_IRQn
#define USART3_RX_DMAx_Streamy              DMA1_Stream1
#define USART3_RX_DMA_IT_TCIFx              DMA_IT_TCIF1
#define USART3_RX_DMA_Channelx              DMA_Channel_4


//USART2   x
#define USART2_TX_RCC_APB2Periph_GPIOx      RCC_AHB1Periph_GPIOD
#define USART2_TX_GPIOx                     GPIOD
#define USART2_TX_GPIO_Pin_x                GPIO_Pin_5
#define USART2_TX_GPIO_PinSourcex           GPIO_PinSource5

#define USART2_RX_RCC_APB2Periph_GPIOx      RCC_AHB1Periph_GPIOD
#define USART2_RX_GPIOx                     GPIOD
#define USART2_RX_GPIO_Pin_x                GPIO_Pin_6
#define USART2_RX_GPIO_PinSourcex           GPIO_PinSource6

#define USART2_RX_DMAx_Streamy_IRQHandler   DMA1_Stream5_IRQHandler
#define USART2_RX_DMAx_Streamy_IRQn         DMA1_Stream5_IRQn
#define USART2_RX_DMAx_Streamy              DMA1_Stream5
#define USART2_RX_DMA_IT_TCIFx              DMA_IT_TCIF1
#define USART2_RX_DMA_Channelx              DMA_Channel_4




#define USART2_MAX_RX_LEN 9
#define USART2_MAX_TX_LEN 9
#define USART3_MAX_RX_LEN 9
#define USART3_MAX_TX_LEN 9

void     USART3_Init(u32 bound);
void     DMA_USART3_Init(void); 

void USART2_Init(u32 bound );
void DMA_USART2_Init(void);


void TFmini_Init(void);




#endif //__TFMINI_H

