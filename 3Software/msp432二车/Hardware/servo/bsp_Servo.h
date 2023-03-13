#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define Servo1 1
#define Servo2 2

/**
 * @brief	初始化舵机
 * @param	无
 * @return	无
 */
void Servo_Init(void);
/**
 * @brief	舵机控制
 * @param	uint8_t Servox 		选择舵机Servo1或Servo2	1或2
 * @param	uint32_t Duty_Cycle	占空比0-99
 * @return	
 */
void Servo_Control(uint8_t Servox,uint16_t Duty_Cycle);
void Servo_Control2(uint8_t Servox,uint8_t Angle);
void Servo_Scan(void);
uint8_t Servo_Scan2(uint8_t Servox,uint8_t Left,uint8_t Right);
#endif
