#ifndef __PCA9685_H
#define __PCA9685_H
#include "sys.h"
#include "myiic.h"

#define PCA9685_adrr 0x40
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4


#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE


#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9


#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
extern u16 TURN_1;
extern u16 TURN_2;
extern u16 TURN_3;
extern u16 TURN_4;
extern u16  BEFORE[4];


void PCA9685_write(unsigned char reg,unsigned char data);
u8 PCA9685_read(unsigned char reg);

//void SLOWER_1(u16 BEFORE,u16 TURN_1);
//void SLOWER_2(u16 BEFORE,u16 TURN_2);
//void SLOWER_3(u16 BEFORE,u16 TURN_3);
//void SLOWER_4(u16 BEFORE,u16 TURN_4);
//void RETURN(u16 TURN_1,u16 TURN_2,u16 TURN_3,u16 TURN_4);

void setPWMFreq(u8 freq);
void setPWM(u8 num, u16 on, u16 off);
void RETURN(u16 *BEFORE,u16 TURN_1,u16 TURN_2,u16 TURN_3,u16 TURN_4);
void EXPECT(u16 expect_1,u16 expect_2,u16 expect_3,u16 expect_4);
void up(void);

#endif

