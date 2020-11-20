#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明 
//V1.1   20160120
//新增_ttywrch函数,修改使用HAL库时,可能报__use_no_semihosting的错误. 
////////////////////////////////////////////////////////////////////////////////// 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void uart1_init(u32 pclk2,u32 bound); 
void uart6_init(u32 pclk2,u32 bound);
void usart_init(void);
















//引脚定义
/*******************************************************/
#define PLAY_USART                             UART7
#define PLAY_USART_CLK                         RCC_APB1Periph_UART7

#define PLAY_USART_RX_GPIO_PORT                GPIOF
#define PLAY_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOF
#define PLAY_USART_RX_PIN                      GPIO_Pin_7
#define PLAY_USART_RX_AF                       GPIO_AF_UART7
#define PLAY_USART_RX_SOURCE                   GPIO_PinSource7

#define PLAY_USART_TX_GPIO_PORT                GPIOF
#define PLAY_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOF
#define PLAY_USART_TX_PIN                      GPIO_Pin_6
#define PLAY_USART_TX_AF                       GPIO_AF_UART7
#define PLAY_USART_TX_SOURCE                   GPIO_PinSource6


#define PLAY_USART_IRQHandler                   UART7_IRQHandler
#define PLAY_USART_IRQ                 					UART7_IRQn
/************************************************************/


//串口波特率
#define PLAY_USART_BAUDRATE                    9600
void PLAY (char *fmt, ...);


void uart7_init(void);
void uart7_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void uart7_SendString( USART_TypeDef * pUSARTx, uint8_t *str);









#endif	   
















