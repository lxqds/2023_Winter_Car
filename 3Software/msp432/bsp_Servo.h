#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H

#define Servo1 1
#define Servo2 2
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
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

#endif
