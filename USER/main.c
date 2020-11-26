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







int sign = 0;
int sign_x = 0 ;
int sign_y = 0;

u8 flag_Stop=1;      //ֹͣ��־λ
int  Encoder[4];     //���������������
int moto;            //���PWM����
int task_flag;       //�����־��1 = 11��2 = 12��3 = 21��4 = 22
int wuliao_flag;     //���ϱ�־�����Ϊ1��һ�����ϣ�2Ϊ��������
int dir_flag;	      //�����־�����Ϊ0����ת��Ϊ1����ת   


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

	USART_SendData( USART1,(u8)c );	// ���͵��ֽ�����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ�������� 

	return (c); //�����ַ�
}



void GO_BACK(void){
	
	TIM8_PwmSetPulse(1,62);
	TIM8_PwmSetPulse(3,62);
   TIM1_PwmSetPulse(1,65);
	TIM1_PwmSetPulse(3,65);
	delay_ms(100);
	TIM8_PwmSetPulse(1,0);
	TIM8_PwmSetPulse(3,0);
   TIM1_PwmSetPulse(1,0);
	TIM1_PwmSetPulse(3,0);
	
}



int main(void)
{
/***********��������************/	

	
/***********STM32�����ʼ��************/	
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz

	delay_init(180);			          //��ʼ����ʱ����
	uart_init(90,115200);             //����Զ˴���ͨѶ,ʹ�õ��ʱ����������к�����printfע�͵��������������
//	PAout(9) = 0;
//	PAout(10) = 0;
//	K210_USART(90,115200);            //��K210����ͨѶ
	USART6_Init(115200);              //��OPEN MV����ͨѶ
	I2C_Configuration();              //Ӳ��I2C��ʼ��
	
//	printf("STM32�����ʼ���ɹ���");
	
/***********Ӳ����ʼ��************/	
	MOTOR_Init();                    //��ʼ�����
//	SDRAM_Init();	       			  //��ʼ��SDRAM 		
	LED_Init();		            			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					            //��ʼ������
	BREEZE_Init();                  //��ʼ��������
//	PLAY_Init();	                  //��ʼ����������ģ��
//	OLED0561_Init();                 //��ʼ��OLED��ʾ��
//	EncoderInit();	                  //��ʼ��������   
//	OLED_SHOW();                     //oled��ʾ  
		
/***********Ӳ����ʼ������************/	
	
//	int a = 123;
	//OLED_ShowNumber(2,0,a,3);
	
	printf("%s", pro);              //��������
	delay_ms(200);                  //��ʱ�ȴ�
	
/***********��ʼ�����************/	
	
	LED0 = 0;                       //����LED������
	LED1 = 0;                       //����LED������˵��������ʼ���ɹ�!

//  	TFmini_Init();                   //��ʼ��TFmini PLUS
//	TIM6_Init();                    //��ʼ����ʱ6�жϣ�������������󣬷��߻����	
/***********������************/	
	delay_ms(200);

	TIM12_PWMinit(20000,1000000);  //50hz
//	TIM9_PWMinit(20000,1000000);  //50hz

void ontrol(int sign,int sign_x,int sign_y);





sign = 1;
sign_x = 0;
sign_y = 0;


TIM12_PwmSetPulse(1,20);
delay_ms(500);
task_flag = 0;

	while(1){		
		//ontrol(sign,sign_x,sign_y);
		
		
//		delay_ms(100);
		//sign_x++;
//		printf("1");
	//	GO_BACK();
		
		TIM12_PwmSetPulse(1,8);
		delay_ms(500);
		TIM12_PwmSetPulse(1,22);
		delay_ms(500);
		
		
		
		TIM12_PwmSetPulse(2,8);
		delay_ms(500);
		TIM12_PwmSetPulse(2,22);
		delay_ms(500);
	
//		TIM12_PwmSetPulse(1,20);
//		delay_ms(500);

//		TIM12_PwmSetPulse(1,10);
//		delay_ms(285);
//		TIM12_PwmSetPulse(2,10);
//		delay_ms(2000);
//		TIM12_PwmSetPulse(2,30);
//		delay_ms(285);
//		TIM9_PwmSetPulse(1,0);
//		delay_ms(5000);
//		TIM12_PwmSetPulse(1,80);
//		delay_ms(1000);
		
//		  TIM12_PwmSetPulse(1,0);
//		delay_ms(200);
//		TIM12_PwmSetPulse(1,50);
		//printf("%s",suc2);
//		delay_ms(5000);
//		OLED_DISPLAY_8x16(6,0,sign);   
//		OLED_DISPLAY_8x16(6,24,sign_x); 
//		OLED_DISPLAY_8x16(6,48,sign_y);   

		
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


void ontrol(int sign,int sign_x,int sign_y){
	int safe_x = 80;		//x����İ�ȫ��Χ
	int safe_y = 60; 		//y����İ�ȫ��Χ
	
	switch(sign){
		case 1://ʶ�𵽶�ά��
		{
			switch(sign_x){
			case 1: printf("%s",open1); delay_ms(5000);task_flag = 1;break;  
			case 2: printf("%s",open2);  delay_ms(5000);task_flag = 2;break;
			case 3: printf("%s",arr1);  delay_ms(5000);break;
			case 4: printf("%s",arr2);  delay_ms(5000);break;
			case 5: printf("%s",close1);  delay_ms(5000);break;
			case 6: printf("%s",close2);  delay_ms(5000);break;
			default:printf("error\n");delay_ms(2000); break;}
			}
		case 2://�ϰ���
			{
				if(sign_x < safe_x && sign_y < safe_y)//��ʱ�ϰ��������ϣ�����J������ʻ
				{
//					GO_right_UP();//α���룬������ʻ
					
				}else if(sign_x > safe_x && sign_y < safe_y){//��ʱ�ϰ��������ϣ�����I������ʻ
					
//					GO_left_UP();//α���룬������ʻ
					
				}else if(sign_x < safe_x&& sign_y > safe_y){//��ʱ�ϰ��������£����������ʻ
					
//					GO_right();
					
				}else if(sign_x > safe_x&& sign_y > safe_y){
					
//					GO_left();
					
				}else{
//					GO_BACK();//����
				}
				break;
			}
		case 3://���̵�ͣ����
			{
					break;
				
			}
		case 4://�̵�
			{	
//				GO_TO(0,sign_x,sign_y);//����
				if(task_flag == 1){//����1����ʻ
//					GO_UP();       //ǰ��
//					GO_left();     //����ʻ
				}else if(task_flag == 2){//����2����ʻ
//					GO_UP();       //ǰ��
//					GO_right();     //����ʻ
				}else 
				break;
				
			}
		case 5:
			{
			//	GO_TO(1,sign_x,sign_y);//����
			}
		case 6://�ջ���2
			{
		//		GO_TO(2,sign_x,sign_y);//����
			}
		default://���������ͣ��
			{
		//		STOP();
				break;	
			}							
		}
	
	

}







