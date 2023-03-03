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
#include "myconfig.h"
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
 * @name	转向环
 * @brief	转向环
 * @param	无
 * @return	无
 */
void CTRL_compute_Direction(int16_t Bias)
{
	Dir_pid.output = PID_realize(&Dir_pid,Bias);
	Dir_pid2.output = PID_realize(&Dir_pid2,Bias);
	
	if(Dir_pid.output >90)
	{
		Dir_pid.output = 90;
	}
	else if(Dir_pid.output<-90)
	{
		Dir_pid.output = -90;
	}
	if(Dir_pid2.output >90)
	{
		Dir_pid2.output = 90;
	}
	else if(Dir_pid2.output<-90)
	{
		Dir_pid2.output = -90;
	}
	
}
/**
 * @name	位置环
 * @brief	位置环
 * @param	无
 * @return	无
 */
void CTRL_compute_Position(void)
{
	uint16_t Limit_MAXspeed = 40;
	move_pid.output = PID_realize(&move_pid,Encoder.Distance[2]);
	move_pid2.output = PID_realize(&move_pid2,Encoder.Distance[3]);
	move_pid3.output = PID_realize(&move_pid3,Encoder.Distance[0]);
	move_pid4.output = PID_realize(&move_pid4,Encoder.Distance[1]);
	
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
	
	if(move_pid3.output > Limit_MAXspeed)
	{
		move_pid3.output = Limit_MAXspeed;//限制轮子最高转速
	}
	else if(move_pid3.output < -Limit_MAXspeed)
	{
		move_pid3.output = -Limit_MAXspeed;//限制轮子最高转速
	}
	if(move_pid4.output > Limit_MAXspeed)
	{
		move_pid4.output = Limit_MAXspeed;//限制轮子最高转速
	}
	else if(move_pid4.output < -Limit_MAXspeed)
	{
		move_pid4.output = -Limit_MAXspeed;//限制轮子最高转速
	}
	speed_pid.target_val = move_pid.output;			//速度环输入
	speed_pid2.target_val = move_pid2.output;		//
	speed_pid3.target_val = move_pid3.output;			//速度环输入
	speed_pid4.target_val = move_pid4.output;		//
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
	speed_pid3.output = PID_realize(&speed_pid3,Encoder.Speed[0]);
	speed_pid4.output = PID_realize(&speed_pid4,Encoder.Speed[1]);
//	if(speed_pid.output - speed_pid.Last_output > 5)// 限制增量为5
//	{
//		speed_pid.output = speed_pid.Last_output + 5;
//	}   
//	 speed_pid.Last_output = speed_pid.output;
//	if(speed_pid2.output - speed_pid2.Last_output > 5)// 限制增量为5
//	{
//		speed_pid2.output = speed_pid2.Last_output + 5;
//	}
//	 speed_pid2.Last_output = speed_pid2.output;
	
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
	
	if(speed_pid3.output >90)
	{
		speed_pid3.output = 90;
	}
	else if(speed_pid3.output<-90)
	{
		speed_pid3.output = -90;
	}
	if(speed_pid4.output >90)
	{
		speed_pid4.output = 90;
	}
	else if(speed_pid4.output<-90)
	{
		speed_pid4.output = -90;
	}
}

void Car_Go(float Distance)
{
	Encoder.Distance[0] = 0;
	Encoder.Distance[1] = 0;
	Encoder.Distance[2] = 0;
	Encoder.Distance[3] = 0;

	Flag.Stop_Flag =0;
	Flag.Start_Line_Flag = 1;
	Flag.Target_Distance_Left = Distance;
	Flag.Target_Distance_Right = Distance;
	Flag.Is_EnMOTOR = 1;
	Flag.CarStart_Flag = 1;
	
	set_pid_target(&move_pid,Flag.Target_Distance_Left);
		set_pid_target(&move_pid3,Flag.Target_Distance_Left);
	set_pid_target(&move_pid2,Flag.Target_Distance_Right);
		set_pid_target(&move_pid4,Flag.Target_Distance_Right);
}
void Car_Spin(uint8_t Direction)
{
	float Distance_Left,Distance_Right;
	
	
	Encoder.Distance[2] = 0;
	Encoder.Distance[3] = 0;
	
	switch(Direction)
	{
		case 0: Distance_Left=-10.5f;Distance_Right=10.5f;break;
		case 1: Distance_Left=10.5f;Distance_Right=-10.5f;break;
		case 2: Distance_Left=-21;Distance_Right=21;break;
		default:break;
	}
	Flag.Target_Distance_Left  = Distance_Left;
	Flag.Target_Distance_Right = Distance_Right;
	
	Flag.Stop_Flag =0;//走之前将停止标志位置0
	Flag.Spin_Start_Flag = 1;
	Flag.Is_EnMOTOR = 1;
	Flag.CarStart_Flag = 1;
	
	set_pid_target(&move_pid,Flag.Target_Distance_Left);
	set_pid_target(&move_pid2,Flag.Target_Distance_Right);
	set_pid_target(&move_pid3,Flag.Target_Distance_Left);
	set_pid_target(&move_pid4,Flag.Target_Distance_Right);
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
	
	if((Encoder.Distance[2]>=Distance1-0.1f)&&(Encoder.Distance[3]>=Distance2-0.1f))
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
	}
	else
	if(Angle ==1)
	{
		if(Car_Go_Distance(-10.5,10.5))
			return 1;
		else
			return 0;
	}
	else
	if(Angle ==2)
	{
		if(Car_Go_Distance(21,-21))
			return 1;
		else
			return 0;
	}
	return 1;
}
/*****************************************************END OF FILE*********************************************************/	
