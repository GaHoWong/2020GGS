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






u8 flag_Stop=1;     //ֹͣ��־λ
int  Encoder[4];        //���������������
int moto;           //���PWM����

char pro[20] = {0x41,0x46,0x3a,0x33,0x30}; //��������ģ�顪��>��������
char open1[38] = {0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
char open2[38] ={0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
char arr1[30] = {0xb5,0xbd,0xb4,0xef,0x31,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
char arr2[30] = {0xb5,0xbd,0xb4,0xef,0x32,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
char suc1[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
char suc2[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};

//�ض���printf����
int fputc(int c, FILE *fp)
{

	USART_SendData( USART1,(u8)c );	// ���͵��ֽ�����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ�������� 

	return (c); //�����ַ�
}

int main(void)
{
/***********��������************/	

	
/***********STM32�����ʼ��************/	
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	PAout(9) = 0;
	PAout(10) = 0;
	delay_init(180);			          //��ʼ����ʱ����
	uart_init(90,115200);           //����Զ˴���ͨѶ,ʹ�õ��ʱ����������к�����printfע�͵��������������
//	K210_USART(90,115200);            //��K210����ͨѶ
	I2C_Configuration();              //Ӳ��I2C��ʼ��
	
//	printf("STM32�����ʼ���ɹ���");
	
/***********Ӳ����ʼ��************/	
	MOTOR_Init();                    //��ʼ�����
//	SDRAM_Init();	       			  //��ʼ��SDRAM 		
	LED_Init();		            			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					            //��ʼ������
//	BREEZE_Init();                  //��ʼ��������
//	PLAY_Init();	                  //��ʼ����������ģ��
	OLED0561_Init();                //��ʼ��OLED��ʾ��
	EncoderInit();	                //��ʼ��������   
	OLED_SHOW();                    //oled��ʾ
	printf("Ӳ����ʼ���ɹ���");     
		
/***********Ӳ����ʼ������************/	
	
	int a = 123;
	//OLED_ShowNumber(2,0,a,3);
	
//	printf("%s", pro);              //��������
	delay_ms(200);                  //��ʱ�ȴ�
	
/***********��ʼ�����************/	
	
	LED0 = 0;                       //����LED������
	LED1 = 0;                       //����LED������˵��������ʼ���ɹ�!

//  	TFmini_Init();                   //��ʼ��TFmini PLUS
//	TIM6_Init();                    //��ʼ����ʱ6�жϣ�������������󣬷��߻����	
/***********������************/	
	delay_ms(200);

//	printf("ok");
//	TIM8_PwmSetPulse(1,62);
//	TIM8_PwmSetPulse(4,62);
//  TIM1_PwmSetPulse(1,65);
//	TIM1_PwmSetPulse(4,65);
//	TIM12_PWMinit(20000,1000000);  //50hz
//	
//	
	

	
//	Debug_USART_Config();


////////////////////�߼�///////////////////













	while(1){
		
		//printf("1");
		
//		TIM12_PwmSetPulse(1,40);
		delay_ms(200);
		

//		TIM12_PwmSetPulse(1,20);
//		delay_ms(285);
//		TIM12_PwmSetPulse(1,0);
//		delay_ms(2000);
////		TIM12_PwmSetPulse(1,10);
//		delay_ms(285);
//		TIM12_PwmSetPulse(1,0);
//		delay_ms(5000);
//		TIM12_PwmSetPulse(1,80);
//		delay_ms(1000);
		
//				TIM12_PwmSetPulse(1,0);
//		delay_ms(200);
//		TIM12_PwmSetPulse(1,50);
		//printf("%s",suc2);
//		delay_ms(5000);
//		OLED_DISPLAY_8x16(4,0,Encoder[0]);   //��ʾMOTOR1��������ֵ
//		OLED_DISPLAY_8x16(2,64,Encoder[1]);  //��ʾMOTOR1��������ֵ
//		OLED_DISPLAY_8x16(4,0,Encoder[2]);   //��ʾMOTOR1��������ֵ
//		OLED_DISPLAY_8x16(4,64,Encoder[3]);  //��ʾMOTOR1��������ֵ
		
		/*TURN_1+=5;
		if(TURN_1>180)
			TURN_1=90;
		RETURN(BEFORE,TURN_1,TURN_2,TURN_3,TURN_4);
		delay_ms(1000);*/
		//printf("12134");

//		printf("%d",Distance);		
//		TIM12_PwmSetPulse(1,led0pwmval);	
//		delay_ms(500);			
//		printf("PWM:%d\r\n",led0pwmval);//�鿴ռ�ձ�
	}

}









