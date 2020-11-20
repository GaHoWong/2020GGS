#include "motor.h"
#include "sys.h"
#include "control.h"

static  __IO u16 TIM1_Period = 8999;	// ��ʱ��1������ֵ
static  __IO u16 TIM8_Period = 8999;	// ��ʱ��8������ֵ
#define PWM_MAX   499
#define PWM_MIN  -499

float Kp= 30,Ki=20;
float K_Speed_Move = 7,K_Speed_Turn = 5;
#define Target_Max  350

void Moter_Target_limit(short int *A,short int *B,short int *C,short int *D)
{
	short int Max,temp[4];
	u8 i;
	if(*A  > Target_Max || *B  > Target_Max || *C  > Target_Max || *D >Target_Max)  
	{//�����������Ŀ��ֵ����ʱ��Ҫ��������
		temp[0] = *A;
		temp[1] = *B;
		temp[2] = *C;
		temp[3] = *D;
		Max = temp[0];
		for(i =1 ; i < 4 ; i++) // �ҳ����ֵ
		{
			if(temp[i] > Max)
			{
				Max = temp[i];
			}
		}
		*A = temp[0] * (float)(Target_Max/Max);
		*B = temp[1] * (float)(Target_Max/Max);
		*C = temp[2] * (float)(Target_Max/Max);
		*D = temp[3] * (float)(Target_Max/Max);		
	}
}



int Moter_PWM_Limit(int PWM)
{
	if(PWM > PWM_MAX ) PWM = PWM_MAX;
	if(PWM < PWM_MIN ) PWM = PWM_MIN;
	return  PWM;
}

int Motor1_PI(int Encoder,int Target)  //������������һ��������  ��ǰ���ٶ� ��Ԥ�ڵ��ٶ�
{  
    static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 Pwm = Moter_PWM_Limit(Pwm);
	return Pwm;   // ����ֵ��һ�������������Ƶ� PWM�ź�
} 

int Motor2_PI(int Encoder,int Target)
{ 
	static int Bias,Pwm,Last_bias;
	Bias=Encoder-Target;                //����ƫ��
	Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	Last_bias=Bias;	                   //������һ��ƫ�� 
	Pwm = Moter_PWM_Limit(Pwm);
	return Pwm; 
}
int Motor3_PI(int Encoder,int Target)
{ 
	static int Bias,Pwm,Last_bias;
	Bias=Encoder-Target;                //����ƫ��
	Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	Last_bias=Bias;	                   //������һ��ƫ�� 
	Pwm = Moter_PWM_Limit(Pwm);
	return Pwm; 
}
int Motor4_PI(int Encoder,int Target)
{  
	static int Bias,Pwm,Last_bias;
	Bias=Encoder-Target;                //����ƫ��
	Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	Last_bias=Bias;	                   //������һ��ƫ�� 
	Pwm = Moter_PWM_Limit(Pwm);
	return Pwm; 
}









static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ����TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = 5000-1;       
	
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 9000-1;	
	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
	
	// �����ʱ�������жϱ�־λ
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	
	// ������ʱ�������ж�
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(BASIC_TIM, ENABLE);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	
	
	TIM1_PWM_Init(TIM1_Period,0);//��ʱ��1����>PWM��ʼ��
	TIM8_PWM_Init(TIM8_Period,0);
	TIMx_NVIC_Configuration();	
  
  TIM_Mode_Config();
}



/**************************************************************************
�������ܣ�����TIM1���4·PWM
��ڲ�����arr��װ��ֵ pscԤ��Ƶϵ��
����  ֵ����
���Ŷ��壺Motor4  PA8-TIM1_CH1	  PA9-TIM1_CH2
          Motor2  PA10-TIM1_CH3	  PA11-TIM1_CH4		
˵    ��: 1������TIM1������APB2��2��Ƶ��ʱ�����ϣ�����TIM1��ʱ��Ƶ��Ϊ90M����ʱ����Ԥ��Ƶϵ���� 1-65536 ֮�䣬
           ���Զ�ʱ����ʱ��Ƶ�ʷ�ΧΪ 90M - 1373 Hz����ʵ��Ӧ�������ȡ�ܱ�90M������Ƶ��ֵ��
					2������ʱ��Ƶ��Ϊ72Mhz������PWMƵ��Ϊ50Hz��=0.02s=20ms������ʽΪ�����ʱ��Tout����λ�룩=(arr+1)(psc+1)/Tclk	 20MS = [(59999+1)*(23+1)]/72000000
					����Ҫ����10Mhz=0.0000001s=0.0001ms
					���0.0001ms = (arr+1)(psc+1)/90Mhz = (59999+1)(1499+1)/90Mhz
					
            //PWMʱ��Ƶ��=90,000,000/[(59999+1)*(1499+1)] = 10Mhz (0.0001ms),�����Զ�װ��ֵ59999+1,Ԥ��Ƶϵ��1499+1
**************************************************************************/
void TIM1_PWM_Init(u16 arr,u16 psc) //  �����PWM�źų�ʼ������
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);              //���� IO�ڵ�ʱ��
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO���õ���ز�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;         //������������4�� IO �� PWM���
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // ʵ�ʱ���������Ҳ�������õ�  һ����ʱ�� ������·
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // ռ�ձȲ�ͬ�� PWM�ź�  ����·�ź�ռ��һ����ʱ������Դ
  GPIO_Init(GPIOA,&GPIO_InitStructure);															// ��·�źŵ�Ƶ����һ�µ�
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8 ,GPIO_AF_TIM1);            // IO����
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);              //������ʱ����ʱ��
	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //��ʱ��������������
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = arr;    
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //��ʱ������� ����
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��1 ��ʼ��

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��2 ��ʼ��
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��3 ��ʼ��
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);                        // ���ͨ��4 ��ʼ��
  
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
}





/**************************************************************************
�������ܣ�����TIM8���4·PWM
��ڲ�����Period - ��ʱ������ֵ��frequency - ��ʱ������Ƶ��
����  ֵ����
���Ŷ��壺Motor3  PC6-TIM8_CH1	  PC7-TIM8_CH2
          Motor1  PC8-TIM8_CH3	  PC9-TIM8_CH4		
˵    ��: ����TIM8������APB2��2��Ƶ��ʱ�����ϣ�����TIM1��ʱ��Ƶ��Ϊ90M����ʱ���ķ�Ƶϵ���� 1-65536 ֮�䣬
           ���Զ�ʱ����ʱ��Ƶ�ʷ�ΧΪ 90M - 1373 Hz����ʵ��Ӧ�������ȡ�ܱ�90M������Ƶ��ֵ��
**************************************************************************/
void TIM8_PWM_Init(u16 arr,u16 psc) //  �����PWM�źų�ʼ������
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);              //���� IO�ڵ�ʱ��
	
	TIM8_Period = arr; //��ȡ��ʱ��������ֵ�����Լ���PWMռ�ձ�
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO���õ���ز�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7|          
																 GPIO_Pin_8 | GPIO_Pin_9;           //������������4�� IO �� PWM���
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // һ����ʱ�� ������·
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // ռ�ձȲ�ͬ�� PWM�ź�  ����·�ź�ռ��һ����ʱ������Դ
  GPIO_Init(GPIOC,&GPIO_InitStructure);															// ��·�źŵ�Ƶ����һ�µ�
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6 ,GPIO_AF_TIM8);            // IO����
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);              //������ʱ����ʱ��APB2����tim8
	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // �����˲�����ʱ�ӷ�Ƶ����
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  // ��Ƶϵ��
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = arr;
  TIM_TimeBaseInitStructure.TIM_Period = TIM8_Period;                              // ����ֵ
  TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ�ܱȽ����
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 50;                             //����ͨ��1ռ�ձ�Ϊ0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //С������ֵ����ߵ�ƽ
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
	TIM_ARRPreloadConfig(TIM8,ENABLE);                           //	ʹ���Զ�����
  TIM_OC1Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��1 ��ʼ��

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��2 ��ʼ��
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��3 ��ʼ��
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM8,&TIM_OCInitStructure);                        // ���ͨ��4 ��ʼ��
  
  TIM_Cmd(TIM8,ENABLE);
  TIM_CtrlPWMOutputs(TIM8,ENABLE);
}


///******************************************************************************************************
//*	�� �� ��: TIM8_PwmSetPulse
//*	��ڲ���: TIM8_CH - ��ʱ��ͨ��ѡ�񣬿�ѡ TIM8_CH1/2/3/4
//*				    pulse - ռ�ձȣ���Χ 0-100
//*	�� �� ֵ: ��
//*	��������: ����TIM8��PWMռ�ձ�
//*	˵    ��: 
//*******************************************************************************************************/

void  TIM8_PwmSetPulse (u8 TIM8_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM8_Period / 100; 	  //����ռ�ձ����ñȽ�ֵ

	switch(TIM8_CH)
	{
		case 1: TIM_SetCompare1(TIM8,compareValue); break;
		case 2: TIM_SetCompare2(TIM8,compareValue); break;
		case 3: TIM_SetCompare3(TIM8,compareValue); break;
		case 4: TIM_SetCompare4(TIM8,compareValue); break;		
		default: break;
	}
}


///******************************************************************************************************
//*	�� �� ��: TIM1_PwmSetPulse
//*	��ڲ���: TIM1_CH - ��ʱ��ͨ��ѡ�񣬿�ѡ TIM1_CH1/2/3/4
//*				 pulse - ռ�ձȣ���Χ 0-100
//*	�� �� ֵ: ��
//*	��������: ����TIM2��PWMռ�ձ�
//*	˵    ��: 
//*******************************************************************************************************/

void  TIM1_PwmSetPulse (u8 TIM1_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM1_Period / 100; 	  //����ռ�ձ����ñȽ�ֵ

	switch(TIM1_CH)
	{
		case 1: TIM_SetCompare1(TIM1,compareValue); break;
		case 2: TIM_SetCompare2(TIM1,compareValue); break;
		case 3: TIM_SetCompare3(TIM1,compareValue); break;
		case 4: TIM_SetCompare4(TIM1,compareValue); break;		
		default: break;
	}
}



//int Target_velocity=50;  //�趨�ٶȿ��Ƶ�Ŀ���ٶ�Ϊ50������ÿ10ms

void  BASIC_TIM_IRQHandler (void)
{
//	extern int PWM[4],Target_Speed[4];                // �����ⲿ�ı���
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		Encoder[0]=Read_Encoder(2);                     //ȡ��ʱ��2��������ֵ 
		Encoder[1]=Read_Encoder(3);                     //ȡ��ʱ��2��������ֵ 
		Encoder[2]=Read_Encoder(4);                     //ȡ��ʱ��2��������ֵ 
		Encoder[3]=Read_Encoder(5);                     //ȡ��ʱ��2��������ֵ 
		
//		Encoder[0] = TIM_GetCounter(TIM4);	
//		printf("%d\r\n ", Encoder[0]);
//		TIM_SetCounter(TIM4,0);
//		PWM[0] =  Motor1_PI(Encoder[2],Target_Speed[0]); //  MOTOR1 �� PWM�ļ���
//		PWM[1] =  Motor2_PI(Encoder[1],Target_Speed[1]); //  MOTOR1 �� PWM�ļ���
//		PWM[2] =  Motor3_PI(Encoder[3],Target_Speed[2]); //  MOTOR1 �� PWM�ļ���
//		PWM[3] =  Motor4_PI(Encoder[0],Target_Speed[3]); //  MOTOR1 �� PWM�ļ���
//		
//		TIM8->CCR3 = PWM[0] + 500;                            //  MOTOR1 �� PWM��ֵ
//		TIM1->CCR3 = PWM[1] + 500;                            //  MOTOR2 �� PWM��ֵ
//		TIM8->CCR1 = PWM[2] + 500;                            //  MOTOR3 �� PWM��ֵ
//		TIM1->CCR1 = PWM[3] + 500;                            //  MOTOR4 �� PWM��ֵ
//		

		
    printf("Encoder0=%d \r\n Encoder1=%d \r\n Encoder2=%d \r\n Encoder3=%d \r\n",Encoder[0],Encoder[1],Encoder[2],Encoder[3]);   
	 //moto=Incremental_PI(Encoder,0);    //===λ��PID������
	 //Xianfu_Pwm();
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		 
	}		 	
}

