/*
 * @Description: #de
 * @Author: Liang xiaoqi
 * @Date: 2022-07-25 14:47:54
 * @LastEditTime: 2022-07-27 16:14:37
 * @LastEditors: Liang xiaoqi
 */


#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define KEY1 BITBAND_PERI(P1IN, 1) //读取按键1
#define KEY2 BITBAND_PERI(P1IN, 4) //读取按键2

#define KEY1_PRES 1 //KEY1按下
#define KEY2_PRES 2 //KEY2按下

#define KEY_KEEP_TIMELENTH 100 //按键扫描判断长按时间长度

#define KEY1_ONCE_PRESS	1
#define KEY1_LONG_PRESS	2
#define KEY2_ONCE_PRESS	3
#define KEY2_LONG_PRESS	4

typedef enum
{
	Key_Press_Once=0,
	Key_Press_Again,
	Key_Press_Keep,
	Key_Press_Release,
}Key_State;


void KEY_Init(void);       //IO初始化
uint8_t KEY_Scan(uint8_t); //按键扫描函数

uint8_t Key_Scan(void);
#endif
