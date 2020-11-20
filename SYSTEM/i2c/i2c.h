#ifndef __I2C_H
#define __I2C_H	 
#include "sys.h"

#define I2CPORT		GPIOB	//定义IO接口
#define I2C_SCL		GPIO_Pin_6	//定义IO接口
#define I2C_SDA		GPIO_Pin_7	//定义IO接口

#define HostAddress	0xc0	//总线主机的器件地址
#define BusSpeed	200000	




/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS7      0X0A   
/* STM32 I2C 快速模式 */
#define I2C_Speed              200000  //总线速度（最好不高于400000）

/*I2C接口*/
#define I2C1_CLK                      RCC_APB1Periph_I2C1
#define I2C1_CLK_INIT							  	RCC_APB1PeriphClockCmd

#define I2C1_SCL_PIN                  GPIO_Pin_8                 
#define I2C1_SCL_GPIO_PORT            GPIOB                       
#define I2C1_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2C1_SCL_SOURCE               GPIO_PinSource8
#define I2C1_SCL_AF                   GPIO_AF_I2C1

#define I2C1_SDA_PIN                  GPIO_Pin_9                  
#define I2C1_SDA_GPIO_PORT            GPIOB                       
#define I2C1_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2C1_SDA_SOURCE               GPIO_PinSource9
#define I2C1_SDA_AF                   GPIO_AF_I2C1

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


//void I2C_Configuration(void);
//void I2C_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
//uint32_t I2C_ByteWrite(u8* pBuffer, u8 WriteAddr);
//uint32_t I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
//uint32_t I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
//void I2C_WaitEepromStandbyState(void);


void I2C_Configuration(void);
void I2C_SAND_BUFFER(u8 SlaveAddr, u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite);
void I2C_SAND_BYTE(u8 SlaveAddr,u8 writeAddr,u8 pBuffer);
void I2C_READ_BUFFER(u8 SlaveAddr,u8 readAddr,u8* pBuffer,u16 NumByteToRead);
u8 I2C_READ_BYTE(u8 SlaveAddr,u8 readAddr);
		 				    
#endif
