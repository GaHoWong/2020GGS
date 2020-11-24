#include "encoder.h"
#include "sys.h"


//void TIM5_ENC_Init(void)  // PA0  PA1   ENC_C1 ENC_C2
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʹ��TIM5ʱ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//����ٶ�Ϊ100MHz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���Ÿ���ģʽ
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©���
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);//��PA0��������ΪTIM5
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);//��PA1��������ΪTIM5
//	
//	//�������ӿ�����
//	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

//  TIM_SetCounter(TIM4,0);//����TIMx �������Ĵ���ֵ
//  TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��
//	
//	
//	TIM_Cmd(TIM5, ENABLE); 
//}



//void Encoder_Init_TIM5(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//  TIM_ICInitTypeDef TIM_ICInitStructure;  
//  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //��������
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©���
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
//  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
//  
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
//  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //�趨�������Զ���װֵ
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;////TIM���ϼ���  
//  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//	
//  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
//	
//  TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
//  TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
//  TIM_ICInit(TIM5, &TIM_ICInitStructure);//���� TIM_ICInitStruct �Ĳ�����ʼ������	TIMx
// 
//  TIM_ClearFlag(TIM5, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
//  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ���ж�
// 
//  TIM_SetCounter(TIM5,0);//����TIMx �������Ĵ���ֵ
//  TIM_Cmd(TIM5, ENABLE); //ʹ�ܶ�ʱ��
//}



/**************************************************************************
�������ܣ�MOTOR4�������Ķ�ʱ����ʼ������
��ڲ�������
����  ֵ����
���Ŷ��壺������4  		
˵    ��: A�ࡪ��>PA15  B�ࡪ��>PB3
**************************************************************************/


void TIM2_ENC_Init(void)  // PA15  PB3  ENC_D1 ENC_D2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ��TIM5ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//����ٶ�Ϊ100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���Ÿ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = 8999; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);//��PA0��������ΪTIM5
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);//��PA1��������ΪTIM5
	
	//�������ӿ�����
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM2, ENABLE); 
}


/**************************************************************************
�������ܣ�MOTOR3�������Ķ�ʱ����ʼ������
��ڲ�������
����  ֵ����
���Ŷ��壺������4  		
˵    ��: A�ࡪ��>PA6  B�ࡪ��>PA7
*************************************************************************/

void TIM3_ENC_Init(void)  // PA6   PA7  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ��TIM3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//����ٶ�Ϊ100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���Ÿ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//��PA0��������ΪTIM5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//��PA1��������ΪTIM5
	
	//�������ӿ�����
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3, ENABLE); 
}


/**************************************************************************
�������ܣ�MOTOR2�������Ķ�ʱ����ʼ������
��ڲ�������
����  ֵ����
���Ŷ��壺������4  		
˵    ��: A�ࡪ��>PB6  B�ࡪ��>PB7
*************************************************************************/
void TIM4_ENC_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIM4ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIODʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //����ٶ�Ϊ100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		     //���Ÿ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	     //��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		     //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);//��PD12��������ΪTIM4
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);//��PD13��������ΪTIM4
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//�������ӿ�����
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM4, ENABLE); 
}


/**************************************************************************
�������ܣ�MOTOR1�������Ķ�ʱ����ʼ������
��ڲ�������
����  ֵ����
���Ŷ��壺������4  		
˵    ��: A�ࡪ��>PA0  B�ࡪ��>PA1
*************************************************************************/
void TIM5_ENC_Init(void)  // PA0  PA1   ENC_C1 ENC_C2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʹ��TIM5ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//����ٶ�Ϊ100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���Ÿ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);//��PA0��������ΪTIM5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);//��PA1��������ΪTIM5
	
	//�������ӿ�����
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM5, ENABLE); 
}



void EncoderInit(void){
		
	TIM2_ENC_Init();
	TIM3_ENC_Init();
	TIM4_ENC_Init();
	TIM5_ENC_Init();


}





//u8 Read_Encoder(u8 TIMX)//��ȡ��������ֵ
//{
//  int Encoder_TIM;
//	switch(TIMX)
//	{
//	  case 2:
//					if(TIM2->CR1&0x10)              // ��ת�����ж�
//					{                             
//						Encoder_TIM= 0xffffffff - TIM2->CNT;
//						TIM2->CNT = 0xffffffff;     //��������ַ���ʱ TIMx->CNT �� -- ��
//					}else {
//						Encoder_TIM = TIM2->CNT;//��������ַ���ʱ TIMx->CNT �� ++ ��
//						TIM2->CNT = 0;
//					}
//					break;
//	  case 3:												
//					if(TIM3->CR1&0x10)           // ����Ӧ��ע�⣬ ��ͬ�Ķ�ʱ�������ֵ�ǲ�ͬ�ģ�����Ҫ����Ϊ��ʱ����λ���ǲ�ͬ��
//					{                            // TIM2 TIM5 ��32λ��ʱ�� ���ֵ��0xffffffff TIM3 TIM4��16λ��ʱ�������ֵ�� 0xffff
//					Encoder_TIM = 0xffff - TIM3->CNT;
//					TIM3->CNT = 0xffff;
//					}else{
//					Encoder_TIM = TIM3->CNT;
//					TIM3->CNT = 0;			
//					}			
//					break;
//	  case 4:
//					if(TIM4->CR1&0x10)
//					{
//						Encoder_TIM= 0xffff - TIM4->CNT;
//						TIM4->CNT = 0xffff;
//					}else {
//						Encoder_TIM = TIM4->CNT;
//						TIM4->CNT = 0;
//						}
//					break;
//		case 5:
//					if(TIM5->CR1&0x10)
//					{
//						Encoder_TIM = 0xffffffff - TIM5->CNT;
//						TIM5->CNT = 0xffffffff;
//					}else 
//					{
//						Encoder_TIM = TIM5->CNT;
//						TIM5->CNT = 0;	
//					}
//					break;
//		default: Encoder_TIM=0;
//	}
//  return Encoder_TIM;
//}


int Read_Encoder(u8 TIMX)    //��ȡ��������ֵ
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;   TIM2 -> CNT=0;  break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;   TIM3 -> CNT=0; break;	// 
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;   TIM4 -> CNT=0;  break;	
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

