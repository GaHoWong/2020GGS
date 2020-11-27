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

u8 flag_Stop=1;      //停止标志位
int  Encoder[4];     //编码器的脉冲计数
int moto;            //电机PWM变量
int task_flag;       //任务标志，1 = 11，2 = 12，3 = 21，4 = 22
int wuliao_flag;     //物料标志，如果为1则一号物料，2为二号物料
int dir_flag;	      //方向标志，如果为0则左转，为1则右转   


char pro[20] = {0x41,0x46,0x3a,0x33,0x30}; //语音播报模块——>增大声音
//扫描完成，请装入1号物料，并关闭仓门
char open1[38] = {0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
//扫描完成，请装入2号物料，并关闭仓门
char open2[38] ={0xc9,0xa8,0xc3,0xe8,0xcd,0xea,0xb3,0xc9,0xa3,0xac,0xc7,0xeb,0xd7,0xb0,0xc8,0xeb,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
//到达1号收货点，请出示提货码
char arr1[30] = {0xb5,0xbd,0xb4,0xef,0x31,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
//到达2号收货点，请出示提货码
char arr2[30] = {0xb5,0xbd,0xb4,0xef,0x32,0xba,0xc5,0xca,0xd5,0xbb,0xf5,0xb5,0xe3,0xa3,0xac,0xc7,0xeb,0xb3,0xf6,0xca,0xbe,0xcc,0xe1,0xbb,0xf5,0xc2,0xeb};
//请取出1号物料，并关闭仓门
char close1[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x31,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};
//请取出2号物料，并关闭仓门
char close2[27] = {0xc7,0xeb,0xc8,0xa1,0xb3,0xf6,0x32,0xba,0xc5,0xce,0xef,0xc1,0xcf,0xa3,0xac,0xb2,0xa2,0xb9,0xd8,0xb1,0xd5,0xb2,0xd6,0xc3,0xc5};

//重定向printf函数
int fputc(int c, FILE *fp)//语音播报模块为UART7
{

	USART_SendData(UART7,(u8)c );	// 发送单字节数据
	while (USART_GetFlagStatus(UART7, USART_FLAG_TXE) == RESET);	//等待发送完毕 

	return (c); //返回字符
}



void GO_BACK(void){
	
	TIM8_PwmSetPulse(1,70);
	TIM8_PwmSetPulse(3,70);
   TIM1_PwmSetPulse(2,65);
	TIM1_PwmSetPulse(4,65);
}



void GO(void){
	
	TIM8_PwmSetPulse(1,62);
	TIM8_PwmSetPulse(3,62);
   TIM1_PwmSetPulse(2,65);
	TIM1_PwmSetPulse(4,65);
}


int main(void)
{
/***********变量定义************/	

	
/***********STM32外设初始化************/	
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz

	delay_init(180);			          //初始化延时函数
//	uart_init(90,115200);             //与电脑端串口通讯,使用电机时，必须把这行和所有printf注释掉，否则电脑蓝屏
	PAout(9) = 0;
	PAout(10) = 0;
//	K210_USART(90,115200);            //与K210进行通讯
	USART6_Init(115200);              //与OPEN MV进行通讯
	I2C_Configuration();              //硬件I2C初始化
	
//	printf("STM32外设初始化成功！");
	
/***********硬件初始化************/	
//	MOTOR_Init();                    //初始化电机
	TIM12_PWMinit(20000,1000000);    //50hz
	TIM9_PWMinit(20000,1000000);     //50hz
//	SDRAM_Init();	       			   //初始化SDRAM 		
	LED_Init();		            			//初始化与LED连接的硬件接口
	KEY_Init();					            //初始化按键
	BREEZE_Init();                  //初始化蜂鸣器
	PLAY_Init();	                  //初始化语音播报模块
	OLED0561_Init();                 //初始化OLED显示屏
	EncoderInit();	                  //初始化编码器   
	OLED_SHOW();                     //oled显示  
		
/***********硬件初始化设置************/	
	

	printf("%s", pro);              //增大音量
	delay_ms(200);                  //延时等待
	
/***********初始化完成************/	
	
	LED0 = 0;                       //板载LED灯亮起
	LED1 = 0;                       //板载LED灯亮起，说明整个初始化成功!

//  	TFmini_Init();                   //初始化TFmini PLUS
	TIM6_Init();                    //初始化定时6中断，这个必须放在最后，否者会错误	
/***********调试区************/	
	delay_ms(200);



void ontrol(int sign,int sign_x,int sign_y);


//		TIM12_PwmSetPulse(1,8);//1号物料仓打开
//		delay_ms(500);
//		TIM12_PwmSetPulse(1,22);//1号物料仓关闭
//		delay_ms(500);
//		
//		
//		TIM12_PwmSetPulse(2,8);//2号物料仓打开
//		delay_ms(500);
//		TIM12_PwmSetPulse(2,22);//2号物料仓关闭
//		delay_ms(500);



sign = 0;
sign_x = 0;
sign_y = 0;


delay_ms(500);
task_flag = 0;

while(1){	

//				TIM9_PwmSetPulse(2,40);//ch1摄像头抬起范围10-24，越小抬越高，ch2设置为40，174则逆时针23，278顺时针
//				delay_ms(174);
//				TIM9_PwmSetPulse(2,0);
//	delay_ms(2000);
//			//	TIM9_PwmSetPulse(2,23);
//				delay_ms(274);
////				TIM9_PwmSetPulse(2,18);//摄像头抬起
////	delay_ms(300);
//				TIM9_PwmSetPulse(2,0);
//	
//				delay_ms(400);
				
//				TIM9_PwmSetPulse(1,20);//摄像头抬起
//				delay_ms(2000);
		
		//GO();
		delay_ms(2000);
		
		//ontrol(sign,sign_x,sign_y);
//		TIM9_PwmSetPulse(2,22);
//		delay_ms(2000);
//		TIM9_PwmSetPulse(2,12);
//		delay_ms(2000);
//		TIM9_PwmSetPulse(2,22);
//		delay_ms(1500);
//		
		
//		delay_ms(100);
		//sign_x++;
//		printf("1");
	//	GO_BACK();
		

		
		
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
//		printf("PWM:%d\r\n",led0pwmval);//查看占空比
	
}
}


//void ontrol(int sign,int sign_x,int sign_y){
//	int safe_x = 80;		//x坐标的安全范围
//	int safe_y = 60; 		//y坐标的安全范围
//	
//	switch(sign){
//		case 1://识别到二维码
//		{
//			switch(sign_x){
//			case 1: printf("%s",open1); delay_ms(5000);task_flag = 1;break;  
//			case 2: printf("%s",open2);  delay_ms(5000);task_flag = 2;break;
//			case 3: printf("%s",arr1);  delay_ms(5000);break;
//			case 4: printf("%s",arr2);  delay_ms(5000);break;
//			case 5: printf("%s",close1);  delay_ms(5000);break;
//			case 6: printf("%s",close2);  delay_ms(5000);break;
//			default://printf("error\n");delay_ms(2000); break;}
//			}
//		case 2://障碍物
//			{
//				if(sign_x < safe_x && sign_y < safe_y)//此时障碍物在左上，则车向↗右上行驶
//				{
////					GO_right_UP();//伪代码，右上行驶
//					
//				}else if(sign_x > safe_x && sign_y < safe_y){//此时障碍物在右上，则车向↖左上行驶
//					
////					GO_left_UP();//伪代码，左上行驶
//					
//				}else if(sign_x < safe_x&& sign_y > safe_y){//此时障碍物在左下，则车向→右行驶
//					
////					GO_right();
//					
//				}else if(sign_x > safe_x&& sign_y > safe_y){
//					
////					GO_left();
//					
//				}else{
////					GO_BACK();//后退
//				}
//				break;
//			}
//		case 3://红绿灯停车区
//			{
//					break;
//				
//			}
//		case 4://绿灯
//			{	
////				GO_TO(0,sign_x,sign_y);//调整
//				if(task_flag == 1){//任务1左行驶
////					GO_UP();       //前进
////					GO_left();     //左行驶
//				}else if(task_flag == 2){//任务2右行驶
////					GO_UP();       //前进
////					GO_right();     //左行驶
//				}else 
//				break;
//				
//			}
//		case 5:
//			{
//			//	GO_TO(1,sign_x,sign_y);//调整
//			}
//		case 6://收货地2
//			{
//		//		GO_TO(2,sign_x,sign_y);//调整
//			}
//		default://其它情况，停车
//			{
//		//		STOP();
//				break;	
//			}							
//		}
//	
//	

//}

int motor1_speed = 65;
int motor2_speed = 65;
int motor3_speed = 65;
int motor4_speed = 65 ;

void forward(void){
			TIM8_PwmSetPulse(3,motor1_speed);     //motor1 = T8CH3前进   CH4后退
			TIM1_PwmSetPulse(4,motor2_speed);    //motor2 = T1CH4前进   CH3后退
	
			TIM8_PwmSetPulse(1,motor3_speed);		//motor3 = T8CH1前进   CH2后退
			TIM1_PwmSetPulse(2,motor4_speed);     //motor4 = T1CH2前进   CH1后退
}


void backward(void){
			TIM8_PwmSetPulse(4,motor1_speed);     //motor1 = T8CH3前进   CH4后退
			TIM1_PwmSetPulse(3,motor2_speed);    //motor2 = T1CH4前进   CH3后退
	
			TIM8_PwmSetPulse(2,motor3_speed);		//motor3 = T8CH1前进   CH2后退
			TIM1_PwmSetPulse(1,motor4_speed);     //motor4 = T1CH2前进   CH1后退
}


//左上行驶
void left_forward(void){
			TIM8_PwmSetPulse(3,motor1_speed);     //motor1 = T8CH4前进   CH3后退
			TIM1_PwmSetPulse(4,motor2_speed);    //motor2 = T1CH3前进   CH4后退
	
			TIM8_PwmSetPulse(1,motor3_speed);		//motor3 = T8CH2前进   CH1后退
			TIM1_PwmSetPulse(2,motor4_speed);     //motor4 = T1CH1前进   CH2后退

}
//右上行驶
void right_forward(void){
			TIM8_PwmSetPulse(3,motor1_speed);     //motor1 = T8CH4前进   CH3后退
			TIM1_PwmSetPulse(4,motor2_speed);    //motor2 = T1CH3前进   CH4后退
	
			TIM8_PwmSetPulse(1,motor3_speed);		//motor3 = T8CH2前进   CH1后退
			TIM1_PwmSetPulse(2,motor4_speed);     //motor4 = T1CH1前进   CH2后退

}




//向左行驶  14号轮子反转，23正转
void left(void){
			TIM8_PwmSetPulse(4,motor1_speed);     //motor1 = T8CH3前进   CH4后退
			TIM1_PwmSetPulse(4,motor2_speed);    //motor2 = T1CH4前进   CH3后退
	
			TIM8_PwmSetPulse(1,motor3_speed);		//motor3 = T8CH1前进   CH2后退
			TIM1_PwmSetPulse(1,motor4_speed);     //motor4 = T1CH2前进   CH1后退

}
void right(void){

			TIM8_PwmSetPulse(3,motor1_speed);     //motor1 = T8CH3前进   CH4后退
			TIM1_PwmSetPulse(3,motor2_speed);    //motor2 = T1CH4前进   CH3后退
	
			TIM8_PwmSetPulse(2,motor3_speed);		//motor3 = T8CH1前进   CH2后退
			TIM1_PwmSetPulse(2,motor4_speed);     //motor4 = T1CH2前进   CH1后退
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
	









int task_flag = 0;
int go_back = 0;  	//如果为1则从左边回来，为2则从右边回来
int safe_x = 80;		//x坐标的安全范围
int safe_y = 60; 		//y坐标的安全范围

void  BASIC_TIM_IRQHandler (void)
{	
		if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	


		
		//printf("%d\r\n",sign);
//		forward();
//		delay_ms(3000);
//		stop();
//		delay_ms(800);
//		backward();
//		delay_ms(3000);
//		stop();
//		delay_ms(800);		
//		left();
//		delay_ms(3000);
//		stop();
//		delay_ms(800);		
//		right();
//		delay_ms(3000);
//		stop();
//		delay_ms(900000);
		sign = 3;
	 
		sign_x=6;      
		delay_ms(1000);
		if(sign == 1){
			switch(sign_x){
			case 1: {
				printf("%s",open1);
				TIM12_PwmSetPulse(1,8);/*1号打开*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(1,23);/*1号关闭*/
				task_flag = 1;
				forward();
				delay_ms(1000);
				break;}  
			case 2: {
				printf("%s",open2);
				TIM12_PwmSetPulse(2,8); /*2号打开*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(2,22);/*1号关闭*/
				task_flag = 2;
				forward();
				delay_ms(1000);
				break;}
			case 3: printf("%s",arr1);stop(); delay_ms(4000);break;
			case 4: printf("%s",arr2);stop(); delay_ms(4000);break;
			case 5:{ 		
				if(task_flag ==1){
					go_back = 1;
				}else if(task_flag ==2){
					go_back = 2;
				}else{
				printf("%s",close1); 
				TIM12_PwmSetPulse(1,8);/*1号打开*/ 
				delay_ms(5000);
				TIM12_PwmSetPulse(1,22);/*1号关闭*/ 
				}
				break;}
			case 6: {//请取出2号物料并关闭舱门
				if(task_flag ==1){
					go_back = 1;
				}else if(task_flag ==2){
					go_back = 2;
				}else{
					printf("%s",close2); 
					TIM12_PwmSetPulse(2,8);/*2号打开*/ 				
					delay_ms(5000);
					TIM12_PwmSetPulse(2,22);/*2号打开*/ 
					}
					break;}
			default:printf("error");delay_ms(2000); break;}
			}else if(sign == 2){//障碍物
				if(sign_x < safe_x && sign_y < safe_y)//障碍物左上方
				{
					right();
				}else if(sign_x > safe_x && sign_y < safe_y)//障碍物在右上方
				{
					left();
				}else if(sign_x < safe_x && sign_y > safe_y)//障碍物左下方
				{	
					backward();
					delay_ms(200);
					right();
				}else if(sign_x > safe_x && sign_y > safe_y)//障碍物在右下方
				{
					backward();
					delay_ms(200);
					left();
				}else 
				stop();
			}else if(sign == 3){//红绿灯停车区
				stop();
				TIM9_PwmSetPulse(1,12);//摄像头抬起//ch1摄像头抬起范围10-24，越小抬越高，ch2设置为40，174则逆时针23，278顺时针
//				TIM9_PwmSetPulse(1,8);//摄像头抬起
//				delay_ms(2000);
//				
//				TIM9_PwmSetPulse(1,20);//摄像头抬起
//				delay_ms(2000);
				
			}else if(sign == 4){
					if(task_flag==1){   //任务1
						TIM9_PwmSetPulse(1,22);//摄像头抬下//摄像头抬起//ch1摄像头抬起范围10-24，越小抬越高，ch2设置为40，174则逆时针23，278顺时针
						TIM9_PwmSetPulse(2,23);//摄像头左转
						delay_ms(278);
						TIM9_PwmSetPulse(2,0);//摄像头左转
						forward();
						delay_ms(1000);
						stop();
						delay_ms(500);
						left();
						delay_ms(2000);
						}else if(task_flag==2){//任务2
						TIM9_PwmSetPulse(1,8);//摄像头抬下//摄像头抬起//ch1摄像头抬起范围10-24，越小抬越高，ch2设置为逆时针40，174 顺时针为23，278
						TIM9_PwmSetPulse(2,40);//摄像头右转
						delay_ms(174);
						TIM9_PwmSetPulse(2,0);
						forward();
						delay_ms(1000);
						stop();
						delay_ms(500);
						right();
						delay_ms(2000);
						}
			}else if(sign == 5){
					if(task_flag ==1){
						if(sign_x < safe_x && sign_y < safe_y)//停车区在左上方
						{backward();delay_ms(200);left();delay_ms(200);}else if(sign_x > safe_x && sign_y < safe_y)//停车区在右上方
						{forward();delay_ms(200);left();delay_ms(200);}else if(sign_x < safe_x && sign_y > safe_y)//停车区左下方
						{stop();}else if(sign_x > safe_x && sign_y > safe_y)//停车区在右下方
						{stop();}else stop();
					}else if(task_flag ==2){
					if(sign_x < safe_x && sign_y < safe_y)//停车区在左上方
						{backward();delay_ms(200);right();delay_ms(200);}else if(sign_x > safe_x && sign_y < safe_y)//停车区在右上方
						{forward();delay_ms(200);right();delay_ms(200);}else if(sign_x < safe_x && sign_y > safe_y)//停车区左下方
						{stop();}else if(sign_x > safe_x && sign_y > safe_y)//停车区在右下方
						{stop();}else stop();
					}
			}else 
			stop();
		}
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		 
	}		 	









