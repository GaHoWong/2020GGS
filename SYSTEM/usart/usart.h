#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ�� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵�� 
//V1.1   20160120
//����_ttywrch����,�޸�ʹ��HAL��ʱ,���ܱ�__use_no_semihosting�Ĵ���. 
////////////////////////////////////////////////////////////////////////////////// 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void uart1_init(u32 pclk2,u32 bound); 
void uart6_init(u32 pclk2,u32 bound);
void usart_init(void);
















//���Ŷ���
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


//���ڲ�����
#define PLAY_USART_BAUDRATE                    9600
void PLAY (char *fmt, ...);


void uart7_init(void);
void uart7_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void uart7_SendString( USART_TypeDef * pUSARTx, uint8_t *str);









#endif	   
















