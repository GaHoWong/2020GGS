#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  
 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define USART6_REC_LEN  			200  	//定义最大接收字节数 200


#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收



#define USART6_TX_RCC_APB2Periph_GPIOx      RCC_AHB1Periph_GPIOG
#define USART6_TX_GPIOx                     GPIOG
#define USART6_TX_GPIO_Pin_x                GPIO_Pin_14
#define USART6_TX_GPIO_PinSourcex           GPIO_PinSource14

#define USART6_RX_RCC_APB2Periph_GPIOx      RCC_AHB1Periph_GPIOG
#define USART6_RX_GPIOx                     GPIOG
#define USART6_RX_GPIO_Pin_x                GPIO_Pin_9
#define USART6_RX_GPIO_PinSourcex           GPIO_PinSource9





	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void uart_init(u32 pclk2,u32 bound);
void USART6_Init(u32 bound );



void OPENMV_date_anl(u8 data);



#endif	   
















