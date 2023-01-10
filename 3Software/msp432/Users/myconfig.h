/**
  *************************************************************************************************************************
  * @file    myconfig.h
  * @author  lxq
  * @version V1.0
  * @date    2023-01-03
  * @brief   xx模块.h文件配置
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */


/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __MYCONFIG_H
#define	__MYCONFIG_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "key.h"
#include "led.h"
#include "oled.h"
#include "Reflectance.h"
#include "timA.h"
#include "bsp_Servo.h"

#include "bsp_MPU6050.h"
#include "inv_mpu.h"
/* define定义 -----------------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	Main_State,
	State1,
	State2,
}Menu_State;

typedef struct 
{
	uint8_t Distance_Flag;
	uint8_t CarStart_Flag;
}Flag_Init;

/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
extern 	uint8_t Key;
extern	uint8_t Reflectance_Data;






#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/	
