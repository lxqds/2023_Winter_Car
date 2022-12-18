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

#define xvnxina1    BITBAND_PERI(P6IN,7)
#define xvnxina2    BITBAND_PERI(P5IN,6)
#define xvnxina3    BITBAND_PERI(P5IN,7)
#define xvnxina4    BITBAND_PERI(P6IN,6)
#define xvnxina5    BITBAND_PERI(P9IN,5)

#define KEY1_PRES 1 //KEY1按下
#define KEY2_PRES 2 //KEY2按下

void KEY_Init(void);       //IO初始化
uint8_t KEY_Scan(uint8_t); //按键扫描函数
void xvanxian(void);

#endif
