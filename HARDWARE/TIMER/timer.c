//#include "timer.h"
//#include "sys.h"






///**************************************************************************
//函数功能：定时中断初始化
//入口参数：arr：自动重装值  psc：时钟预分频数 
//返回  值：无
//**************************************************************************/
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	
//	TIM_TimeBaseInitStruct.TIM_Period = arr;     //重装载值
//	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;  //预分频系数
//	TIM_TimeBaseInitStruct.TIM_ClockDivision =0; //时钟分割
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  //使能定时器中断
//	
//	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;   //使能外部中断通道
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   //使能外部中断通道
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;    //响应优先级3
//	NVIC_Init(&NVIC_InitStruct);
//	
//	TIM_Cmd(TIM3,ENABLE);	  //使能定时器3
//}









////void TIM1_PWM_Init(u32 arr,u32 psc) //  电机的PWM信号初始化函数
////{
////	GPIO_InitTypeDef 					 GPIO_InitStructure;
////	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
////	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);              //开启 IO口的时钟
////	
////	TIM1_Period = arr; //获取定时器的重载值，用以计算PWM占空比
////	
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO复用的相关参数配置
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
////  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9|          
////																 GPIO_Pin_10 | GPIO_Pin_11;         //这里是配置了4个 IO 的 PWM输出
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  //   一个定时器 生成四路
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // 占空比不同的 PWM信号  这四路信号占用一个定时器的资源
////  GPIO_Init(GPIOA,&GPIO_InitStructure);															// 四路信号的频率是一致的
////  
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8 ,GPIO_AF_TIM1);            // IO复用
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
////  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
////  
////  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);              //开启定时器的时钟
////	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
////	//Period = (TIM counter clock / TIM output clock) - 1 = 10K
////  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //定时器基本参数设置
////  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
////	
////  TIM_TimeBaseInitStructure.TIM_Prescaler =psc;          //预分频系数
////  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
////  TIM_TimeBaseInitStructure.TIM_Period = arr;           //设置自动重装载值
////  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
////  
////  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器的输出 配置
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
////  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
////  
////  TIM_OC1Init(TIM1,&TIM_OCInitStructure);                        // 输出通道1 初始化

////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OC2Init(TIM1,&TIM_OCInitStructure);                        // 输出通道2 初始化
////  
////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OC3Init(TIM1,&TIM_OCInitStructure);                        // 输出通道3 初始化
////  
////  TIM_OCInitStructure.TIM_Pulse = 0;
////  TIM_OC4Init(TIM1,&TIM_OCInitStructure);                        // 输出通道4 初始化
////  
////  TIM_Cmd(TIM1,ENABLE);
////  TIM_CtrlPWMOutputs(TIM1,ENABLE);
////}

///////******************************************************************************************************
//////*	函 数 名: TIM1_PwmSetPulse
//////*	入口参数: TIM1_CH - 定时器通道选择，可选 TIM1_CH1/2/3/4
//////*				 pulse - 占空比，范围 0-100
//////*	返 回 值: 无
//////*	函数功能: 设置TIM2的PWM占空比
//////*	说    明: 
//////*******************************************************************************************************/

////void  TIM1_PwmSetPulse (u8 TIM1_CH,u8 pulse)
////{
////	u16 compareValue ; 
////	compareValue = pulse * TIM1_Period / 100; 	  //根据占空比设置比较值

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
//函数功能：设置TIM8输出4路PWM
//入口参数：Period - 定时器重载值，frequency - 定时器计数频率
//返回  值：无
//引脚定义：Motor3  PC6-TIM8_CH1	  PC7-TIM8_CH2
//          Motor1  PC8-TIM8_CH3	  PC9-TIM8_CH4		
//说    明: 由于TIM8挂载在APB2（2分频）时钟线上，所以TIM1的时钟频率为90M，定时器的分频系数在 1-65536 之间，
//           所以定时器的时钟频率范围为 90M - 1373 Hz，在实际应用中最好取能被90M整除的频率值。
//**************************************************************************/
//void TIM8_PWM_Init(u32 arr,u32 psc) //  电机的PWM信号初始化函数
//{
//	GPIO_InitTypeDef 					 GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);              //开启 IO口的时钟
//	
//	TIM8_Period = arr; //获取定时器的重载值，用以计算PWM占空比
//	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      // IO复用的相关参数配置
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7|          
//																 GPIO_Pin_8 | GPIO_Pin_9;           //这里是配置了4个 IO 的 PWM输出
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  // 一个定时器 生成四路
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                // 占空比不同的 PWM信号  这四路信号占用一个定时器的资源
//  GPIO_Init(GPIOA,&GPIO_InitStructure);															// 四路信号的频率是一致的
//  
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6 ,GPIO_AF_TIM1);            // IO复用
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM1);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);              //开启定时器的时钟APB2——tim8
//	//Timer clock = sysclock /(TIM_Prescaler+1) = 168M
//	//Period = (TIM counter clock / TIM output clock) - 1 = 20K
//  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // 数字滤波采样时钟分频设置
//  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
//  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  // 分频系数
//  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = arr;
//  TIM_TimeBaseInitStructure.TIM_Period = TIM8_Period;                              // 重载值
//  TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);
//  
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWM模式1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能比较输出
//  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
//  TIM_OCInitStructure.TIM_Pulse = 50;                             //设置通道1占空比为0
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //小于跳变值输出高电平
//  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//  
//	TIM_ARRPreloadConfig(TIM8,ENABLE);                           //	使能自动重载
//  TIM_OC1Init(TIM8,&TIM_OCInitStructure);                        // 输出通道1 初始化

//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC2Init(TIM8,&TIM_OCInitStructure);                        // 输出通道2 初始化
//  
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC3Init(TIM8,&TIM_OCInitStructure);                        // 输出通道3 初始化
//  
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC4Init(TIM8,&TIM_OCInitStructure);                        // 输出通道4 初始化
//  
//  TIM_Cmd(TIM8,ENABLE);
//  TIM_CtrlPWMOutputs(TIM8,ENABLE);
//}


/////******************************************************************************************************
////*	函 数 名: TIM8_PwmSetPulse
////*	入口参数: TIM8_CH - 定时器通道选择，可选 TIM8_CH1/2/3/4
////*				    pulse - 占空比，范围 0-100
////*	返 回 值: 无
////*	函数功能: 设置TIM8的PWM占空比
////*	说    明: 
////*******************************************************************************************************/

//void  TIM8_PwmSetPulse (u8 TIM8_CH,u8 pulse)
//{
//	u16 compareValue ; 
//	compareValue = pulse * TIM8_Period / 100; 	  //根据占空比设置比较值

//	switch(TIM8_CH)
//	{
//		case 1: TIM_SetCompare1(TIM8,compareValue); break;
//		case 2: TIM_SetCompare2(TIM8,compareValue); break;
//		case 3: TIM_SetCompare3(TIM8,compareValue); break;
//		case 4: TIM_SetCompare4(TIM8,compareValue); break;		
//		default: break;
//	}
//}







