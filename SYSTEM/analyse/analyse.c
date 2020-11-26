#include "analyse.h"



int sign;
int sign_x;
int sign_y;



/**
* @brief  analyse
* @param  分析K210/OPEN MV/树莓派的数据
* @retval OPEN MV 最大分辨率 x=160  y=120，因此中心点坐标为(80,60) 
*/
void analyse(u8 data)
{
	
	
	//printf("%s",data);
	static u8 RxBuffer[10];
	static u8 state = 0;
/*通信格式 0xAA data0 data1 data2 checkout 0x54*/	
/*其中checkout=(data1+data2)低八位  比如 data0 = 0x00,data1=0xe1,data2=0xf3,data1+data2=0x1d4,则checkout=0xd4*/
	if(state==0&&data==0xAA)
		state=1;
	else if(state==1)
	{
		RxBuffer[0]=data; //元素标志
		state=2;
	}
	else if(state==2)
	{
		RxBuffer[1]=data; //x坐标
		state=3;
	}
	else if(state==3)
	{	
		RxBuffer[2]=data; //y坐标
		state = 4;
	}
	else if(state==4)
	{	
		RxBuffer[3]=data;//checkout
		state = 5;
	}

	else if(state==5&&data==0x54)
	{	
		if(RxBuffer[3]==(u8)(RxBuffer[1]+RxBuffer[2]))//校验成功
		{
		sign  = RxBuffer[0];
		sign_x=RxBuffer[1];
		sign_y=RxBuffer[2];

		}
	
		state = 0;
	}
	else
		state = 0;
}




