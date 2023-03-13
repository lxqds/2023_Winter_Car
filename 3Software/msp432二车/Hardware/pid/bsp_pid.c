/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   PID算法实现
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_pid.h"


/* 定义全局变量 */
_pid speed_pid,move_pid;
_pid speed_pid2,move_pid2;
_pid speed_pid3,move_pid3;
_pid speed_pid4,move_pid4;
_pid Dir_pid,Dir_pid2;
float set_point=0.0;
int pid_status=0;

/**
  * @brief  PID参数初始化
  *	@note 	初始化的pid
  * @retval 无
  */
void PID_param_init(_pid *pid)
{
	/* 初始化位置环PID参数 */
  pid->target_val=0.0;				
  pid->actual_val=0.0;
  pid->err=0.0;
  pid->err_last=0.0;
  pid->integral=0.0;
  pid->Kp = 0.5;
  pid->Ki = 0.0;
  pid->Kd = 0.0;
  
  #if PID_ASSISTANT_EN
    float move_pid_temp[3] = {move_pid.Kp, move_pid.Ki, move_pid.Kd};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, move_pid_temp, 3);// 给通道 1 发送 P I D 值
  #endif
	delay_ms(10);
	/* 初始化速度环PID参数 */
//  speed_pid.target_val=0.0;				
//  speed_pid.actual_val=0.0;
//  speed_pid.err=0.0;
//  speed_pid.err_last=0.0;
//  speed_pid.integral=0.0;
//  speed_pid.Kp = 0.01;
//  speed_pid.Ki = 0.093;
//  speed_pid.Kd = 0.09;
//  
//  speed_pid2.target_val=0.0;				
//  speed_pid2.actual_val=0.0;
//  speed_pid2.err=0.0;
//  speed_pid2.err_last=0.0;
//  speed_pid2.integral=0.0;
//  speed_pid2.Kp = 0.01;
//  speed_pid2.Ki = 0.093;
//  speed_pid2.Kd = 0.09;
  #if PID_ASSISTANT_EN
    float speed_pid_temp[3] = {speed_pid.Kp, speed_pid.Ki, speed_pid.Kd};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, speed_pid_temp, 3);// 给通道 1 发送 P I D 值
  #endif
}

/**
  * @brief  设置目标值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
  *	@note 	无
  * @retval 目标值
  */
float get_pid_actual(_pid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
  *	@note 	无
  * @retval 无
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // 设置比例系数 P
		pid->Ki = i;    // 设置积分系数 I
		pid->Kd = d;    // 设置微分系数 D
}

/**
  * @brief  位置式PID算法实现
  * @param  val：当前实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float PID_realize(_pid *pid, float actual_val) 
{
  /*传入实际值*/
  pid->actual_val = actual_val;
  /*计算目标值与实际值的误差*/
  pid->err = pid->target_val - pid->actual_val;

  /*误差累积*/
  pid->integral += pid->err;
  /*PID算法实现*/
  pid->actual_val = pid->Kp*pid->err+ pid->Ki*pid->integral+ pid->Kd*(pid->err-pid->err_last);
  /*误差传递*/
  pid->err_last = pid->err;
  /*PID算法实现，并返回计算值*/
  return pid->actual_val;
}
/**
  * @brief  PID值清空
  *	@note 	无
  * @retval 无
  */
void PID_Clear(_pid *pid)
{
	pid->actual_val = 0;
	pid->err = 0;
	pid->integral = 0;
	pid->actual_val = 0;
	pid->err_last = 0;
}
