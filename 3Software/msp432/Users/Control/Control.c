/**
  *************************************************************************************************************************
  * @file    Control.c
  * @author  lxq
  * @version V1.0
  * @date    2023-01-03
  * @brief   Control模块.c文件配置
  *************************************************************************************************************************
  * @attention
  *
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "Control.h"
/* define -----------------------------------------------------------------------------------------------------------------*/
typedef struct 
{
	
}Routine_Init;
/**
 * @name	Car_Go_Distance
 * @brief	车运行以速度v走距离x
 * @param	目标距离，目标速度
 * @return	1到达，0未到达
 */
bool Car_Go_Distance(float Distance_cm,float Speed_cm_s)
{
	float PWMTemp1,PWMTemp2,PWMTemp3,PWMTemp4;
	PWMTemp1 = PID1_Run(&PID1_Position,Encoder.Distance[2],Distance_cm);//PID1运行
	PWMTemp2 = PID2_Run(&PID2_Velocity,Encoder.Distance[2],Speed_cm_s);
	PWMTemp3 = PID1_Run(&PID1_Position,Encoder.Distance[3],Distance_cm);//PID1运行
	PWMTemp4 = PID2_Run(&PID2_Velocity,Encoder.Distance[3],Speed_cm_s);
	
	Set_PWM(PWMTemp1+PWMTemp2,PWMTemp3+PWMTemp4);
	if(Encoder.Distance[2] == Distance_cm)
	{
		return 1;
	}else
	{
		return 0;
	}
	
}
/*****************************************************END OF FILE*********************************************************/	
