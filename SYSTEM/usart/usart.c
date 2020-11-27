#include "sys.h"
#include "usart.h"	
#include "oled.h"
#include "led.h"
#include "analyse.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)  
//���HAL��ʹ��ʱ,ĳЩ������ܱ����bug
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
/*int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}*/
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART6_RX_BUF[USART6_REC_LEN];

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���
u16 USART6_RX_STA=0;       //����״̬���	


  
void USART1_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART1->SR&(1<<5))//���յ�����
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	} 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif										 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV@OVER8=0
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<0;   	//ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<4;  	//ʹ�ܴ���1ʱ�� 
	GPIO_Set(GPIOA,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOA,9,7);	//PA9,AF7
	GPIO_AF_Set(GPIOA,10,7);//PA10,AF7  	   
	//����������
 	USART1->BRR=mantissa; 	//����������	 
	USART1->CR1&=~(1<<15); 	//����OVER8=0 
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
#if EN_USART1_RX		  	//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<2;  	//���ڽ���ʹ��
	USART1->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
	USART1->CR1|=1<<13;  	//����ʹ��
}




//PG9--USART6_RX   PG14--USART6_TX
// ������usart IO�ڳ�ʼ��
void USART6_Init(u32 bound )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  
	RCC_AHB1PeriphClockCmd(USART6_RX_RCC_APB2Periph_GPIOx ,ENABLE);   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //ʹ�ܴ���6ʱ��
    
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
	
	//��ʼ������
	USART_InitStructure.USART_BaudRate                   = bound;                         //���ڲ�����
	USART_InitStructure.USART_WordLength                 = USART_WordLength_8b;           //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits                   = USART_StopBits_1;              //һ��ֹͣλ
	USART_InitStructure.USART_Parity                     = USART_Parity_No;               //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl        = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode                       = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure);                                             //��ʼ������
	
	
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);	 
  
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);                                        //�������ڿ���IDEL�ж�
	USART_Cmd(USART6, ENABLE);                                                            //ʹ�ܴ��� 
    
    

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);                                        //�������ڽ����ж�

  //DMA_USART6_Init();                                                                    //���� DMA ����
 
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
//Ϊ�˱�����մ���,��һЩ������״̬�˵�
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {   
        USART_ClearFlag(USART6, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        Data = USART_ReceiveData(USART6);
			//printf("%c",Data);
        analyse(Data); //������
    } 
} 







