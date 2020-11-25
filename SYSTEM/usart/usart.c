#include "sys.h"
#include "usart.h"	
#include "oled.h"
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
u8 Data=0;
    if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
   {
       USART_ReceiveData(USART1);
     USART_ClearFlag(USART1, USART_FLAG_PE);
   }
   if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
   {
       USART_ReceiveData(USART1);
     USART_ClearFlag(USART1, USART_FLAG_ORE);
   }
    
    if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
   {
       USART_ReceiveData(USART1);
      USART_ClearFlag(USART1, USART_FLAG_FE);
   }
//Ϊ�˱�����մ���,��һЩ������״̬�˵�
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {   
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        Data = USART_ReceiveData(USART1);
		OPENMV_date_anl(Data); //������
    } 
//	u8 res;	
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART1->SR&(1<<5))//���յ�����
//	{	 
//		res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//			}else //��û�յ�0X0D
//			{	
//				if(res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}  		 									     
//	} 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif





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


void K210_USART(u32 pclk2,u32 bound){
	
	 
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
 	GPIO_AF_Set(GPIOG,9,8);	//PG9,AF8
	GPIO_AF_Set(GPIOG,14,8);//PG14,AF8  	   
	//����������
 	USART1->BRR=mantissa; 	//����������	 
	USART1->CR1&=~(1<<15); 	//����OVER8=0 
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
#if EN_USART1_RX		  	//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART6->CR1|=1<<2;  	//���ڽ���ʹ��
	USART6->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,2,USART6_IRQn,2);//��2��������ȼ� 
#endif
	USART6->CR1|=1<<13;  	//����ʹ��
}


void USART6_IRQHandler(void)
{
//	u8 res;	
//	if(USART6->SR&(1<<5))//���յ�����
//	{	 
//		res=USART6->DR; 
//		if((USART6_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART6_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(res!=0x0a)USART6_RX_STA=0;//���մ���,���¿�ʼ
//				else USART6_RX_STA|=0x8000;	//��������� 
//			}else //��û�յ�0X0D
//			{	
//				if(res==0x0d)USART6_RX_STA|=0x4000;
//				else
//				{
//					USART6_RX_BUF[USART6_RX_STA&0X3FFF]=res;
//					USART6_RX_STA++;
//					if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}  		 									     
//	} 
	u8 Data=0;
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
		OPENMV_date_anl(Data); //������
    } 


} 




extern int boll_x;
extern int boll_y;
void OPENMV_date_anl(u8 data)
{
	static u8 RxBuffer[10];
	static u8 state = 0;
/*ͨ�Ÿ�ʽ 0xAA 0x55 data1 data2 checkout 0x54*/	
/*����checkout=(data1+data2)�Ͱ�λ  ���� data1=0xe1,data2=0xf3,data1+data2=0x1d4,��checkout=0xd4*/
	if(state==0&&data==0xAA)
		state=1;
	else if(state==1&&data==0x55)
		state=2;
	else if(state==2)
	{
		RxBuffer[0]=data;//x
			state=3;
	}
	else if(state==3)
	{	
		RxBuffer[1]=data;//y
		state = 4;
	}
	else if(state==4)
	{	
		RxBuffer[2]=data;//checkout
		state = 5;
	}

	else if(state==5&&data==0x54)
	{	
		if(RxBuffer[2]==(u8)(RxBuffer[0]+RxBuffer[1]))//У��ɹ�
		{
		boll_x=RxBuffer[0];
		boll_y=RxBuffer[1];

		}
	
		state = 0;
	}
	else
		state = 0;
}








