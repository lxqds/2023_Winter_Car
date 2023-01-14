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
CTRL CTRL_Position;
CTRL CTRL_Speed;


void CTRL_compute_Position2(void)
{
	float error;
	error = CTRL_Position.Setpoint - Encoder.Distance[2];
	
	CTRL_Position.Output = CTRL_Position.q1 * error;
	CTRL_Position.Output += CTRL_Position.q2 * CTRL_Position.Integral;
	CTRL_Position.Output += CTRL_Position.q3 * CTRL_Position.Integral;
}
/**
 * @name	位置环
 * @brief	位置环
 * @param	无
 * @return	无
 */
void CTRL_compute_Position(void)
{ 
	uint16_t Limit_MAXspeed = 100;
	move_pid.output = PID_realize(&move_pid,Encoder.Distance[2]);
	move_pid2.output = PID_realize(&move_pid2,Encoder.Distance[3]);
	
	if(move_pid.output > Limit_MAXspeed)
	{
		move_pid.output = Limit_MAXspeed;//限制轮子最高转速
	}
	else if(move_pid.output < -Limit_MAXspeed)
	{
		move_pid.output = -Limit_MAXspeed;//限制轮子最高转速
	}
	if(move_pid2.output > Limit_MAXspeed)
	{
		move_pid2.output = Limit_MAXspeed;//限制轮子最高转速
	}
	else if(move_pid2.output < -Limit_MAXspeed)
	{
		move_pid2.output = -Limit_MAXspeed;//限制轮子最高转速
	}
	speed_pid.target_val = move_pid.output;			//速度环输入
	speed_pid2.target_val = move_pid2.output;		//
}
/**
 * @name	速度环
 * @brief	速度环
 * @param	无
 * @return	无
 */
void CTRL_compute_Speed(void)
{
	speed_pid.output = PID_realize(&speed_pid,Encoder.Speed[2]);
	speed_pid2.output = PID_realize(&speed_pid2,Encoder.Speed[3]);
	
	if(speed_pid.output >90)
	{
		speed_pid.output = 90;
	}
	else if(speed_pid.output<-90)
	{
		speed_pid.output = -90;
	}
	if(speed_pid2.output >90)
	{
		speed_pid2.output = 90;
	}
	else if(speed_pid2.output<-90)
	{
		speed_pid2.output = -90;
	}
}

/**
 * @name	Car_Go_Distance
 * @brief	车运行走距离x
 * @param	目标距离
 * @return	1到达，0未到达
 */
bool Car_Go_Distance(float Distance1,float Distance2)
{	
	set_pid_target(&move_pid,Distance1);
	set_pid_target(&move_pid2,Distance2);
	
	if((Encoder.Distance[2]>=Distance1-0.1)&&(Encoder.Distance[3]>=Distance2-0.1))
	{
		return 1;
	}else
	{
		return 0;
	}
}

bool Spin_Turn(uint8_t Angle)
{
	if(Angle ==0)
	{
		if(Car_Go_Distance(-10.5,10.5))
			return 1;
		else
			return 0;
	}else 
	if(Angle ==1)
	{
		if(Car_Go_Distance(-10.5,10.5))
			return 1;
		else
			return 0;
	}else
	if(Angle ==2)
	{
		if(Car_Go_Distance(21,-21))
			return 1;
		else
			return 0;
	}
	
}

/*****************************************************END OF FILE*********************************************************/	
