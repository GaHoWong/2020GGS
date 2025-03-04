#include "sys.h"
#include "usart.h"	
#include "oled.h"
#include "led.h"
#include "analyse.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

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
/*int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}*/
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART6_RX_BUF[USART6_REC_LEN];

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记
u16 USART6_RX_STA=0;       //接收状态标记	


  
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
void uart_init(u32 pclk2,u32 bound)
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




//PG9--USART6_RX   PG14--USART6_TX
// 函数：usart IO口初始化
void USART6_Init(u32 bound )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  
	RCC_AHB1PeriphClockCmd(USART6_RX_RCC_APB2Periph_GPIOx ,ENABLE);   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //使能串口6时钟
    
	GPIO_PinAFConfig(USART6_TX_GPIOx,USART6_TX_GPIO_PinSourcex,GPIO_AF_USART6);  
	GPIO_PinAFConfig(USART6_RX_GPIOx,USART6_RX_GPIO_PinSourcex,GPIO_AF_USART6);  

	GPIO_InitStructure.GPIO_Pin   = USART6_TX_GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
	GPIO_Init(USART6_TX_GPIOx,&GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin   = USART6_RX_GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
	GPIO_Init(USART6_RX_GPIOx,&GPIO_InitStructure); 
	
	//初始化设置
	USART_InitStructure.USART_BaudRate                   = bound;                         //串口波特率
	USART_InitStructure.USART_WordLength                 = USART_WordLength_8b;           //字长为8位数据格式
	USART_InitStructure.USART_StopBits                   = USART_StopBits_1;              //一个停止位
	USART_InitStructure.USART_Parity                     = USART_Parity_No;               //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl        = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode                       = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART6, &USART_InitStructure);                                             //初始化串口
	
	
	//NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);	 
  
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);                                        //开启串口空闲IDEL中断
	USART_Cmd(USART6, ENABLE);                                                            //使能串口 
    
    

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);                                        //开启串口接受中断

  //DMA_USART6_Init();                                                                    //串口 DMA 配置
 
}



void USART6_IRQHandler(void)
{	
	u8 Data=0;
	
	LED1_TOGGLE;
	
    if (USART_GetFlagStatus(USART6, USART_FLAG_PE) != RESET)
   {
       USART_ReceiveData(USART6);
     USART_ClearFlag(USART6, USART_FLAG_PE);
   }
   if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)
   {
       USART_ReceiveData(USART6);
     USART_ClearFlag(USART6, USART_FLAG_ORE);
   }
    
    if (USART_GetFlagStatus(USART6, USART_FLAG_FE) != RESET)
   {
       USART_ReceiveData(USART6);
      USART_ClearFlag(USART6, USART_FLAG_FE);
   }
//为了避免接收错误,将一些不正常状态滤掉
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {   
        USART_ClearFlag(USART6, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        Data = USART_ReceiveData(USART6);
			//printf("%c",Data);
        analyse(Data); //处理函数
    } 
} 







