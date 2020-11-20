#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
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

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)  
//解决HAL库使用时,某些情况可能报错的bug
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void USART1_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif										 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<0;   	//使能PORTA口时钟  
	RCC->APB2ENR|=1<<4;  	//使能串口1时钟 
	GPIO_Set(GPIOA,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
 	GPIO_AF_Set(GPIOA,9,7);	//PA9,AF7
	GPIO_AF_Set(GPIOA,10,7);//PA10,AF7  	   
	//波特率设置
 	USART1->BRR=mantissa; 	//波特率设置	 
	USART1->CR1&=~(1<<15); 	//设置OVER8=0 
	USART1->CR1|=1<<3;  	//串口发送使能 
#if EN_USART1_RX		  	//如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<2;  	//串口接收使能
	USART1->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
	USART1->CR1|=1<<13;  	//串口使能
}




void usart_init(void){

		uart1_init(90,115200); //与电脑端通讯
		uart6_init(90,115200);//与k210端通讯
		uart6_init(90,115200);//与k210端通讯
}





//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart6_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<6;   	//使能PORTA口时钟  
	RCC->APB2ENR|=1<<4;    	//使能串口1时钟 
	GPIO_Set(GPIOG,PIN14|PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PG14,PG9复用功能,上拉输出
 	GPIO_AF_Set(GPIOG,14,8);	//TX-PG14,AF8
	GPIO_AF_Set(GPIOG,9,8);//RX-PG9,AF8  	   
	//波特率设置
 	USART1->BRR=mantissa; 	//波特率设置	 
	USART1->CR1&=~(1<<15); 	//设置OVER8=0 
	USART1->CR1|=1<<3;  	//串口发送使能 
#if EN_USART1_RX		  	//如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<2;  	//串口接收使能
	USART1->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
	USART1->CR1|=1<<13;  	//串口使能
}




////USART6串口接收中断
//void USART6_IRQHandler(void)
//{
//	u8 Res,len;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  // 接收中断，接收到的数据必须以0x0d，0x0a结尾
//	{
//		printf("123");
//		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误，重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了
//				
//				len=USART_RX_STA&0x3fff;
//				
//				for(int t=0;t<len;t++)
//				{
//				USART2->DR=USART_RX_BUF[t];
//				while((USART2->SR&0X40)==0);
//				}
//				printf("%d%d%d  %d%d%d \r\n",USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[4],USART_RX_BUF[5],USART_RX_BUF[6]);
//				USART_RX_STA=0;
//			}
//			else //还没收到0x0d
//			{	
//				if(Res==0x0d)
//				{
//					USART_RX_STA|=0x4000;
//				}
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误，重新开始接收
//				}		 
//			}
//		} 
//	}		
//	//  the width limit of k210-lcd:320---------the high limit of k210-lcd:240
//	if( (Res==0x0a)&&(USART_RX_BUF[3]!='/')&&(USART_RX_BUF[7]!='\n' ) )//判断接收到的数据是否符合协议，若否，则赋予所有数据为‘w’
//	{
//		 for(int count_r=0;count_r<8;count_r++)
//		{
//			USART_RX_BUF[count_r]='w';
//		}
//		USART_RX_STA=0;
//	}
////	Music_Play(0);
//	if((USART_RX_BUF[0]=='1')&&(USART_RX_BUF[4]=='1')&&(USART_RX_BUF[1]=='4')&&(USART_RX_BUF[5]=='4'))//判断是否在设定坐标左右，若是，则蜂鸣器响
//	{
//		printf("noooo");
//	}
//}





void USART6_IRQHandler(void)                	//串口3中断服务程序
	{
	u8 Data=0;
    if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)
   {
       USART_ReceiveData(USART3);
     USART_ClearFlag(USART3, USART_FLAG_PE);
   }
   if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
   {
       USART_ReceiveData(USART3);
     USART_ClearFlag(USART3, USART_FLAG_ORE);
   }
    
    if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
   {
       USART_ReceiveData(USART3);
      USART_ClearFlag(USART3, USART_FLAG_FE);
   }
//为了避免接收错误,将一些不正常状态滤掉
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {   
        USART_ClearFlag(USART3, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        Data = USART_ReceiveData(USART3);
		//OPENMV_date_anl(Data); //处理函数
			printf("%d",Data);
    } 		 
} 
	







