#include "motor.h"
#include "sys.h"
#include "control.h"
#include "oled.h"
#include "stdio.h"



static  __IO u16 TIM1_Period = 8999;	// 定时器1的重载值
static  __IO u16 TIM8_Period = 8999;	// 定时器8的重载值
#define PWM_MAX   5400
#define PWM_MIN  -5400  //将PWM最大占空比设置为60

extern float Kp,Ki;
float Kp= 3,Ki=2;
float K_Speed_Move = 7,K_Speed_Turn = 5;
#define Target_Max  350

void Moter_Target_limit(short int *A,short int *B,short int *C,short int *D)
{
	short int Max,temp[4];
	u8 i;
	if(*A  > Target_Max || *B  > Target_Max || *C  > Target_Max || *D >Target_Max)  
	{//当计算出来的目标值过大时需要进行限制
		temp[0] = *A;
		temp[1] = *B;
		temp[2] = *C;
		temp[3] = *D;
		Max = temp[0];
		for(i =1 ; i < 4 ; i++) // 找出最大值
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
	if(PWM < PWM_MIN ) PWM = PWM_MAX;
	return  PWM;
}

int Motor1_PI(int Encoder,int Target)  //这里的输入参数一共是两个  当前的速度 和预期的速度
{  
    static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 Pwm = Moter_PWM_Limit(Pwm);
	return Pwm;   // 返回值是一个经过幅度限制的 PWM信号
} 

int Motor2_PI(int Encoder,int Target)
{ 
	static int Bias,Pwm,Last_bias;
	Bias=Encoder-Target;                //计算偏差
	Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	Last_bias=Bias;	                   //保存上一次偏差 
	Pwm = Moter_PWM_Limit(Pwm);
	return Pwm; 
}
int Motor3_PI(int Encoder,int Target)
{ 
	static int Bias,Pwm,Last_bias;
	Bias=Encoder-Target;                //计算偏差
	Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	Last_bias=Bias;	                   //保存上一次偏差 
	Pwm = Moter_PWM_Limit(Pwm);
	return Pwm; 
}
int Motor4_PI(int Encoder,int Target)
{  
	static int Bias,Pwm,Last_bias;
	Bias=Encoder-Target;                //计算偏差
	Pwm-=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	Last_bias=Bias;	                   //保存上一次偏差 
	Pwm = Moter_PWM_Limit(Pwm);
	return Pwm; 
}


////平衡小车之家的代码




/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}







static void TIM6_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM6_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到4999，即为5000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 5000-1;       
	
	//定时器时钟源TIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 9000-1;	
	
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
	
	// 清除定时器更新中断标志位
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	
	// 开启定时器更新中断
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	
	// 使能定时器
	TIM_Cmd(BASIC_TIM, ENABLE);	
}

/**
  * @brief  初始化基本定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIM6_Init(void)
{
	TIM6_NVIC_Configuration();	
	TIM6_Mode_Config();
}



/**************************************************************************
函数功能：设置TIM1输出4路PWM
入口参数：arr重装载值 psc预分频系数
返回  值：无
引脚定义：Motor4  PA8-TIM1_CH1	  PA9-TIM1_CH2
          Motor2  PA10-TIM1_CH3	  PA11-TIM1_CH4		
说    明: 1、由于TIM1挂载在APB2（2分频）时钟线上，所以TIM1的时钟频率为90M，定时器的预分频系数在 1-65536 之间，
           所以定时器的时钟频率范围为 90M - 1373 Hz，在实际应用中最好取能被90M整除的频率值。
					2、假如时钟频率为72Mhz，设置PWM频率为50Hz（=0.02s=20ms），公式为：溢出时间Tout（单位秒）=(arr+1)(psc+1)/Tclk	 20MS = [(59999+1)*(23+1)]/72000000
					假如要设置10Mhz=0.0000001s=0.0001ms
					因此0.0001ms = (arr+1)(psc+1)/90Mhz = (59999+1)(1499+1)/90Mhz
					
            //PWM时钟频率=90,000,000/[(59999+1)*(1499+1)] = 10Mhz (0.0001ms),设置自动装载值59999+1,预分频系数1499+1
**************************************************************************/
void TIM1_PWM_Init(u16 arr,u16 psc) //  电机的PWM信号初始化函数
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);              //开启 IO口的时钟
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO复用的相关参数配置
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;         //这里是配置了4个 IO 的 PWM输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // 实际比赛中我们也是这样用的  一个定时器 生成四路
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // 占空比不同的 PWM信号  这四路信号占用一个定时器的资源
  GPIO_Init(GPIOA,&GPIO_InitStructure);															// 四路信号的频率是一致的
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8 ,GPIO_AF_TIM1);            // IO复用
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);              //开启定时器的时钟
	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //定时器基本参数设置
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = arr;    
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器的输出 配置
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 50;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);                        // 输出通道1 初始化

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);                        // 输出通道2 初始化
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);                        // 输出通道3 初始化
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);                        // 输出通道4 初始化
  
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
}





/**************************************************************************
函数功能：设置TIM8输出4路PWM
入口参数：Period - 定时器重载值，frequency - 定时器计数频率
返回  值：无
引脚定义：Motor3  PC6-TIM8_CH1	  PC7-TIM8_CH2
          Motor1  PC8-TIM8_CH3	  PC9-TIM8_CH4		
说    明: 由于TIM8挂载在APB2（2分频）时钟线上，所以TIM1的时钟频率为90M，定时器的分频系数在 1-65536 之间，
           所以定时器的时钟频率范围为 90M - 1373 Hz，在实际应用中最好取能被90M整除的频率值。
**************************************************************************/
void TIM8_PWM_Init(u16 arr,u16 psc) //  电机的PWM信号初始化函数
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);              //开启 IO口的时钟
	
	TIM8_Period = arr; //获取定时器的重载值，用以计算PWM占空比
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO复用的相关参数配置
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7|          
																 GPIO_Pin_8 | GPIO_Pin_9;           //这里是配置了4个 IO 的 PWM输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // 一个定时器 生成四路
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // 占空比不同的 PWM信号  这四路信号占用一个定时器的资源
  GPIO_Init(GPIOC,&GPIO_InitStructure);															// 四路信号的频率是一致的
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6 ,GPIO_AF_TIM8);            // IO复用
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);              //开启定时器的时钟APB2——tim8
	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // 数字滤波采样时钟分频设置
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  // 分频系数
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = arr;
  TIM_TimeBaseInitStructure.TIM_Period = TIM8_Period;                              // 重载值
  TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能比较输出
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0;                             //设置通道1占空比为0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //小于跳变值输出高电平
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
	TIM_ARRPreloadConfig(TIM8,ENABLE);                           //	使能自动重载
  TIM_OC1Init(TIM8,&TIM_OCInitStructure);                        // 输出通道1 初始化

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM8,&TIM_OCInitStructure);                        // 输出通道2 初始化
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM8,&TIM_OCInitStructure);                        // 输出通道3 初始化
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM8,&TIM_OCInitStructure);                        // 输出通道4 初始化
  
  TIM_Cmd(TIM8,ENABLE);
  TIM_CtrlPWMOutputs(TIM8,ENABLE);
}


///******************************************************************************************************
//*	函 数 名: TIM8_PwmSetPulse
//*	入口参数: TIM8_CH - 定时器通道选择，可选 TIM8_CH1/2/3/4
//*				    pulse - 占空比，范围 0-100
//*	返 回 值: 无
//*	函数功能: 设置TIM8的PWM占空比
//*	说    明: 
//*******************************************************************************************************/

void  TIM8_PwmSetPulse (u8 TIM8_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM8_Period / 100; 	  //根据占空比设置比较值

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
//*	函 数 名: TIM1_PwmSetPulse
//*	入口参数: TIM1_CH - 定时器通道选择，可选 TIM1_CH1/2/3/4
//*				 pulse - 占空比，范围 0-100
//*	返 回 值: 无
//*	函数功能: 设置TIM2的PWM占空比
//*	说    明: 
//*******************************************************************************************************/

void  TIM1_PwmSetPulse (u8 TIM1_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM1_Period / 100; 	  //根据占空比设置比较值

	switch(TIM1_CH)
	{
		case 1: TIM_SetCompare1(TIM1,compareValue); break;
		case 2: TIM_SetCompare2(TIM1,compareValue); break;
		case 3: TIM_SetCompare3(TIM1,compareValue); break;
		case 4: TIM_SetCompare4(TIM1,compareValue); break;		
		default: break;
	}
}


//int Target_velocity=50;  //设定速度控制的目标速度为50个脉冲每10ms
/*
void  BASIC_TIM_IRQHandler (void)
{	
	
	int PWM[4],Target_Speed[4];              // 引入外部的变量
		
		Target_Speed[0] = 5000;
		Target_Speed[1] = 5000;
		Target_Speed[2] = 5000;
		Target_Speed[3] = 5000;
		
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	

		                                                //显示MOTOR1编码器数值
//		OLED_DISPLAY_8x16_BUFFER(4,64,Read_Encoder(3)); //显示MOTOR2编码器数值
//		OLED_DISPLAY_8x16_BUFFER(6,0,Read_Encoder(4)); //显示MOTOR3编码器数值
//		OLED_DISPLAY_8x16_BUFFER(6,64,Read_Encoder(5)); //显示MOTOR4编码器数值
		
		
//		Encoder[0]=Read_Encoder(5);                     //取定时器2计数器的值 
//		Encoder[1]=Read_Encoder(4);                     //取定时器2计数器的值 
//		Encoder[2]=Read_Encoder(3);                     //取定时器2计数器的值 
//		Encoder[3]=Read_Encoder(2);                     //取定时器2计数器的值 
		
	//	OLED_ShowNumber(4,0,123,3,12);

		
//		Encoder[0] = TIM_GetCounter(TIM4);	
//		printf("%d\r\n ", Encoder[0]);
//		TIM_SetCounter(TIM4,0);
//	   PWM[0] = Incremental_PI(Encoder[0 ],5000);
//		PWM[1] = Incremental_PI(Encoder[0],5000);
//		PWM[2] = Incremental_PI(Encoder[0],5000);
//		PWM[3] = Incremental_PI(Encoder[0],5000);
//		Moter_PWM_Limit(PWM[0]);
//		Moter_PWM_Limit(PWM[1]);
//		Moter_PWM_Limit(PWM[2]);
//		Moter_PWM_Limit(PWM[3]);

		
		
//		PWM[0] =  Motor1_PI(Encoder[0],Target_Speed[0]); //  MOTOR1 的 PWM的计算
//		PWM[1] =  Motor2_PI(Encoder[1],Target_Speed[1]); //  MOTOR2 的 PWM的计算
//		PWM[2] =  Motor3_PI(Encoder[2],Target_Speed[2]); //  MOTOR3 的 PWM的计算
//		PWM[3] =  Motor4_PI(Encoder[3],Target_Speed[3]); //  MOTOR4 的 PWM的计算
		
//		TIM8->CCR3 = PWM[0]-500;                            //  MOTOR1 的 PWM赋值
//		TIM1->CCR4 = PWM[1]-500;                            //  MOTOR2 的 PWM赋值
//		TIM8->CCR1 = PWM[2]-500;                            //  MOTOR3 的 PWM赋值
//		TIM1->CCR2 = PWM[3]-500;                            //  MOTOR4 的 PWM赋值
		
		
		
		
//		OLED_DISPLAY_8x16(4,0*8,(u8)Encoder[0]/10000%10+0x30);
//		OLED_DISPLAY_8x16(4,1*8,(u8)Encoder[0]/1000%10+0x30);
//		OLED_DISPLAY_8x16(4,2*8,(u8)Encoder[0]/100%10+0x30);
//		OLED_DISPLAY_8x16(4,3*8,(u8)Encoder[0]/10%10+0x30);
//		OLED_DISPLAY_8x16(4,4*8,(u8)Encoder[0]%10+0x30);

//		OLED_DISPLAY_8x16(4,7*8,(u8)Encoder[1]/10000%10+0x30);
//		OLED_DISPLAY_8x16(4,8*8,(u8)Encoder[1]/1000%10+0x30);
//		OLED_DISPLAY_8x16(4,9*8,(u8)Encoder[1]/100%10+0x30);
//		OLED_DISPLAY_8x16(4,10*8,(u8)Encoder[1]/10%10+0x30);
//		OLED_DISPLAY_8x16(4,11*8,(u8)Encoder[1]%10+0x30);

//		OLED_DISPLAY_8x16(6,0*8,(u8)Encoder[2]/10000%10+0x30);
//		OLED_DISPLAY_8x16(6,1*8,(u8)Encoder[2]/1000%10+0x30);
//		OLED_DISPLAY_8x16(6,2*8,(u8)Encoder[2]/100%10+0x30);
//		OLED_DISPLAY_8x16(6,3*8,(u8)Encoder[2]/10%10+0x30);
//		OLED_DISPLAY_8x16(6,4*8,(u8)Encoder[2]%10+0x30);

//		OLED_DISPLAY_8x16(6,7*8,(u8)Encoder[3]/10000%10+0x30);
//		OLED_DISPLAY_8x16(6,8*8,(u8)Encoder[3]/1000%10+0x30);
//		OLED_DISPLAY_8x16(6,9*8,(u8)Encoder[3]/100%10+0x30);
//		OLED_DISPLAY_8x16(6,10*8,(u8)Encoder[3]/10%10+0x30);
//		OLED_DISPLAY_8x16(6,11*8,(u8)Encoder[3]%10+0x30);




		//printf("PWM0=%d \r\n PWM1=%d \r\n PWM2=%d \r\n PWM3=%d \r\n",PWM[0],PWM[1],PWM[2],PWM[3]);
			TIM8_PwmSetPulse(1,58);		//motor3,Encoder[2]
			TIM8_PwmSetPulse(3,58);
			TIM1_PwmSetPulse(2,64);
			TIM1_PwmSetPulse(4,64);
    //  printf("Encoder0=%d \r\n Encoder1=%d \r\n Encoder2=%d \r\n Encoder3=%d \r\n",Encoder[0],Encoder[1],Encoder[2],Encoder[3]);   
	 //moto=Incremental_PI(Encoder,0);    //===位置PID控制器
	 //Xianfu_Pwm();
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		 
	}		 	
}

*/
void MOTOR_Init(void){
	TIM1_PWM_Init(TIM1_Period,0);//定时器1——>PWM初始化
	TIM8_PWM_Init(TIM8_Period,0);
}






/**************************************************************************
函数功能：小车运动数学模型
入口参数：X Y 速度或者位置
返回  值：无
**************************************************************************/
//void GO_TO(int sign,int sign_x,int sign_y)
//{

//		Moter_Target_limit(temp,temp+1,temp+2,temp+3);	
//    TIM_Cmd(TIM7,DISABLE);
//		Target_A = temp[0];
//		Target_B = temp[1];
//		Target_C = temp[2];
//		Target_D = temp[3];
//    TIM_Cmd(TIM7,ENABLE);
//}

