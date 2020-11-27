#include "servo.h"



static __IO u16 TIM12_Period = 0;	// ��ʱ��2������ֵ
static __IO u16 TIM9_Period = 0;	// ��ʱ��2������ֵ

/******************************************************************************************************
*	�� �� ��: TIM12_GPIO_Config
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ��ʼ��TIM12���������
*	˵    ��: 
*******************************************************************************************************/

void  TIM12_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (TIM12_CH1_CLK, ENABLE); 	//IO��ʱ������


	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		// ����ģʽ 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	// ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		// ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //	�ٶȵȼ�
	GPIO_InitStructure.GPIO_Pin = TIM12_CH1_PIN | TIM12_CH2_PIN;	 
	
	GPIO_Init(TIM12_CH1_PORT, &GPIO_InitStructure);								// ��ʼ�� TIM12_CH1 ����


 
	GPIO_PinAFConfig(TIM12_CH1_PORT,TIM12_CH1_PinSource,TIM12_CH1_AF);		// ���ø���
	GPIO_PinAFConfig(TIM12_CH2_PORT,TIM12_CH2_PinSource,TIM12_CH2_AF);		// ���ø���
}

/******************************************************************************************************
*	�� �� ��: TIM12_PwmSetPulse
*	��ڲ���: TIM12_CH - ��ʱ��ͨ��ѡ�񣬿�ѡ TIM_CH1/2/3/4
*				 pulse - ռ�ձȣ���Χ 8-25�Ǽ���
*	�� �� ֵ: ��
*	��������: ����TIM12��PWMռ�ձ�
*	˵    ��: 
*******************************************************************************************************/

void  TIM12_PwmSetPulse (u8 TIM12_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM12_Period / 100; 	//	����ռ�ձ����ñȽ�ֵ

	switch(TIM12_CH)
	{
		case 1: TIM_SetCompare1(TIM12,compareValue); break;
		case 2: TIM_SetCompare2(TIM12,compareValue); break;
		case 3: TIM_SetCompare3(TIM12,compareValue); break;
		case 4: TIM_SetCompare4(TIM12,compareValue); break;		
		default: break;
	}
}

/******************************************************************************************************
*	�� �� ��: TIM12_PWMinit
*	��ڲ���: Period - ��ʱ������ֵ��frequency - ��ʱ������Ƶ��
*	�� �� ֵ: ��
*	��������: ��ʼ�� TIM12 ��������PWM������
*	˵    ��: ����TIM12������APB1ʱ�����ϣ�����TIM12��ʱ��Ƶ��Ϊ90M����ʱ���ķ�Ƶϵ���� 1-65536 ֮�䣬
*            ���Զ�ʱ����ʱ��Ƶ�ʷ�ΧΪ 90M - 1373 Hz����ʵ��Ӧ�������ȡ�ܱ�90M������Ƶ��ֵ��
*******************************************************************************************************/

void	TIM12_PWMinit(u16 period,u32 frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM12_GPIO_Config(); 	//��ʼ��IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  //ʹ��ʱ��
	
	TIM12_Period = period; //��ȡ��ʱ��������ֵ�����Լ���PWMֵ
	
	//��ʱ����������
	TIM_TimeBaseInitStructure.TIM_Period 			= period; 								// ����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler		= TimerClkAPB1 / frequency - 1;  // ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up; 				// ���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1; 						// �����˲�����ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);					// ��ʼ��TIM12��������

	//PWM�������
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			// PWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//	ʹ�ܱȽ����
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		// С������ֵ����ߵ�ƽ
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);  			//��ʼ����ʱ���Ƚ����ͨ�� 1
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //�Զ����رȽ����ͨ�� 1 ��ֵ
	
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);  			//��ʼ����ʱ���Ƚ����ͨ�� 1
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);  //�Զ����رȽ����ͨ�� 1 ��ֵ
	
	TIM_ARRPreloadConfig(TIM12,ENABLE);	//	ʹ���Զ�����
	TIM_Cmd(TIM12,ENABLE); 					//	ʹ��TIM12		
	TIM12_PwmSetPulse (1,22); 		//	�ر�
	TIM12_PwmSetPulse (2,22); 		//	�ر�
}


//TIM9
void  TIM9_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (TIM9_CH1_CLK, ENABLE); 	//IO��ʱ������


	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		// ����ģʽ 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	// ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		// ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //	�ٶȵȼ�
	GPIO_InitStructure.GPIO_Pin = TIM9_CH1_PIN | TIM9_CH2_PIN;	 
	
	GPIO_Init(TIM9_CH1_PORT, &GPIO_InitStructure);								// ��ʼ�� TIM12_CH1 ����
	GPIO_Init(TIM9_CH2_PORT, &GPIO_InitStructure);								// ��ʼ�� TIM12_CH1 ����

 
	GPIO_PinAFConfig(TIM9_CH1_PORT,TIM9_CH1_PinSource,TIM9_CH1_AF);		// ���ø���
	GPIO_PinAFConfig(TIM9_CH2_PORT,TIM9_CH2_PinSource,TIM9_CH2_AF);		// ���ø���
}



/******************************************************************************************************
*	�� �� ��: TIM9_PwmSetPulse
*	��ڲ���: TIM9_CH - ��ʱ��ͨ��ѡ�񣬿�ѡ TIM_CH1/2/3/4
*				 pulse - ռ�ձȣ���Χ 8-25�Ǽ���
*	�� �� ֵ: ��
*	��������: ����TIM9��PWMռ�ձ�
*	˵    ��: 
*******************************************************************************************************/

void  TIM9_PwmSetPulse (u8 TIM9_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM9_Period / 100; 	//	����ռ�ձ����ñȽ�ֵ

	switch(TIM9_CH)
	{
		case 1: TIM_SetCompare1(TIM9,compareValue); break;
		case 2: TIM_SetCompare2(TIM9,compareValue); break;
		case 3: TIM_SetCompare3(TIM9,compareValue); break;
		case 4: TIM_SetCompare4(TIM9,compareValue); break;		
		default: break;
	}
}

/******************************************************************************************************
*	�� �� ��: TIM9_PWMinit
*	��ڲ���: Period - ��ʱ������ֵ��frequency - ��ʱ������Ƶ��
*	�� �� ֵ: ��
*	��������: ��ʼ�� TIM9 ��������PWM������
*	˵    ��: ����TIM9������APB1ʱ�����ϣ�����TIM9��ʱ��Ƶ��Ϊ90M����ʱ���ķ�Ƶϵ���� 1-65536 ֮�䣬
*            ���Զ�ʱ����ʱ��Ƶ�ʷ�ΧΪ 90M - 1373 Hz����ʵ��Ӧ�������ȡ�ܱ�90M������Ƶ��ֵ��
*******************************************************************************************************/

void	TIM9_PWMinit(u16 period,u32 frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM9_GPIO_Config(); 	//��ʼ��IO��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  //ʹ��ʱ��
	
	TIM9_Period = period; //��ȡ��ʱ��������ֵ�����Լ���PWMֵ
	
	//��ʱ����������
	TIM_TimeBaseInitStructure.TIM_Period 			= period; 								// ����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler		= TimerClkAPB1 / frequency - 1;  // ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up; 				// ���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1; 						// �����˲�����ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);					// ��ʼ��TIM12��������

	//PWM�������
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			// PWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//	ʹ�ܱȽ����
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		// С������ֵ����ߵ�ƽ
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  			//��ʼ����ʱ���Ƚ����ͨ�� 1
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  //�Զ����رȽ����ͨ�� 1 ��ֵ
	
	TIM_OC2Init(TIM9, &TIM_OCInitStructure);  			//��ʼ����ʱ���Ƚ����ͨ�� 1
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);  //�Զ����رȽ����ͨ�� 1 ��ֵ
	
	TIM_ARRPreloadConfig(TIM9,ENABLE);	//	ʹ���Զ�����
	TIM_Cmd(TIM9,ENABLE); 					//	ʹ��TIM12		
	TIM9_PwmSetPulse (1,20); 		//	����ռ�ձ�Ϊ0%
	TIM9_PwmSetPulse (2,0); 		//	����ռ�ձ�Ϊ0%
}

