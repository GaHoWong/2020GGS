#include "control.h"
#include "sys.h"

int Target_velocity=50;  //�趨�ٶȿ��Ƶ�Ŀ���ٶ�Ϊ50������ÿ10ms
int TIM3_IRQHandler(void)
{
 if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
 {
   TIM_ClearITPendingBit(TIM3,TIM_IT_Update);   //===�����ʱ��1�жϱ�־λ
	 Encoder[0]=Read_Encoder(2);                     //ȡ��ʱ��2��������ֵ
                             //LED��˸
	 moto=Incremental_PI(Encoder[0],Target_velocity);    //===λ��PID������
	 Xianfu_Pwm();
   Set_Pwm(moto);
 }
 return 0;
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto)//��ֵ��PWM�Ĵ���
{
 if(moto>0) AIN1=0,   AIN2=1;
	else      AIN1=1,   AIN2=0;
	PWMA=myabs(moto);
}
/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
 void Xianfu_Pwm(void) //���Ʒ��ȵĺ���
 {
   int Amplitude=7100;  //===PWM������7200 ������7100
	 if(moto<-Amplitude)  moto = -Amplitude;
	 if(moto>Amplitude)   moto =  Amplitude;
 }
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a) //ȡ����ֵ
{ 		   
	 int temp;
	 if(a<0)  temp=-a;  
	 else temp=a;
	 return temp;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
