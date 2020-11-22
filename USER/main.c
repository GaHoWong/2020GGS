#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
 
#include "sdram.h"   
#include "usmart.h" 
#include "key.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "oled.h"

#include "encoder.h"
#include "timer.h"
#include "motor.h" 
#include "breeze.h"
#include "servo.h"
#include "tfmini.h"  
#include "play.h"
#include "pca9685.h"

u8 flag_Stop=1;     //停止标志位
int  Encoder[4];        //编码器的脉冲计数
int moto;           //电机PWM变量

//重定向printf函数
int fputc(int c, FILE *fp)
{

	USART_SendData( USART1,(u8)c );	// 发送单字节数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完毕 

	return (c); //返回字符
}

int main(void)
{
/***********变量定义************/	
	char pro[20] = {0x41,0x46,0x3a,0x33,0x30}; //语音播报模块——>增大声音
	char open1[38] = {0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
	char open2[38] ={0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
	char arr1[30] = {0xb5,0xbd,0xb4,0xef,0x31,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
	char arr2[30] = {0xb5,0xbd,0xb4,0xef,0x32,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
	char suc1[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
	char suc2[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
	
/***********STM32外设初始化************/	
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);			          //初始化延时函数
	uart_init(90,115200);           //与电脑端串口通讯
	I2C_Configuration();            //硬件I2C初始化
	
	printf("STM32外设初始化成功！");
	
/***********硬件初始化************/	
//	SDRAM_Init();	       			  //初始化SDRAM 		
	LED_Init();		            			//初始化与LED连接的硬件接口
	KEY_Init();					            //初始化按键
	BREEZE_Init();                  //初始化蜂鸣器
	PLAY_Init();	                  //初始化语音播报模块
	EncoderInit();	                //初始化编码器   
	OLED0561_Init();                //初始化OLED显示屏

	//TIMx_Configuration();         //初始化电机PWM与时钟6定时中断	
	
	delay_ms(200);                  //延时等待
	BREEZE = 0;                     //硬件初始成功蜂鸣器响一声
	printf("硬件初始化成功！");     
		
/***********硬件初始化设置************/	
	OLED_DISPLAY_LIT(100);          //OLED亮度设置
	OLED_DISPLAY_PIC1();            //显示全屏图片
	delay_ms(1000);                 //延时（必须）
	OLED_DISPLAY_CLEAR();           //清屏
	OLED_DISPLAY_8x16_BUFFER(0,"      GAHO "); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(6,"  KP：KI："); //显示字符串
	printf("%s", pro);              //增大音量
	delay_ms(200);                  //延时等待
	
/***********初始化完成************/		
	LED0 = 0;                       //板载LED灯亮起
	LED1 = 0;                       //板载LED灯亮起，说明整个初始化成功!



/***********调试区************/	


//	OLED_DISPLAY_16x16(2,2*16,0);
//	OLED_DISPLAY_16x16(2,3*16,1);
//	OLED_DISPLAY_16x16(2,4*16,2);
//	OLED_DISPLAY_16x16(2,5*16,3);
	delay_ms(200);

//	printf("ok");
//	TIM8_PwmSetPulse(1,50);
//	TIM8_PwmSetPulse(3,50);
////	
//  TIM1_PwmSetPulse(1,60);
//	TIM1_PwmSetPulse(3,60);
//	TIM124_PWMinit(20000,1000000);
	
	
	
//	Debug_USART_Config();
	/*IIC_Init();
	PCA9685_write(PCA9685_MODE1,0x0);
	setPWMFreq(50);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);
	up();
	TURN_1=90;
	TURN_2=90;
	TURN_3=90;
	TURN_4=40;	
	BEFORE[0]=TURN_1;
	BEFORE[1]=TURN_2;
	BEFORE[2]=TURN_3;
	BEFORE[3]=TURN_4;
	RETURN(BEFORE,TURN_1,TURN_2,TURN_3,TURN_4); */





	while(1){
		delay_ms(1000);
		//printf("%s",suc2);
		delay_ms(5000);
		/*TURN_1+=5;
		if(TURN_1>180)
			TURN_1=90;
		RETURN(BEFORE,TURN_1,TURN_2,TURN_3,TURN_4);
		delay_ms(1000);*/
		//printf("12134");

//		printf("%d",Distance);		
//		TIM12_PwmSetPulse(1,led0pwmval);	
//		delay_ms(500);			
//		printf("PWM:%d\r\n",led0pwmval);//查看占空比
	}

}









