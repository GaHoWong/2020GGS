#include "tfmini.h"  
#include "stdio.h"
#include "stdarg.h"
// ������usart IO�ڳ�ʼ��
//

#define USART3_MAX_RX_LEN 9
#define USART3_MAX_TX_LEN 9
u8   USART3_TX_BUF[USART3_MAX_TX_LEN]; 		//���ͻ���,���USART3_MAX_TX_LEN-1�ֽ�
u8   USART3_RX_BUF[USART3_MAX_RX_LEN]; 		//���ջ���,���USART3_MAX_RX_LEN-1�ֽ�
volatile u16 USART3_RX_STA=0;               //bit15:������ɱ�־   bit14~0:���յ�����Ч�ֽ���Ŀ    

#define USART2_MAX_RX_LEN 9
#define USART2_MAX_TX_LEN 9
u8   USART2_TX_BUF[USART2_MAX_TX_LEN]; 		//���ͻ���,���USART3_MAX_TX_LEN-1�ֽ�
u8   USART2_RX_BUF[USART2_MAX_RX_LEN]; 		//���ջ���,���USART3_MAX_RX_LEN-1�ֽ�
volatile u16 USART2_RX_STA=0;               //bit15:������ɱ�־   bit14~0:���յ�����Ч�ֽ���Ŀ    


//��ֲ�޸���start
//ȫ���滻.h  .c �Ĵ���USART3Ϊ��ֲ�Ĵ��ڣ����޸��������

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



void TFmini_Init(void){
	
	USART3_Init(115200);
	USART2_Init(115200);

}



void USART3_Init(u32 bound )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  
	RCC_AHB1PeriphClockCmd(USART3_RX_RCC_APB2Periph_GPIOx ,ENABLE);   
	RCC_APB1PeriphClockCmd(((uint32_t)0x00040000), ENABLE); //ʹ�ܴ���3ʱ��
    
	GPIO_PinAFConfig(USART3_TX_GPIOx,USART3_TX_GPIO_PinSourcex,GPIO_AF_USART3);  
	GPIO_PinAFConfig(USART3_RX_GPIOx,USART3_RX_GPIO_PinSourcex,GPIO_AF_USART3);  

	GPIO_InitStructure.GPIO_Pin   = USART3_TX_GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
	GPIO_Init(USART3_TX_GPIOx,&GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin   = USART3_RX_GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
	GPIO_Init(USART3_RX_GPIOx,&GPIO_InitStructure); 
	
	//��ʼ������
	USART_InitStructure.USART_BaudRate                   = bound;                         //���ڲ�����
	USART_InitStructure.USART_WordLength                 = USART_WordLength_8b;           //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits                   = USART_StopBits_1;              //һ��ֹͣλ
	USART_InitStructure.USART_Parity                     = USART_Parity_No;               //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl        = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode                       = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);                                             //��ʼ������
	
	
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);	 
  
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);                                        //�������ڿ���IDEL�ж�
	USART_Cmd(USART3, ENABLE);                                                            //ʹ�ܴ��� 
    
    

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                                        //�������ڽ����ж�

  //DMA_USART3_Init();                                                                    //���� DMA ����
 
	printf("USART3 OK...\r\n");
}

void DMA_USART3_Init(void)
{
  DMA_InitTypeDef DMA_USART3_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHBxPeriph_DMAy, ENABLE);                                          //����DMAʱ��
    
	//����DMA����  
	DMA_DeInit(USART3_RX_DMAx_Streamy);                                                           //DMA������
	DMA_USART3_InitStructure.DMA_Channel            = USART3_RX_DMA_Channelx;                     //DMAͨ��
	DMA_USART3_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);                    //�����ַ
	DMA_USART3_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)USART3_RX_BUF;                    //�ڴ��ַ
	DMA_USART3_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;	              //dma���䷽��
	DMA_USART3_InitStructure.DMA_BufferSize         = USART3_MAX_RX_LEN;                          //����DMA�ڴ���ʱ�������ĳ���
	DMA_USART3_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;                  //����DMA���������ģʽ��һ������
	DMA_USART3_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;	                      //����DMA���ڴ����ģʽ	
	DMA_USART3_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;                //���������ֳ�
	DMA_USART3_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;	                  //�ڴ������ֳ�
	DMA_USART3_InitStructure.DMA_Mode               = DMA_Mode_Normal;	                          //����DMA�Ĵ���ģʽ
	DMA_USART3_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;	                  //����DMA�����ȼ���	
	DMA_USART3_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;                       //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ     
	DMA_USART3_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull; 	              //ָ����FIFO��ֵˮƽ       
	DMA_USART3_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;    	              //ָ����Burstת�������ڴ洫��    
	DMA_USART3_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;                 //ָ����Burstת��������Χת��      
	DMA_Init(USART3_RX_DMAx_Streamy, &DMA_USART3_InitStructure);                                  //����DMA��ͨ��  
 
  DMA_ClearITPendingBit(USART3_RX_DMAx_Streamy,USART3_RX_DMA_IT_TCIFx);                         //�����־λ  
	DMA_Cmd(USART3_RX_DMAx_Streamy,ENABLE);                                                       //ʹ�ܽ���ͨ��
	DMA_ITConfig(USART3_RX_DMAx_Streamy,DMA_IT_TC,ENABLE);                                        //ʹ�ܷ�������ж� 
    
	NVIC_InitStructure.NVIC_IRQChannel                   = USART3_RX_DMAx_Streamy_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);                                                  //����DMA��ʽ����
}

void USART2_Init(u32 bound )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  
	RCC_AHB1PeriphClockCmd(USART2_RX_RCC_APB2Periph_GPIOx ,ENABLE);   
	RCC_APB1PeriphClockCmd(((uint32_t)0x00020000), ENABLE); //ʹ�ܴ���2ʱ��

    
	GPIO_PinAFConfig(USART2_TX_GPIOx,USART2_TX_GPIO_PinSourcex,GPIO_AF_USART2);  
	GPIO_PinAFConfig(USART2_RX_GPIOx,USART2_RX_GPIO_PinSourcex,GPIO_AF_USART2);  

	GPIO_InitStructure.GPIO_Pin   = USART2_TX_GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
	GPIO_Init(USART2_TX_GPIOx,&GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin   = USART2_RX_GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
	GPIO_Init(USART2_RX_GPIOx,&GPIO_InitStructure); 
	
	//��ʼ������
	USART_InitStructure.USART_BaudRate                   = bound;                         //���ڲ�����
	USART_InitStructure.USART_WordLength                 = USART_WordLength_8b;           //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits                   = USART_StopBits_1;              //һ��ֹͣλ
	USART_InitStructure.USART_Parity                     = USART_Parity_No;               //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl        = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode                       = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);                                             //��ʼ������
	
	
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);	 
  
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);                                        //�������ڿ���IDEL�ж�
	USART_Cmd(USART2, ENABLE);                                                            //ʹ�ܴ��� 
    
    

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                                        //�������ڽ����ж�

  //DMA_USART2_Init();                                                                    //���� DMA ����
 
	printf("USART2 OK...\r\n");
}

void DMA_USART2_Init(void)
{
  DMA_InitTypeDef DMA_USART2_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHBxPeriph_DMAy, ENABLE);                                          //����DMAʱ��
    
	//����DMA����  
	DMA_DeInit(USART2_RX_DMAx_Streamy);                                                           //DMA������
	DMA_USART2_InitStructure.DMA_Channel            = USART2_RX_DMA_Channelx;                     //DMAͨ��
	DMA_USART2_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);                    //�����ַ
	DMA_USART2_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)USART2_RX_BUF;                    //�ڴ��ַ
	DMA_USART2_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;	              //dma���䷽��
	DMA_USART2_InitStructure.DMA_BufferSize         = USART2_MAX_RX_LEN;                          //����DMA�ڴ���ʱ�������ĳ���
	DMA_USART2_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;                  //����DMA���������ģʽ��һ������
	DMA_USART2_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;	                      //����DMA���ڴ����ģʽ	
	DMA_USART2_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;                //���������ֳ�
	DMA_USART2_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;	                  //�ڴ������ֳ�
	DMA_USART2_InitStructure.DMA_Mode               = DMA_Mode_Normal;	                          //����DMA�Ĵ���ģʽ
	DMA_USART2_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;	                  //����DMA�����ȼ���	
	DMA_USART2_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;                       //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ     
	DMA_USART2_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull; 	              //ָ����FIFO��ֵˮƽ       
	DMA_USART2_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;    	              //ָ����Burstת�������ڴ洫��    
	DMA_USART2_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;                 //ָ����Burstת��������Χת��      
	DMA_Init(USART2_RX_DMAx_Streamy, &DMA_USART2_InitStructure);                                  //����DMA��ͨ��  
 
  DMA_ClearITPendingBit(USART2_RX_DMAx_Streamy,USART2_RX_DMA_IT_TCIFx);                         //�����־λ  
	DMA_Cmd(USART2_RX_DMAx_Streamy,ENABLE);                                                       //ʹ�ܽ���ͨ��
	DMA_ITConfig(USART2_RX_DMAx_Streamy,DMA_IT_TC,ENABLE);                                        //ʹ�ܷ�������ж� 
    
	NVIC_InitStructure.NVIC_IRQChannel                   = USART2_RX_DMAx_Streamy_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);                                                  //����DMA��ʽ����
}

void DMA1_Stream5_IRQHandler(void)
{
	//printf(" %d : DMA1 star ", i++);	
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TCIF1) !=RESET)   
	{  
			//printf("DMA1 over\r\n");
			DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF1);    //�����־λ    
			DMA_Cmd(DMA1_Stream5,DISABLE);                                 //�ر�DMA  
	}
		
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TCIF5) != 	RESET)//DMA_FLAG_TCIF6
	{
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5); 
		DMA_Cmd(DMA1_Stream5, DISABLE);
		DMA_ClearFlag(DMA1_Stream5,DMA_IT_TCIF5); 
		USART_DMACmd(USART2, USART_DMAReq_Rx , DISABLE); 
		//NotGet_INVCMD_count = 0;
	}
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_HTIF5) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_HTIF5); 
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_HTIF5); 
	}
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TEIF5) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TEIF5); 
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TEIF5); 
	}
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_DMEIF5) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_DMEIF5); 
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_DMEIF5); 
	}
			
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_FEIF5) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_FEIF5); 
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_FEIF5); 
	}
		
		//printf("error:  %d \r\n",DMA_GetITStatus(DMA2_Stream5,DMA_IT_TCIF5));
}

void DMA1_Stream1_IRQHandler(void)
{
	//printf(" %d : DMA1 star ", i++);	
	if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_TCIF1) !=RESET)   
	{  
			//printf("DMA1 over\r\n");
			DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TCIF1);    //�����־λ    
			DMA_Cmd(DMA1_Stream1,DISABLE);                                 //�ر�DMA  
	}
		
	if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_TCIF1) != 	RESET)//DMA_FLAG_TCIF6
	{
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TCIF1); 
		DMA_Cmd(DMA1_Stream1, DISABLE);
		DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1); 
		USART_DMACmd(USART3, USART_DMAReq_Rx , DISABLE); 
		//NotGet_INVCMD_count = 0;
	}
	if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_HTIF1) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_HTIF1); 
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_HTIF1); 
	}
	if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_TEIF1) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TEIF1); 
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TEIF1); 
	}
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_DMEIF5) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_DMEIF1); 
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_DMEIF1); 
	}
			
	if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_FEIF1) != 	RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_FEIF1); 
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_FEIF1); 
	}
		
		//printf("error:  %d \r\n",DMA_GetITStatus(DMA2_Stream5,DMA_IT_TCIF5));
}


uint8_t temp2_data = 0;
uint8_t TFmini2_low,TFmini2_high = 0;
uint8_t flag2 = 0;
uint8_t distance2 = 0;
uint8_t lastdis2 = 0;
uint8_t rx2_dis[30]= 0;
uint8_t num2 = 0;
uint16_t sum2 = 0;
int x  = 0;
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	//printf("USAART1 stay\r\n");
	uint8_t i = 0;
	//DMA_Cmd(DMA2_Stream5, DISABLE);
	//printf("error :   %d\r\n",USART_GetITStatus(USART3, USART_IT_RXNE));
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //jinbuqu
	{
		//printf("USAART1 stay\r\n");
		temp2_data = USART2->SR;
		temp2_data = USART_ReceiveData(USART2);
		//printf("data : %d", temp_data);
		if((temp2_data == 0x59) && (flag2 == 0))
		{
			flag2 = 1;
		}
		else if((temp2_data == 0x59) && (flag2 == 1))
		{
			flag2 = 2;
		}
		else if(flag2 == 2)
		{
			TFmini2_low = temp2_data;
			
			flag2 = 3;
		}
		else if(flag2 == 3)
		{

			TFmini2_high = temp2_data;
			
			if(distance2 <= 1200 && distance2 != 89 && distance2 != 86)
				lastdis2 = distance2;
			distance2 = ((uint16_t)TFmini2_high<<8) | ((uint16_t)TFmini2_low);
			if(lastdis2 >=distance2 && distance2 <= 120 && distance2 != 89 && distance2 != 86)
				rx2_dis[num2++] = distance2;
			
			if(lastdis2 >= distance2)
				lastdis2 = distance2;
			
			
			if(num2 == 8)
			{
				num2 = 0;
				for(i=0; i<8; i++)
					if(lastdis2>=rx2_dis[i])
						num2++;
				
				if(num2>=3)
				{
					printf("receive once��  %d cm \r\n",rx2_dis[i-1]-2);
					x = rx2_dis[i-1]-2;
				}				
				num2 = 0;
				//sum = 0;
				
				for(i=0; i<10; i++)
					rx2_dis[i] = 0;
			}
			/*if((lastdis-distance >= -10)||(lastdis-distance <= 10))
				printf("receive once��  %d cm \r\n",distance);
			*/
			flag2 = 0;
		}	
	}	
    USART_ClearITPendingBit(USART2,USART_FLAG_TC);        
}

uint8_t temp3_data = 0;
uint8_t TFmini3_low,TFmini3_high = 0;
uint8_t flag3 = 0;
uint8_t distance3 = 0;
uint8_t lastdis3 = 0;
uint8_t rx3_dis[30]= 0;
uint8_t num3 = 0;
uint16_t sum3 = 0;
int y = 0;
void USART3_IRQHandler(void)                	//����1�жϷ������
{
	//printf("USAART1 stay\r\n");
	uint8_t i = 0;
	//DMA_Cmd(DMA2_Stream5, DISABLE);
	//printf("error :   %d\r\n",USART_GetITStatus(USART3, USART_IT_RXNE));
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //jinbuqu
	{
		//printf("USAART1 stay\r\n");
		temp3_data = USART3->SR;
		temp3_data = USART_ReceiveData(USART3);
		//printf("data : %d", temp_data);
		if((temp3_data == 0x59) && (flag3 == 0))
		{
			flag3 = 1;
		}
		else if((temp3_data == 0x59) && (flag3 == 1))
		{
			flag3 = 2;
		}
		else if(flag3 == 2)
		{
			TFmini3_low = temp3_data;
			
			flag3 = 3;
		}
		else if(flag3 == 3)
		{

			TFmini3_high = temp3_data;
			
			if(distance3 <= 120 && distance3 != 89 && distance3 != 86)
				lastdis3 = distance3;
			distance3 = ((uint16_t)TFmini3_high<<8) | ((uint16_t)TFmini3_low);
			if(lastdis3>=distance3 && distance3 <= 120 && distance3 != 89 && distance3 != 86)
				rx3_dis[num3++] = distance3;
			
			if(lastdis3 >= distance3)
				lastdis3 = distance3;
			
			
			if(num3 == 8)
			{
				num3 = 0;
				for(i=0; i<8; i++)
					if(lastdis3>=rx3_dis[i])
						num3++;
				
				if(num3>=3)
				{
					printf("receive once��  %d cm \r\n",rx3_dis[i-1]-2);
					y = rx3_dis[i-1]-2;
				}				
				num3 = 0;
				//sum = 0;
				
				for(i=0; i<10; i++)
					rx3_dis[i] = 0;
			}
			flag3 = 0;
		}
	}
	
    USART_ClearITPendingBit(USART3,USART_FLAG_TC);        
}


