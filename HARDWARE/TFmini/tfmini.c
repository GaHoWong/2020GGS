#include "tfmini.h"
#include "led.h"
 
#define USART3_REC_LEN  	200  	//定义最大接收字节数 200
#define EN_USART3_RX 		1		//使能（1）/禁止（0）串口3接收
 
#define Data_Head         0X59   
#define Data_Length       9  
 
///////////////

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}


 /**
  * @brief  RS232_USART GPIO 配置,工作模式配置。115200 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB1PeriphClockCmd(RS232_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE, RS232_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串口RS232_USART 模式 */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &USART_InitStructure); 
	
	NVIC_Configuration();
	/*配置串口接收中断*/
	USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(RS232_USART, ENABLE);
}





u8 USART2_RX_BUF[200];  

u16 flag_REC,count_REC;


u8 Distance;
u8 Strength;

void RS232_USART_IRQHandler(void)
{
	static u8 flag_data = 0;
	static u8 index = 0;
	u16 CheckSum = 0;
	u8 i;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE))//接收中断
	{	

			printf("USART2_GetITStatus!!");
		if(USART_ReceiveData(USART2) == Data_Head)
			flag_data = 1;
		
		if(flag_data)
		{
			USART2_RX_BUF[index ++] = USART_ReceiveData(USART2);
			
			//接收完毕进行数据校验
			if(index == Data_Length)
			{
				//如果第一位和第二位是0x59
				if((USART2_RX_BUF[0] == Data_Head) && (USART2_RX_BUF[1] == Data_Head))
				{
					//将接收到的数据累加
					for(i = 0; i < (Data_Length - 1); i ++)
					{
						CheckSum += USART2_RX_BUF[i];
					}
					//进行CheckSum校验
					if((CheckSum & 0x00ff) == USART2_RX_BUF[8])
					{
						//计算距离
						Distance = USART2_RX_BUF[2] + USART2_RX_BUF[3] * 256;
						//计算信号强度
						Strength = USART2_RX_BUF[4] + USART2_RX_BUF[5] * 256;
						//接收完成标志
						flag_REC = 1;
						count_REC ++;
						printf("Distance = %d\r\n",Distance);
						printf("Strength = %d\r\n",Strength);
					}
					index = 0;
					flag_data = 0;
				}
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}













