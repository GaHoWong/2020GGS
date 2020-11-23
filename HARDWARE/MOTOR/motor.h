#ifndef MOTER_H_
#define MOTER_H_

#include "sys.h"
#include "timer.h"






/***********电机方向控制IO 宏定义*******************/ 
#define Moter_A_2   PBout(12)
#define Moter_A_1   PBout(13)

#define Moter_B_2   PBout(15)
#define Moter_B_1   PBout(14)

#define Moter_C_2   PDout(10)
#define Moter_C_1   PDout(11)  

#define Moter_D_1   PCout(3)
#define Moter_D_2   PCout(2) 







/***********编码器代号*******************/ 
#define Encoder_A  0
#define Encoder_B  1
#define Encoder_C  2
#define Encoder_D  3

/***********方向代号*******************/ 
#define Dir_Forword  0
#define Dir_Back     1
#define Dir_Right    2
#define Dir_Left     3
#define Dir_Stop     4
#define Dir_FR       5
#define Dir_FL       6
#define Dir_BR       7
#define Dir_BL       8
#define Turn_R       9
#define Turn_L      10

/***********速度代号*******************/ 
#define PWM_A PWM[0]
#define PWM_B PWM[1]
#define PWM_C PWM[2]
#define PWM_D PWM[3]

extern int Target_Speed[];
#define Target_A Target_Speed[0]
#define Target_B Target_Speed[1]
#define Target_C Target_Speed[2]
#define Target_D Target_Speed[3]


/*************电机速度PI的时间基准*******************/
void TIM7_Int_Init(u16 arr,u16 psc);

/***********车身运动控制算法*******************/ 
void Kinematic_Analysis(short Vx,short Vy,short Vz);

/************电机速度控制算法******************/
int Motor1_PI(int Encoder, int Target);
int Motor2_PI(int Encoder, int Target);
int Motor3_PI(int Encoder, int Target);
int Motor4_PI(int Encoder, int Target);

void Moter_Target_limit(short int *A,short int *B,short int *C,short int *D);
int Moter_PWM_Limit(int a);



/*************电机硬件底层初始化***************/
void Moter_Init(void);
// 以下是电机硬件编码器初始化函数
void TIM2_ENC_Init(void);
void TIM3_ENC_Init(void);
void TIM4_ENC_Init(void);
void TIM5_ENC_Init(void);

//void TIM1_PWM_Init(u16 arr,u16 psc); //  电机的PWM信号初始化函数

void Moter_IOs_Init(void);       // 电机方向控制IO初始化

/*****************************************/




int num_String(int x, char *s);






#define BASIC_TIM           		TIM6
#define BASIC_TIM_CLK       		RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn					TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler    TIM6_DAC_IRQHandler

///////////////////
void TIM6_Init(void); //定时器中断初始化
void MOTOR_Init(void);         //电机初始化


void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM1_PwmSetPulse (u8 TIM1_CH,u8 pulse);
void TIM8_PWM_Init(u16 arr,u16 psc);
void TIM8_PwmSetPulse (u8 TIM8_CH,u8 pulse);



#define MOTOR1_A PAout(8)	
#define MOTOR1_B PAout(9)	
#define MOTOR3_A PAout(10)	
#define MOTOR3_B PAout(11)	



#endif


