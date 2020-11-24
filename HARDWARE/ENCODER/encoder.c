#include "encoder.h"
#include "sys.h"


//void TIM5_ENC_Init(void)  // PA0  PA1   ENC_C1 ENC_C2
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //使能TIM5时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高速度为100MHz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//引脚复用模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//开漏输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);//将PA0引脚配置为TIM5
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);//将PA1引脚配置为TIM5
//	
//	//编码器接口配置
//	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

//  TIM_SetCounter(TIM4,0);//设置TIMx 计数器寄存器值
//  TIM_Cmd(TIM4, ENABLE); //使能定时器
//	
//	
//	TIM_Cmd(TIM5, ENABLE); 
//}



//void Encoder_Init_TIM5(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//  TIM_ICInitTypeDef TIM_ICInitStructure;  
//  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//使能定时器4的时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用输入
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//开漏输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
//  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
//  
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
//  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //设定计数器自动重装值
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;////TIM向上计数  
//  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//	
//  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
//	
//  TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
//  TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
//  TIM_ICInit(TIM5, &TIM_ICInitStructure);//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
// 
//  TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM的更新标志位
//  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//使能定时器中断
// 
//  TIM_SetCounter(TIM5,0);//设置TIMx 计数器寄存器值
//  TIM_Cmd(TIM5, ENABLE); //使能定时器
//}



/**************************************************************************
函数功能：MOTOR4编码器的定时器初始化函数
入口参数：无
返回  值：无
引脚定义：编码器4  		
说    明: A相——>PA15  B相——>PB3
**************************************************************************/


void TIM2_ENC_Init(void)  // PA15  PB3  ENC_D1 ENC_D2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能TIM5时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高速度为100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//引脚复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = 8999; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);//将PA0引脚配置为TIM5
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);//将PA1引脚配置为TIM5
	
	//编码器接口配置
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM2, ENABLE); 
}


/**************************************************************************
函数功能：MOTOR3编码器的定时器初始化函数
入口参数：无
返回  值：无
引脚定义：编码器4  		
说    明: A相——>PA6  B相——>PA7
*************************************************************************/

void TIM3_ENC_Init(void)  // PA6   PA7  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能TIM3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高速度为100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//引脚复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//将PA0引脚配置为TIM5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//将PA1引脚配置为TIM5
	
	//编码器接口配置
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3, ENABLE); 
}


/**************************************************************************
函数功能：MOTOR2编码器的定时器初始化函数
入口参数：无
返回  值：无
引脚定义：编码器4  		
说    明: A相——>PB6  B相——>PB7
*************************************************************************/
void TIM4_ENC_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOD时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //最高速度为100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		     //引脚复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	     //开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		     //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);//将PD12引脚配置为TIM4
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);//将PD13引脚配置为TIM4
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//编码器接口配置
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM4, ENABLE); 
}


/**************************************************************************
函数功能：MOTOR1编码器的定时器初始化函数
入口参数：无
返回  值：无
引脚定义：编码器4  		
说    明: A相——>PA0  B相——>PA1
*************************************************************************/
void TIM5_ENC_Init(void)  // PA0  PA1   ENC_C1 ENC_C2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //使能TIM5时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高速度为100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//引脚复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);//将PA0引脚配置为TIM5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);//将PA1引脚配置为TIM5
	
	//编码器接口配置
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM5, ENABLE); 
}



void EncoderInit(void){
		
	TIM2_ENC_Init();
	TIM3_ENC_Init();
	TIM4_ENC_Init();
	TIM5_ENC_Init();


}





//u8 Read_Encoder(u8 TIMX)//读取计数器的值
//{
//  int Encoder_TIM;
//	switch(TIMX)
//	{
//	  case 2:
//					if(TIM2->CR1&0x10)              // 旋转方向判断
//					{                             
//						Encoder_TIM= 0xffffffff - TIM2->CNT;
//						TIM2->CNT = 0xffffffff;     //如果是这种方向时 TIMx->CNT 是 -- 的
//					}else {
//						Encoder_TIM = TIM2->CNT;//如果是这种方向时 TIMx->CNT 是 ++ 的
//						TIM2->CNT = 0;
//					}
//					break;
//	  case 3:												
//					if(TIM3->CR1&0x10)           // 这里应该注意， 不同的定时器的最大值是不同的，这主要是因为定时器的位数是不同的
//					{                            // TIM2 TIM5 是32位定时器 最大值是0xffffffff TIM3 TIM4是16位定时器，最大值是 0xffff
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


int Read_Encoder(u8 TIMX)    //读取计数器的值
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

