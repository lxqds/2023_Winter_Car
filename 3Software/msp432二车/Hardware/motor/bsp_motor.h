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


#define MotorA 1;
#define MotorB 2;
#define MotorC 3;
#define MotorD 4;

#define Motor_Forward	1;
#define Motor_Back		0;
#define Motor_Stop 		3;

void Motor_Init(void);
void Motor_Control(uint8_t Motorx,uint8_t Dir,uint8_t PWM);
void Set_PWM(float PWM1,float PWM2);
void Set_PWM2(float PWM1,float PWM2);
void set_motor_enable(void);
void set_motor_disable(void);
//void motor_Init(void);
//void motor2_Init(void);

//void PWMB_Init(void);


/**************************************************************/

//void motor_all_control(uint8_t motorx,uint16_t m_command);
//void motor_speed_all(uint8_t motorx,uint16_t m_speed);
//void Motor_right(void);
//void Motor_left(void);
//void Motor_forward(void);
//void Motor_back(void);
//void Motor_stop_all(void);

//void Motor_Right_1(void);void Motor_Right_2(void);void Motor_Right_3(void);void Motor_Right_4(void);
//void Motor_Left_1(void);void Motor_Left_2(void);void Motor_Left_3(void);void Motor_Left_4(void);

#endif
