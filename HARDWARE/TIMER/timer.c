//#include "timer.h"
//#include "sys.h"






///**************************************************************************
//�������ܣ���ʱ�жϳ�ʼ��
//��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
//����  ֵ����
//**************************************************************************/
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	
//	TIM_TimeBaseInitStruct.TIM_Period = arr;     //��װ��ֵ
//	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;  //Ԥ��Ƶϵ��
//	TIM_TimeBaseInitStruct.TIM_ClockDivision =0; //ʱ�ӷָ�
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  //ʹ�ܶ�ʱ���ж�
//	
//	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;   //ʹ���ⲿ�ж�ͨ��
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   //ʹ���ⲿ�ж�ͨ��
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�1
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;    //��Ӧ���ȼ�3
//	NVIC_Init(&NVIC_InitStruct);
//	
//	TIM_Cmd(TIM3,ENABLE);	  //ʹ�ܶ�ʱ��3
//}









////void TIM1_PWM_Init(u32 arr,u32 psc) //  �����PWM�źų�ʼ������
////{
////	GPIO_InitTypeDef 					 GPIO_InitStructure;
////	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
////	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);              //���� IO�ڵ�ʱ��
////	
////	TIM1_Period = arr; //��ȡ��ʱ��������ֵ�����Լ���PWMռ�ձ�
////	
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO���õ���ز�������
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
////  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9|          
////																 GPIO_Pin_10 | GPIO_Pin_11;         //������������4�� IO �� PWM���
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  //   һ����ʱ�� ������·
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // ռ�ձȲ�ͬ�� PWM�ź�  ����·�ź�ռ��һ����ʱ������Դ
////  GPIO_Init(GPIOA,&GPIO_InitStructure);															// ��·�źŵ�Ƶ����һ�µ�
////  
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8 ,GPIO_AF_TIM1);            // IO����
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
////  
////  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);              //������ʱ����ʱ��
////	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
////	//Period = (TIM counter clock / TIM output clock) - 1 = 10K
////  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //��ʱ��������������
////  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
////	
////  TIM_TimeBaseInitStructure.TIM_Prescaler =psc;          //Ԥ��Ƶϵ��
////  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
////  TIM_TimeBaseInitStructure.TIM_Period = arr;           //�����Զ���װ��ֵ
////  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
////  
////  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //��ʱ������� ����
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
////  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
////  
////  TIM_OC1Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��1 ��ʼ��

////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OC2Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��2 ��ʼ��
////  
////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OC3Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��3 ��ʼ��
////  
////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OC4Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��4 ��ʼ��
////  
////  TIM_Cmd(TIM1,ENABLE);
////  TIM_CtrlPWMOutputs(TIM1,ENABLE);
////}

///////******************************************************************************************************
//////*	�� �� ��: TIM1_PwmSetPulse
//////*	��ڲ���: TIM1_CH - ��ʱ��ͨ��ѡ�񣬿�ѡ TIM1_CH1/2/3/4
//////*				 pulse - ռ�ձȣ���Χ 0-100
//////*	�� �� ֵ: ��
//////*	��������: ����TIM2��PWMռ�ձ�
//////*	˵    ��: 
//////*******************************************************************************************************/

////void  TIM1_PwmSetPulse (u8 TIM1_CH,u8 pulse)
////{
////	u16 compareValue ; 
////	compareValue = pulse * TIM1_Period / 100; 	  //����ռ�ձ����ñȽ�ֵ

////	switch(TIM1_CH)
////	{
////		case 1: TIM_SetCompare1(TIM1,compareValue); break;
////		case 2: TIM_SetCompare2(TIM1,compareValue); break;
////		case 3: TIM_SetCompare3(TIM1,compareValue); break;
////		case 4: TIM_SetCompare4(TIM1,compareValue); break;		
////		default: break;
////	}
////}



///**************************************************************************
//�������ܣ�����TIM8���4·PWM
//��ڲ�����Period - ��ʱ������ֵ��frequency - ��ʱ������Ƶ��
//����  ֵ����
//���Ŷ��壺Motor3  PC6-TIM8_CH1	  PC7-TIM8_CH2
//          Motor1  PC8-TIM8_CH3	  PC9-TIM8_CH4		
//˵    ��: ����TIM8������APB2��2��Ƶ��ʱ�����ϣ�����TIM1��ʱ��Ƶ��Ϊ90M����ʱ���ķ�Ƶϵ���� 1-65536 ֮�䣬
//           ���Զ�ʱ����ʱ��Ƶ�ʷ�ΧΪ 90M - 1373 Hz����ʵ��Ӧ�������ȡ�ܱ�90M������Ƶ��ֵ��
//**************************************************************************/
//void TIM8_PWM_Init(u32 arr,u32 psc) //  �����PWM�źų�ʼ������
//{
//	GPIO_InitTypeDef 					 GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);              //���� IO�ڵ�ʱ��
//	
//	TIM8_Period = arr; //��ȡ��ʱ��������ֵ�����Լ���PWMռ�ձ�
//	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO���õ���ز�������
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7|          
//																 GPIO_Pin_8 | GPIO_Pin_9;           //������������4�� IO �� PWM���
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // һ����ʱ�� ������·
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // ռ�ձȲ�ͬ�� PWM�ź�  ����·�ź�ռ��һ����ʱ������Դ
//  GPIO_Init(GPIOA,&GPIO_InitStructure);															// ��·�źŵ�Ƶ����һ�µ�
//  
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6 ,GPIO_AF_TIM1);            // IO����
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM1);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);              //������ʱ����ʱ��APB2����tim8
//	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
//	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
//  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // �����˲�����ʱ�ӷ�Ƶ����
//  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���
//  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  // ��Ƶϵ��
//  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = arr;
//  TIM_TimeBaseInitStructure.TIM_Period = TIM8_Period;                              // ����ֵ
//  TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);
//  
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWMģʽ1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ�ܱȽ����
//  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
//  TIM_OCInitStructure.TIM_Pulse = 50;                             //����ͨ��1ռ�ձ�Ϊ0
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //С������ֵ����ߵ�ƽ
//  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//  
//	TIM_ARRPreloadConfig(TIM8,ENABLE);                           //	ʹ���Զ�����
//  TIM_OC1Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��1 ��ʼ��

//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC2Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��2 ��ʼ��
//  
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC3Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��3 ��ʼ��
//  
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC4Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��4 ��ʼ��
//  
//  TIM_Cmd(TIM8,ENABLE);
//  TIM_CtrlPWMOutputs(TIM8,ENABLE);
//}


/////******************************************************************************************************
////*	�� �� ��: TIM8_PwmSetPulse
////*	��ڲ���: TIM8_CH - ��ʱ��ͨ��ѡ�񣬿�ѡ TIM8_CH1/2/3/4
////*				    pulse - ռ�ձȣ���Χ 0-100
////*	�� �� ֵ: ��
////*	��������: ����TIM8��PWMռ�ձ�
////*	˵    ��: 
////*******************************************************************************************************/

//void  TIM8_PwmSetPulse (u8 TIM8_CH,u8 pulse)
//{
//	u16 compareValue ; 
//	compareValue = pulse * TIM8_Period / 100; 	  //����ռ�ձ����ñȽ�ֵ

//	switch(TIM8_CH)
//	{
//		case 1: TIM_SetCompare1(TIM8,compareValue); break;
//		case 2: TIM_SetCompare2(TIM8,compareValue); break;
//		case 3: TIM_SetCompare3(TIM8,compareValue); break;
//		case 4: TIM_SetCompare4(TIM8,compareValue); break;		
//		default: break;
//	}
//}







