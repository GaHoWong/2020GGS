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

u8 flag_Stop=1;     //停止标志位
int Encoder[4];        //编码器的脉冲计数
int moto;           //电机PWM变量

int main(void)
{     

	
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);			          //初始化延时函数 
	//usart_init();                   //初始化串口
	usart_init();
	//uart1_init(90,115200); //与电脑端通讯
//	usmart_dev.init(90);		//初始化USMART
	LED_Init();					//初始化与LED连接的硬件接口
	
	
	
	
//	
//	SDRAM_Init();				  //初始化SDRAM 
//		TIM1_PWM_Init(59999,1499);//PWM时钟频率=90,000,000/[(59999+1)*(1499+1)] = 10Mhz (0.0001ms),设置自动装载值59999+1,预分频系数1499+1
		
		
		
		TIMx_Configuration();//定时器初始化
		
	
	
	printf("init ok");
	
	
//	LCD_Init();					//初始化LCD
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
	LED0 = 1;
	LED1 = 1;
	BREEZE_Init();
	delay_ms(200);
	BREEZE = 0;
	printf("ok");
//	TIM8_PwmSetPulse(2,60);
//	TIM8_PwmSetPulse(4,60);
//	
//	TIM1_PwmSetPulse(1,60);
//	TIM1_PwmSetPulse(3,60);

	while(1){
	//		printf("Encoder0:%d \r\n",Encoder[0]);
//		TIM8_PwmSetPulse (1,70);
		I2C_SAND_BYTE(0x41,00000,40);
		delay_ms(2000);
		I2C_SAND_BYTE(0x41,00000,80);
//				delay_ms(2000);
//		BREEZE = 0;
		
		
//		TIM8_PwmSetPulse (2,70);
//		delay_ms(3000);
//		TIM8_PwmSetPulse (3,70);
//		delay_ms(3000);
//		TIM8_PwmSetPulse (4,70);
//		delay_ms(3000);
	}
}



























