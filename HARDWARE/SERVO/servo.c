#include "servo.h"



static __IO u16 TIM12_Period = 0;	// 定时器2的重载值
static __IO u16 TIM9_Period = 0;	// 定时器2的重载值

/******************************************************************************************************
*	函 数 名: TIM12_GPIO_Config
*	入口参数: 无
*	返 回 值: 无
*	函数功能: 初始化TIM12的相关引脚
*	说    明: 
*******************************************************************************************************/

void  TIM12_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (TIM12_CH1_CLK, ENABLE); 	//IO口时钟配置


	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		// 复用模式 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	// 推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //	速度等级
	GPIO_InitStructure.GPIO_Pin = TIM12_CH1_PIN | TIM12_CH2_PIN;	 
	
	GPIO_Init(TIM12_CH1_PORT, &GPIO_InitStructure);								// 初始化 TIM12_CH1 引脚


 
	GPIO_PinAFConfig(TIM12_CH1_PORT,TIM12_CH1_PinSource,TIM12_CH1_AF);		// 设置复用
	GPIO_PinAFConfig(TIM12_CH2_PORT,TIM12_CH2_PinSource,TIM12_CH2_AF);		// 设置复用
}

/******************************************************************************************************
*	函 数 名: TIM12_PwmSetPulse
*	入口参数: TIM12_CH - 定时器通道选择，可选 TIM_CH1/2/3/4
*				 pulse - 占空比，范围 8-25是极限
*	返 回 值: 无
*	函数功能: 设置TIM12的PWM占空比
*	说    明: 
*******************************************************************************************************/

void  TIM12_PwmSetPulse (u8 TIM12_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM12_Period / 100; 	//	根据占空比设置比较值

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
*	函 数 名: TIM12_PWMinit
*	入口参数: Period - 定时器重载值，frequency - 定时器计数频率
*	返 回 值: 无
*	函数功能: 初始化 TIM12 ，并进行PWM的设置
*	说    明: 由于TIM12挂载在APB1时钟线上，所以TIM12的时钟频率为90M，定时器的分频系数在 1-65536 之间，
*            所以定时器的时钟频率范围为 90M - 1373 Hz，在实际应用中最好取能被90M整除的频率值。
*******************************************************************************************************/

void	TIM12_PWMinit(u16 period,u32 frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM12_GPIO_Config(); 	//初始化IO口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  //使能时钟
	
	TIM12_Period = period; //获取定时器的重载值，用以计算PWM值
	
	//定时器基本设置
	TIM_TimeBaseInitStructure.TIM_Period 			= period; 								// 重载值
	TIM_TimeBaseInitStructure.TIM_Prescaler		= TimerClkAPB1 / frequency - 1;  // 分频系数
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up; 				// 向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1; 						// 数字滤波采样时钟分频设置
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);					// 初始化TIM12基本设置

	//PWM输出配置
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			// PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//	使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		// 小于跳变值输出高电平
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);  			//初始化定时器比较输出通道 1
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //自动重载比较输出通道 1 的值
	
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);  			//初始化定时器比较输出通道 1
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);  //自动重载比较输出通道 1 的值
	
	TIM_ARRPreloadConfig(TIM12,ENABLE);	//	使能自动重载
	TIM_Cmd(TIM12,ENABLE); 					//	使能TIM12		
	TIM12_PwmSetPulse (1,22); 		//	关闭
	TIM12_PwmSetPulse (2,22); 		//	关闭
}


//TIM9
void  TIM9_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (TIM9_CH1_CLK, ENABLE); 	//IO口时钟配置


	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		// 复用模式 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	// 推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //	速度等级
	GPIO_InitStructure.GPIO_Pin = TIM9_CH1_PIN | TIM9_CH2_PIN;	 
	
	GPIO_Init(TIM9_CH1_PORT, &GPIO_InitStructure);								// 初始化 TIM12_CH1 引脚
	GPIO_Init(TIM9_CH2_PORT, &GPIO_InitStructure);								// 初始化 TIM12_CH1 引脚

 
	GPIO_PinAFConfig(TIM9_CH1_PORT,TIM9_CH1_PinSource,TIM9_CH1_AF);		// 设置复用
	GPIO_PinAFConfig(TIM9_CH2_PORT,TIM9_CH2_PinSource,TIM9_CH2_AF);		// 设置复用
}



/******************************************************************************************************
*	函 数 名: TIM9_PwmSetPulse
*	入口参数: TIM9_CH - 定时器通道选择，可选 TIM_CH1/2/3/4
*				 pulse - 占空比，范围 8-25是极限
*	返 回 值: 无
*	函数功能: 设置TIM9的PWM占空比
*	说    明: 
*******************************************************************************************************/

void  TIM9_PwmSetPulse (u8 TIM9_CH,u8 pulse)
{
	u16 compareValue ; 
	compareValue = pulse * TIM9_Period / 100; 	//	根据占空比设置比较值

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
*	函 数 名: TIM9_PWMinit
*	入口参数: Period - 定时器重载值，frequency - 定时器计数频率
*	返 回 值: 无
*	函数功能: 初始化 TIM9 ，并进行PWM的设置
*	说    明: 由于TIM9挂载在APB1时钟线上，所以TIM9的时钟频率为90M，定时器的分频系数在 1-65536 之间，
*            所以定时器的时钟频率范围为 90M - 1373 Hz，在实际应用中最好取能被90M整除的频率值。
*******************************************************************************************************/

void	TIM9_PWMinit(u16 period,u32 frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM9_GPIO_Config(); 	//初始化IO口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  //使能时钟
	
	TIM9_Period = period; //获取定时器的重载值，用以计算PWM值
	
	//定时器基本设置
	TIM_TimeBaseInitStructure.TIM_Period 			= period; 								// 重载值
	TIM_TimeBaseInitStructure.TIM_Prescaler		= TimerClkAPB1 / frequency - 1;  // 分频系数
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up; 				// 向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1; 						// 数字滤波采样时钟分频设置
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);					// 初始化TIM12基本设置

	//PWM输出配置
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			// PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//	使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		// 小于跳变值输出高电平
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  			//初始化定时器比较输出通道 1
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  //自动重载比较输出通道 1 的值
	
	TIM_OC2Init(TIM9, &TIM_OCInitStructure);  			//初始化定时器比较输出通道 1
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);  //自动重载比较输出通道 1 的值
	
	TIM_ARRPreloadConfig(TIM9,ENABLE);	//	使能自动重载
	TIM_Cmd(TIM9,ENABLE); 					//	使能TIM12		
	TIM9_PwmSetPulse (1,20); 		//	设置占空比为0%
	TIM9_PwmSetPulse (2,0); 		//	设置占空比为0%
}

