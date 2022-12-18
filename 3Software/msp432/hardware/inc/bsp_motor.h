/*
 * @Description: 
 * @Author: Liang xiaoqi
 * @Date: 2022-07-25 14:47:54
 * @LastEditTime: 2022-07-26 10:57:35
 * @LastEditors: Liang xiaoqi
 */
#ifndef __bsp_motor_H
#define __bsp_motor_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void motor_Init(void);
void motor2_Init(void);


void PWMB_Init(void);


/**************************************************************/

void motor_all_control(uint8_t motorx,uint16_t m_command);
void motor_speed_all(uint8_t motorx,uint16_t m_speed);
void Motor_right(void);
void Motor_left(void);
void Motor_forward(void);
void Motor_back(void);
void Motor_stop_all(void);

void Motor_Right_1(void);void Motor_Right_2(void);void Motor_Right_3(void);void Motor_Right_4(void);
void Motor_Left_1(void);void Motor_Left_2(void);void Motor_Left_3(void);void Motor_Left_4(void);

#endif
