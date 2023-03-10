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
#include "tim32.h"
#include "bsp_Servo.h"
#include "sysinit.h"
#include "usart.h"

#include "bsp_MPU6050.h"
#include "inv_mpu.h"

#include "protocol.h"
#include "bsp_pid.h"
#include "control.h"

#include "k210.h"
/* define定义 -----------------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	Main_State,
	State1,
	State2,
}Menu_State;

typedef struct 
{
	uint8_t Is_EnMOTOR;
	uint8_t Stop_Flag;
	uint8_t Start_Line_Flag;
	uint8_t Turn_Flag;
	uint8_t Spin_Start_Flag;
	uint8_t Spin_Succeed_Flag;
	uint8_t Stop_Count;
	uint8_t Spin_count;
	uint8_t Distance_Flag;
	uint8_t CarStart_Flag;
	float  Target_Distance_Left;	//左轮
	float Target_Distance_Right;//右轮
	
	uint8_t Bias_Left;
	uint8_t Bias_Right;
	uint8_t Bias_Middle;
	int16_t Bias;//偏差
	int16_t Last_Bias;//偏差
	uint8_t CrossRoad_Flag;
	
	uint8_t Load_drug;
	uint8_t Target_Num;
	uint8_t Target_Num_Flag;
	uint8_t Step_Count;//用于处理识别目标后所做的路线步骤
	
	uint8_t Target_Distance_Arrive;//距离到达标志位
	
	
	uint8_t Recognize_Num_Count;//识别数字的数量判断
	uint8_t Recognize_Num_Flag;//识别到数字
	uint8_t Recognize_Num;//识别到的数字
	uint8_t Recognize_Num2;//识别数字滤波
	
	uint8_t GO_Flag;
	uint8_t Back_Flag;
	
	uint8_t Routine[9];//记忆路线
	uint8_t Current_Position;
	uint8_t Task;
	
	uint8_t White_Count;//跑偏计数
	uint8_t White_Flag;//空白标志位
	
	uint8_t Servo_Scan_Flag;//舵机开始扫描标志位
	uint8_t Servo_Scan_Flag2;//舵机开始扫描标志位2
	uint8_t Servo_Scan_Flag3;//舵机开始扫描标志位2
	
	uint8_t Num_Recognize[8];
	uint8_t Num_Recognize2[8];
	uint8_t Num_Recognize3[8];
	uint8_t Num_Angle[8];
	uint8_t Num_Angle2[8];
	uint8_t Num_Angle3[8];

}Flag_Init;

/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
extern 	uint8_t Key;
extern	uint8_t Reflectance_Data;
extern 	uint8_t Last_Reflectance_Data;

extern Flag_Init Flag;
extern  SensorData SensorData1;


#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/	
