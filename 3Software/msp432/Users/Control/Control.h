/**
  *************************************************************************************************************************
  * @file    Control.h
  * @author  lxq
  * @version V1.0
  * @date    2023-01-03
  * @brief   Control模块.h文件配置
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */


/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __CONTROL_H
#define	__CONTROL_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "bsp_pid.h"
/* define定义 -----------------------------------------------------------------------------------------------------------------*/
typedef struct 
{
	float q1,q2,q3,q4;
	float Setpoint,Output,Integral,Last_error;
	uint32_t timer;
}CTRL;
/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
void CTRL_compute_Position(void);
void CTRL_compute_Speed(void);
void CTRL_compute_Direction(int16_t Bias);


void Car_Go(float Distance);
void Car_Spin(uint8_t Direction);

bool Car_Go_Distance(float Distance1,float Distance2);
bool Spin_Turn(uint8_t Angle);
#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/	
