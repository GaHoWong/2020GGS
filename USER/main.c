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


u8 flag_Stop=1;     //停止标志位
int  Encoder[4];        //编码器的脉冲计数
int moto;           //电机PWM变量



//extern int Distance;
//extern int Strength;

int main(void)
{     
	
	

	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);			          //初始化延时函数
//	TIMx_Configuration();//定时器初始化	
	//usart_init();                   //初始化串口
//	usart_init();
   // uart1_init(90,115200); //与电脑端通讯
//	usmart_dev.init(90);		//初始化USMART
	LED_Init();					//初始化与LED连接的硬件接口	
//	SDRAM_Init();				  //初始化SDRAM 		
		
		
	//printf("init ok");
	
//	KEY_Init();					//初始化按键
	I2C_Configuration();//I2C初始化
	
	
	IIC_Init();

	OLED0561_Init(); //OLED初始化
	OLED_DISPLAY_LIT(100);//亮度设置

	OLED_DISPLAY_PIC1();//显示全屏图片
	delay_ms(1000); //延时
	OLED_DISPLAY_CLEAR();
	OLED_DISPLAY_8x16_BUFFER(0,"      GAHO "); //显示字符串
//	OLED_DISPLAY_8x16_BUFFER(6,"  KP：KI："); //显示字符串

//	OLED_DISPLAY_16x16(2,2*16,0);
//	OLED_DISPLAY_16x16(2,3*16,1);
//	OLED_DISPLAY_16x16(2,4*16,2);
//	OLED_DISPLAY_16x16(2,5*16,3);

//	EncoderInit();
	LED0 = 0;
	LED1 = 0;
	BREEZE_Init();
	delay_ms(200);
	BREEZE = 0;
//	printf("ok");
//	TIM8_PwmSetPulse(2,60);
//	TIM8_PwmSetPulse(4,60);
////	
//  TIM1_PwmSetPulse(1,60);
//	TIM1_PwmSetPulse(3,60);
	TIM12_PWMinit(20000,1000000);
	
	
	
//	Debug_USART_Config();

	while(1){
		
		delay_ms(100);

//		printf("%d",Distance);		
//		TIM12_PwmSetPulse(1,led0pwmval);	
//		delay_ms(500);			
//		printf("PWM:%d\r\n",led0pwmval);//查看占空比
}

}









