#ifndef __VOICE_H
#define __VOICE_H

#define  USART1_BaudRate  115200

#include <string.h>
#include "stdio.h"	
#include "string.h" 
#include "stdarg.h"
#include "stm32f4xx.h"

void  Voice_Config(void);

/*	char pro[20] = {0x41,0x46,0x3a,0x33,0x30}; //增大声音
	char chain[20] = {0xd2,0xbb,0xba,0xc5,0xb2,0xd6,0xc3,0xc5}; //1号仓
	printf("%s", pro);
    printf("%s",chain);
*/
#endif //__VOICE_H