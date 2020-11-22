#include "pca9685.h"
#include "delay.h"
#include <math.h>
#define YANSHI 20   //���ÿתʮ�ȼ���200ms

u16 TURN_1;          //�����ĸ�ƫת�Ƕȱ���
u16 TURN_2;
u16 TURN_3;
u16 TURN_4;

u16  BEFORE[4];    //���ڴ�����һ�ε�ƫת�Ƕ�


void PCA9685_write(unsigned char reg,unsigned char data)//PCA9685д�ֽ�
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

u8 PCA9685_read(unsigned char reg)//PCA9685���ֽ�
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
    
void setPWMFreq(u8 freq) //����pwmƵ��
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


/*���ÿ���  
	����Ӧ�ļĴ�����LED0_ON_L��LED0_ON_H��LED0_OFF_L��LED0_OFF_H��дֵ����
����numΪ���PWM�������0~15��on��PWM��������ֵ0~4096,off��PWM�½�����ֵ0~4096
PWM����ԭ���������ģ�

һ��PWM���ڷֳ�4096�ݣ���0��ʼ+1�������Ƶ�onʱ����Ϊ�ߵ�ƽ������������offʱ����Ϊ�͵�ƽ��ֱ������4096���¿�ʼ��
���Ե�on������0ʱ������ʱ,��on����0ʱ��off/4096��ֵ����PWM��ռ�ձȡ�
*/
void setPWM(u8 num, u16 on, u16 off) 
{
	PCA9685_write(LED0_ON_L+4*num,on);
	PCA9685_write(LED0_ON_H+4*num,on>>8);
	PCA9685_write(LED0_OFF_L+4*num,off);
	PCA9685_write(LED0_OFF_H+4*num,off>>8);
}
//PWM=4096��(0.5+(2.5-0.5)��angle/180)/20
//����20ms������������Ƶ�ʵ�ʱ��Ҫ��Ϊ50��ռ�ձ�0.5ms~2.5ms�ֱ��Ӧ0��~180�㣬�ҳ����Թ�ϵ�����Լ��㹫ʽΪ

u16 calculate_PWM(u8 angle)//�õ��Ƕ�
{
	return (int)(204.8*(0.5+angle*0.9/90));//ʵ����Ӧ�ó���0.9���õ��Ľ����Ϊ׼ȷ
}



//�����Ż��Ľ�������̫���ˣ�ǣ���Ķ����е��Ŷ��ÿһ�ζ���Ҫ�˼������ͬʱ����ռ��һ�����ڣ���Ȼʱ��������
void RETURN(u16 *BEFORE,u16 TURN_1,u16 TURN_2,u16 TURN_3,u16 TURN_4)//�����ƽ���˶�����ֹ�˶����죬����ֵ�ֱ�Ϊ�ĸ������ֵ
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


void up()//�����еĶ������	
{
	u16 pwm = calculate_PWM(90);	
	setPWM(0x0,0,pwm);
	delay_ms(1);
	setPWM(0x1,0,pwm);
	delay_ms(1);
	setPWM(0x2,0,pwm);
	delay_ms(1);
	setPWM(0x3,0,calculate_PWM(40));//�оٶ����40Ϊ���ʵ�ֵ��70Ϊ��ȡ����
	delay_ms(1);
	setPWM(0x4,0,pwm);
	delay_ms(1);
	setPWM(0x5,0,pwm);
	delay_ms(1);
	setPWM(0x6,0,pwm);
	delay_ms(1);
	setPWM(0x7,0,pwm);
}

void EXPECT(u16 expect_1,u16 expect_2,u16 expect_3,u16 expect_4)//����ֵ����������Ԥ��Ŀ���λ�ã����ƶ���˶�����λ��
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

//void RETURN(u16 TURN_1,u16 TURN_2,u16 TURN_3,u16 TURN_4)//�ö��ת��һ���ĽǶ�
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


