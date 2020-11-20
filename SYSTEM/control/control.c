#include "control.h"
#include "sys.h"

int Target_velocity=50;  //设定速度控制的目标速度为50个脉冲每10ms
int TIM3_IRQHandler(void)
{
 if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
 {
   TIM_ClearITPendingBit(TIM3,TIM_IT_Update);   //===清除定时器1中断标志位
	 Encoder[0]=Read_Encoder(2);                     //取定时器2计数器的值
                             //LED闪烁
	 moto=Incremental_PI(Encoder[0],Target_velocity);    //===位置PID控制器
	 Xianfu_Pwm();
   Set_Pwm(moto);
 }
 return 0;
}
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto)//赋值给PWM寄存器
{
 if(moto>0) AIN1=0,   AIN2=1;
	else      AIN1=1,   AIN2=0;
	PWMA=myabs(moto);
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
 void Xianfu_Pwm(void) //限制幅度的函数
 {
   int Amplitude=7100;  //===PWM满幅是7200 限制在7100
	 if(moto<-Amplitude)  moto = -Amplitude;
	 if(moto>Amplitude)   moto =  Amplitude;
 }
/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a) //取绝对值
{ 		   
	 int temp;
	 if(a<0)  temp=-a;  
	 else temp=a;
	 return temp;
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
