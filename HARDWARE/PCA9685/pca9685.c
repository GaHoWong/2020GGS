#include "pca9685.h"
#include "delay.h"
#include <math.h>
#define YANSHI 20   //舵机每转十度减速200ms

u16 TURN_1;          //定义四个偏转角度变量
u16 TURN_2;
u16 TURN_3;
u16 TURN_4;

u16  BEFORE[4];    //用于储存上一次的偏转角度


void PCA9685_write(unsigned char reg,unsigned char data)//PCA9685写字节
{
    IIC_Start();
    IIC_Send_Byte(PCA9685_adrr);
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    IIC_Wait_Ack();
    IIC_Stop();
}

u8 PCA9685_read(unsigned char reg)//PCA9685读字节
{
    u8 res;
    IIC_Start();
    IIC_Send_Byte(PCA9685_adrr);
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();    
		IIC_Start();                
    IIC_Send_Byte(PCA9685_adrr|0X01);
    IIC_Wait_Ack();
    res=IIC_Read_Byte(0);		
    IIC_Stop();             
    return res;  
}
    
void setPWMFreq(u8 freq) //设置pwm频率
{
   u8 prescale,oldmode,newmode;
	 double prescaleval;
	 prescaleval = 25000000.0/(4096*freq*0.915);
   prescale = (u8)floor(prescaleval+0.5)-1;

   oldmode = PCA9685_read(PCA9685_MODE1);
   newmode = (oldmode&0x7F) | 0x10; // sleep
   PCA9685_write(PCA9685_MODE1, newmode); // go to sleep
   PCA9685_write(PCA9685_PRESCALE, prescale); // set the prescaler
   PCA9685_write(PCA9685_MODE1, oldmode);
   delay_ms(5);
   PCA9685_write(PCA9685_MODE1, oldmode | 0xa1); 
}


/*设置开关  
	向相应的寄存器（LED0_ON_L、LED0_ON_H、LED0_OFF_L、LED0_OFF_H）写值即可
其中num为舵机PWM输出引脚0~15，on是PWM上升计数值0~4096,off是PWM下降计数值0~4096
PWM发生原理是这样的：

一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时跳变为低电平，直到计满4096重新开始。
所以当on不等于0时可作延时,当on等于0时，off/4096的值就是PWM的占空比。
*/
void setPWM(u8 num, u16 on, u16 off) 
{
	PCA9685_write(LED0_ON_L+4*num,on);
	PCA9685_write(LED0_ON_H+4*num,on>>8);
	PCA9685_write(LED0_OFF_L+4*num,off);
	PCA9685_write(LED0_OFF_H+4*num,off>>8);
}
//PWM=4096×(0.5+(2.5-0.5)×angle/180)/20
//周期20ms，所以在设置频率的时候要设为50；占空比0.5ms~2.5ms分别对应0°~180°，且成线性关系，所以计算公式为

u16 calculate_PWM(u8 angle)//得到角度
{
	return (int)(204.8*(0.5+angle*0.9/90));//实际中应该乘以0.9，得到的结果较为准确
}



//可以优化改进，但是太懒了，牵扯的东西有点多哦，每一次都需要人几个舵机同时动，占满一个周期，不然时序发生错误
void RETURN(u16 *BEFORE,u16 TURN_1,u16 TURN_2,u16 TURN_3,u16 TURN_4)//舵机的平缓运动，防止运动过快，输入值分别为四个舵机的值
{
	u16 pwm = calculate_PWM(90);
	u16 i;
	if(TURN_1>BEFORE[0])
	{	
		for(i=BEFORE[0];i<TURN_1;i++)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(BEFORE[1]));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(BEFORE[2]));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(BEFORE[3]));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(TURN_1));
		delay_ms(1);
		setPWM(0x1,0,pwm);
		delay_ms(1);
		setPWM(0x2,0,pwm);
		delay_ms(1);
		setPWM(0x3,0,pwm);
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);
	}
	else
	{
		for(i=BEFORE[0];i>TURN_1;i--)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(BEFORE[1]));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(BEFORE[2]));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(BEFORE[3]));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(TURN_1));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(BEFORE[1]));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(BEFORE[2]));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(BEFORE[3]));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);		
	}
	
	BEFORE[0]=TURN_1;	
	
	if(TURN_2>BEFORE[1])
	{	
		for(i=BEFORE[1];i<TURN_2;i++)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(BEFORE[0]));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(BEFORE[2]));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(BEFORE[3]));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(BEFORE[0]));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(TURN_2));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(BEFORE[2]));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(BEFORE[3]));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);
	}
	else
	{
		for(i=BEFORE[1];i>TURN_2;i--)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(BEFORE[0]));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(BEFORE[2]));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(BEFORE[3]));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(BEFORE[0]));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(TURN_2));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(BEFORE[2]));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(BEFORE[3]));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);		
	}
	
	BEFORE[1]=TURN_2;
	
	if(TURN_3>BEFORE[2])
	{	
		for(i=BEFORE[2];i<TURN_3;i++)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(BEFORE[0]));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(BEFORE[1]));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(BEFORE[3]));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(BEFORE[0]));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(BEFORE[1]));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(TURN_2));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(BEFORE[3]));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);
	}
	else
	{
		for(i=BEFORE[2];i>TURN_3;i--)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(BEFORE[0]));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(BEFORE[1]));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(BEFORE[3]));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(BEFORE[0]));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(BEFORE[1]));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(TURN_3));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(BEFORE[3]));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);		
	}
	BEFORE[2]=TURN_3;
	
	if(TURN_4>BEFORE[3])
	{	
		for(i=BEFORE[3];i<TURN_4;i++)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(BEFORE[0]));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(BEFORE[1]));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(BEFORE[2]));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(BEFORE[0]));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(BEFORE[1]));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(BEFORE[2]));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(TURN_4));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);
	}
	else
	{
		for(i=BEFORE[3];i>TURN_4;i--)
		{
			if(i%10==0)
			{
				delay_ms(YANSHI);
				setPWM(0x0,0,calculate_PWM(BEFORE[0]));
				delay_ms(1);
				setPWM(0x1,0,calculate_PWM(BEFORE[1]));
				delay_ms(1);
				setPWM(0x2,0,calculate_PWM(BEFORE[2]));
				delay_ms(1);
				setPWM(0x3,0,calculate_PWM(i));
				delay_ms(1);
				setPWM(0x4,0,pwm);
				delay_ms(1);
				setPWM(0x5,0,pwm);
				delay_ms(1);
				setPWM(0x6,0,pwm);
				delay_ms(1);
				setPWM(0x7,0,pwm);				
			}
		}
		setPWM(0x0,0,calculate_PWM(BEFORE[0]));
		delay_ms(1);
		setPWM(0x1,0,calculate_PWM(BEFORE[1]));
		delay_ms(1);
		setPWM(0x2,0,calculate_PWM(BEFORE[2]));
		delay_ms(1);
		setPWM(0x3,0,calculate_PWM(TURN_4));
		delay_ms(1);
		setPWM(0x4,0,pwm);
		delay_ms(1);
		setPWM(0x5,0,pwm);
		delay_ms(1);
		setPWM(0x6,0,pwm);
		delay_ms(1);
		setPWM(0x7,0,pwm);		
	}
	BEFORE[3]=TURN_4;
	
}


void up()//让所有的舵机举升	
{
	u16 pwm = calculate_PWM(90);	
	setPWM(0x0,0,pwm);
	delay_ms(1);
	setPWM(0x1,0,pwm);
	delay_ms(1);
	setPWM(0x2,0,pwm);
	delay_ms(1);
	setPWM(0x3,0,calculate_PWM(40));//夹举舵机，40为合适的值，70为夹取动作
	delay_ms(1);
	setPWM(0x4,0,pwm);
	delay_ms(1);
	setPWM(0x5,0,pwm);
	delay_ms(1);
	setPWM(0x6,0,pwm);
	delay_ms(1);
	setPWM(0x7,0,pwm);
}

void EXPECT(u16 expect_1,u16 expect_2,u16 expect_3,u16 expect_4)//期望值函数，给出预期目标的位置，控制舵机运动到此位置
{
	RETURN(BEFORE,expect_1,TURN_2,TURN_3,TURN_4);
	delay_ms(100);
	RETURN(BEFORE,TURN_1,expect_2,TURN_3,TURN_4);
	delay_ms(100);
	RETURN(BEFORE,TURN_1,TURN_2,expect_3,TURN_4);
	delay_ms(100);
	RETURN(BEFORE,TURN_1,TURN_2,TURN_3,expect_4);
	delay_ms(100);
}

//void RETURN(u16 TURN_1,u16 TURN_2,u16 TURN_3,u16 TURN_4)//让舵机转动一定的角度
//{
//	u16 i;
//	u16 pwm = calculate_PWM(10);   
//	setPWM(0x0,0,calculate_PWM(TURN_1));
//	delay_ms(1);
//	setPWM(0x1,0,calculate_PWM(TURN_2));
//	delay_ms(1);
//	setPWM(0x2,0,calculate_PWM(TURN_3));
//	delay_ms(1);
//	setPWM(0x3,0,calculate_PWM(TURN_4));
//	delay_ms(1);
//	setPWM(0x4,0,pwm);
//	delay_ms(1);
//	setPWM(0x5,0,pwm);
//	delay_ms(1);
//	setPWM(0x6,0,pwm);
//	delay_ms(1);
//	setPWM(0x7,0,pwm);
////	SLOWER_1(BEFORE[0],TURN_1);
//	BEFORE[0]=TURN_1;
//	BEFORE[1]=TURN_2;
//	BEFORE[2]=TURN_3;
//	BEFORE[3]=TURN_4;
//	
//}


