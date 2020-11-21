#ifndef __TFMINI_H
#define __TFMINI_H

#include "stdio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
/*----------------------USART≈‰÷√∫Í ------------------------*/

#define  USART1_BaudRate  115200


void     USART3_Init(u32 bound);
void     DMA_USART3_Init(void); 

void USART2_Init(u32 bound );
void DMA_USART2_Init(void);
#endif //__TFMINI_H

