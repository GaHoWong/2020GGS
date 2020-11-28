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
#include "analyse.h"



int X ;//�����״�X
int Y;//�����״�Y

int sign = 0;
int sign_x = 0 ;
int sign_y = 0;

u8 flag_Stop=1;      //ֹͣ��־λ
int  Encoder[4];     //���������������
int moto;            //���PWM����
int task_flag;       //�����־��1 = 11��2 = 12��3 = 21��4 = 22
int wuliao_flag;     //���ϱ�־�����Ϊ1��һ�����ϣ�2Ϊ��������
int dir_flag;	      //�����־�����Ϊ0����ת��Ϊ1����ת   



int task_flag = 0;      //�����־1Ϊ1�ŵص㡣2Ϊ2��
int go_flag = 0;			//��ת��־
int go_back = 0;  	//���Ϊ1�����߻�����Ϊ2����ұ߻���
int safe_x = 80;		//x����İ�ȫ��Χ
int safe_y = 60; 		//y����İ�ȫ��Χ
int wuliao = 0;    	//����
int didian = 0;		//�ص�
int arr_flag = 0;







char pro[20] = {0x41,0x46,0x3a,0x33,0x30}; //��������ģ�顪��>��������
//ɨ����ɣ���װ��1�����ϣ����رղ���
char open1[38] = {0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
//ɨ����ɣ���װ��2�����ϣ����رղ���
char open2[38] ={0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
//����1���ջ��㣬���ʾ�����
char arr1[30] = {0xb5,0xbd,0xb4,0xef,0x31,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
//����2���ջ��㣬���ʾ�����
char arr2[30] = {0xb5,0xbd,0xb4,0xef,0x32,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
//��ȡ��1�����ϣ����رղ���
char close1[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
//��ȡ��2�����ϣ����رղ���
char close2[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};

//�ض���printf����
int fputc(int c, FILE *fp)//��������ģ��ΪUART7
{

	USART_SendData(UART7,(u8)c );	// ���͵��ֽ�����
	while (USART_GetFlagStatus(UART7, USART_FLAG_TXE) == RESET);	//�ȴ�������� 

	return (c); //�����ַ�
}

int main(void)
{
/***********��������************/	

	
/***********STM32�����ʼ��************/	
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz

	delay_init(180);			          //��ʼ����ʱ����
//	uart_init(90,115200);             //����Զ˴���ͨѶ,ʹ�õ��ʱ����������к�����printfע�͵��������������
	PAout(9) = 0;
	PAout(10) = 0;
//	K210_USART(90,115200);            //��K210����ͨѶ
	USART6_Init(115200);              //��OPEN MV����ͨѶ
	I2C_Configuration();              //Ӳ��I2C��ʼ��
	
//	printf("STM32�����ʼ���ɹ���");
	
/***********Ӳ����ʼ��************/	
	MOTOR_Init();                    //��ʼ�����
	TIM12_PWMinit(20000,1000000);    //50hz
//	TIM9_PWMinit(20000,1000000);     //50hz
//	SDRAM_Init();	       			   //��ʼ��SDRAM 		
	LED_Init();		            			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					            //��ʼ������
	BREEZE_Init();                  //��ʼ��������
	PLAY_Init();	                  //��ʼ����������ģ��
//	OLED0561_Init();                 //��ʼ��OLED��ʾ��
	//EncoderInit();	                  //��ʼ��������   
	//OLED_SHOW();                     //oled��ʾ  
		
/***********Ӳ����ʼ������************/	
	
delay_ms(2000);
	printf("%s", pro);              //��������
	delay_ms(200);                  //��ʱ�ȴ�
	
/***********��ʼ�����************/	
	
	LED0 = 0;                       //����LED������
	LED1 = 0;                       //����LED������˵��������ʼ���ɹ�!

 // 	TFmini_Init();                   //��ʼ��TFmini PLUS
	TIM6_Init();                    //��ʼ����ʱ6�жϣ�������������󣬷��߻����	
/***********������************/	
	delay_ms(200);



void ontrol(int sign,int sign_x,int sign_y);


//		TIM12_PwmSetPulse(1,8);//1�����ϲִ�
//		delay_ms(500);
//		TIM12_PwmSetPulse(1,22);//1�����ϲֹر�
//		delay_ms(500);
//		
//		
//		TIM12_PwmSetPulse(2,8);//2�����ϲִ�
//		delay_ms(500);
//		TIM12_PwmSetPulse(2,22);//2�����ϲֹر�
//		delay_ms(500);



sign = 0;
sign_x = 0;
sign_y = 0;


delay_ms(500);
task_flag = 0;

//left();

//right();
//forward();

while(1){	
	
	//go_flag=1;
	//forward();
	//backward();
	//left();
	
		
//		delay_ms(200);
//		OLED_DISPLAY_8x16(6,0,X);   
//		OLED_DISPLAY_8x16(6,24,Y); 
//		OLED_DISPLAY_8x16(2,80,Y/1000%10+0x30);
//		OLED_DISPLAY_8x16(2,88,Y/100%10+0x30);
//		OLED_DISPLAY_8x16(2,96,Y/10%10+0x30);
//		OLED_DISPLAY_8x16(2,104,Y%10+0x30); 
//	
//		OLED_DISPLAY_8x16(2,16,X/1000%10+0x30);
//		OLED_DISPLAY_8x16(2,24,X/100%10+0x30);
//		OLED_DISPLAY_8x16(2,32,X/10%10+0x30);
//		OLED_DISPLAY_8x16(2,40,X%10+0x30);	
//	if(go_flag==1 && Y<180&&X>100&&X<115){
//		forward();
//	}else if(go_flag==1 && Y<180&&X>105){
//		left();
//	}else if(go_flag==1 && Y<180&&X<100){
//		
//		right();
//	}else if(go_flag == 2 && X<95){
//		right();
//	}else if(go_flag == 2 && X>95&&X<145)
//	{
//		backward();
//	}else if(go_flag ==2&& X>145){
//	
//		left();
//	}else if(go_flag == 3&& X>100){
//		left();
//	}else if(go_flag == 3&& X>100&&X<145){
//		
//		backward();
//	}else if (go_flag == 3&& X<95){
//		right();
//	}else 
//	stop();
	
	
}
}





int motor1_speed = 60;
int motor2_speed = 60;
int motor3_speed = 60;
int motor4_speed = 60 ;



void forward(void){
			TIM8_PwmSetPulse(3,motor1_speed);     //motor1 = T8CH3ǰ��   CH4����
			TIM1_PwmSetPulse(4,motor2_speed);    //motor2 = T1CH4ǰ��   CH3����
	
			TIM8_PwmSetPulse(1,motor3_speed);		//motor3 = T8CH1ǰ��   CH2����
			TIM1_PwmSetPulse(2,motor4_speed);     //motor4 = T1CH2ǰ��   CH1����
//			delay_ms(200);
//			stop();
}


void backward(void){
			TIM8_PwmSetPulse(4,motor1_speed);     //motor1 = T8CH3ǰ��   CH4����
			TIM1_PwmSetPulse(3,motor2_speed);    //motor2 = T1CH4ǰ��   CH3����
	
			TIM8_PwmSetPulse(2,motor3_speed);		//motor3 = T8CH1ǰ��   CH2����
			TIM1_PwmSetPulse(1,motor4_speed);     //motor4 = T1CH2ǰ��   CH1����
	
//			delay_ms(200);
//			stop();
}


//������ʻ  14�����ӷ�ת��23��ת
void left(void){
			forward();
			delay_ms(1500);
			TIM8_PwmSetPulse(4,70);     //motor1 = T8CH3ǰ��   CH4����
			TIM1_PwmSetPulse(4,70);    //motor2 = T1CH4ǰ��   CH3����
	
			TIM8_PwmSetPulse(2,70);		//motor3 = T8CH1ǰ��   CH2����
			TIM1_PwmSetPulse(2,70);     //motor4 = T1CH2ǰ��   CH1����
	
	
			delay_ms(550);
			stop();

}
void right(void){
			forward();
			delay_ms(1500);
			TIM8_PwmSetPulse(3,70);     //motor1 = T8CH3ǰ��   CH4����
			TIM1_PwmSetPulse(3,70);    //motor2 = T1CH4ǰ��   CH3����
	
			TIM8_PwmSetPulse(1,70);		//motor3 = T8CH1ǰ��   CH2����
			TIM1_PwmSetPulse(1,70);     //motor4 = T1CH2ǰ��   CH1����
			delay_ms(550);
			stop();
}


void stop (void){
			
	TIM1_PwmSetPulse(1,0);   
	TIM1_PwmSetPulse(2,0); 
	TIM1_PwmSetPulse(3,0); 
	TIM1_PwmSetPulse(4,0); 
	TIM8_PwmSetPulse(1,0);   
	TIM8_PwmSetPulse(2,0);	
	TIM8_PwmSetPulse(3,0);	
	TIM8_PwmSetPulse(4,0);	


}
	




void  BASIC_TIM_IRQHandler (void)
{	
		if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		
	
		
				//delay_ms(200);
		//printf("sing = %d \r\nsing_x = %d \r\nsing_y = %d \r\n",sign,sign_x,sign_y);  
		if(sign == 1){
			didian = sign_x;
			wuliao = sign_y;
			if(sign_y == 1&& task_flag ==0 ){
				
				sign_x =0;
				sign_y=0;
				printf("%s",open1);
				TIM12_PwmSetPulse(1,8);/*1�Ŵ�*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(1,22);/*1�Źر�*/
				task_flag = 1;
				go_flag = 1;
				forward();
				delay_ms(7300);
				stop();
				delay_ms(10000);
						if(sign_x ==1){
							left();
							forward();
							delay_ms(5000);
							stop();
							}else{
							right();
							forward();
							delay_ms(5000);
							stop();
				
				}
			}else if(sign_y == 2&& task_flag ==0){
				sign_x =0;
				sign_y=0;
				printf("%s",open2);
				TIM12_PwmSetPulse(2,8);/*2�Ŵ�*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(2,22);/*2�Źر�*/
				task_flag = 2;
				go_flag = 1;
				forward();
				delay_ms(7400);
				stop();
				delay_ms(10000);
				if(sign_x ==1){
					left();
					forward();
					delay_ms(5000);
					stop();
				}else{
					right();
					forward();
					delay_ms(5000);
					stop();
				
				}
				
				
			}else if(sign_y == 1&& task_flag ==1){
				sign_x =0;
				sign_y=0;
				printf("%s",close1);
				TIM12_PwmSetPulse(1,8);/*1�Ŵ�*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(1,22);/*1�Źر�*/
				task_flag = 1;
				go_flag = 2;
				right();
				delay_ms(4000);
				stop();
			}else if(sign_y == 2&& task_flag ==2){
				sign_x =0;
				sign_y=0;
				printf("%s",close2);
				TIM12_PwmSetPulse(2,8);/*2�Ŵ�*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(2,22);/*2�Źر�*/
				task_flag = 1;
				go_flag = 3;
				left();
				delay_ms(4000);
				stop();
			}
		}else //if(sign==2){//////////////�ϰ���
//				if(sign_x < safe_x && sign_y < safe_y)//�ϰ������Ϸ�
//				{
//					right();
//				}else if(sign_x > safe_x && sign_y < safe_y)//�ϰ��������Ϸ�
//				{
//					left();
//				}else if(sign_x < safe_x && sign_y > safe_y)//�ϰ������·�
//				{	
//					backward();
//					delay_ms(200);
//					right();
//				}else if(sign_x > safe_x && sign_y > safe_y)//�ϰ��������·�
//				{
//					backward();
//					delay_ms(200);
//					left();
//				}else 
//				stop();
			
//			}else if(sign != 1&& go_flag==5){//���̵�ͣ����
//				stop();
//				TIM9_PwmSetPulse(1,12);//����ͷ̧��//ch1����ͷ̧��Χ10-24��ԽС̧Խ�ߣ�ch2����Ϊ40��174����ʱ��23��278˳ʱ��
//				BREEZE=1;//BREEZE��ʼ�������һ��
//				delay_ms(600);                  //��ʱ�ȴ�
//				BREEZE = 0;  
//				TIM9_PwmSetPulse(1,8);//����ͷ̧��
//				delay_ms(2000);
			
//				TIM9_PwmSetPulse(1,20);//����ͷ̧��
//				delay_ms(2000);
				
//			}else if(sign == 4){
//					if(task_flag==1){   //����1
//							TIM9_PwmSetPulse(1,22);//����ͷ̧��//����ͷ̧��//ch1����ͷ̧��Χ10-24��ԽС̧Խ�ߣ�ch2����Ϊ40��174����ʱ��23��278˳ʱ��
//							TIM9_PwmSetPulse(2,23);//����ͷ��ת
//							delay_ms(278);
//							TIM9_PwmSetPulse(2,0);//����ͷ��ת
//							forward();
//							delay_ms(1000);
//							stop();
//							delay_ms(500);
//							left();
//							delay_ms(5000);
//					}else if(task_flag==2){//����2
//							TIM9_PwmSetPulse(1,8);//����ͷ̧��//����ͷ̧��//ch1����ͷ̧��Χ10-24��ԽС̧Խ�ߣ�ch2����Ϊ��ʱ��40��174 ˳ʱ��Ϊ23��278
//							TIM9_PwmSetPulse(2,40);//����ͷ��ת
//							delay_ms(174);
//							TIM9_PwmSetPulse(2,0);
//							forward();
//							delay_ms(1000);
//							stop();
//							delay_ms(500);
//							right();
//							delay_ms(5000);
//						}
//			}else if(sign == 5){
//					if(task_flag ==1){
//						if(sign_x < safe_x && sign_y < safe_y)//ͣ���������Ϸ�
//						{backward();delay_ms(200);left();delay_ms(200);}else if(sign_x > safe_x && sign_y < safe_y)//ͣ���������Ϸ�
//						{forward();delay_ms(200);left();delay_ms(200);}else if(sign_x < safe_x && sign_y > safe_y)//ͣ�������·�
//						{stop();}else if(sign_x > safe_x && sign_y > safe_y)//ͣ���������·�
//						{stop();}else stop();
//					}else if(task_flag ==2){
//					if(sign_x < safe_x && sign_y < safe_y)//ͣ���������Ϸ�
//						{backward();delay_ms(200);right();delay_ms(200);}else if(sign_x > safe_x && sign_y < safe_y)//ͣ���������Ϸ�
//						{forward();delay_ms(200);right();delay_ms(200);}else if(sign_x < safe_x && sign_y > safe_y)//ͣ�������·�
//						{stop();}else if(sign_x > safe_x && sign_y > safe_y)//ͣ���������·�
//						{stop();}else stop();
//					}
//			}else 
			stop();
		}
	
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		 
	}		 	









