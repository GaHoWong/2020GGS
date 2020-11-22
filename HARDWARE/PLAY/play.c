#include "play.h"


void  PLAY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOF, ENABLE); 	//IO口时钟配置

	//IO配置
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;   	 //复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		 //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度等级

	//初始化 TX	引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	//初始化 RX 引脚													   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);		
	
	//IO复用，复用到UART7
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_UART7); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_UART7);	
		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
	
	// IO口初始化
		 
	// 配置串口各项参数
	USART_InitStructure.USART_BaudRate 	 = 9600; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //停止位1位
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //无校验
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //发送和接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用硬件流控制
	
	USART_Init(UART7,&USART_InitStructure); //初始化串口7
	USART_Cmd(UART7,ENABLE);	//使能串口7
}
