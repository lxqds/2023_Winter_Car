#ifndef __BSP_PID_H
#define	__BSP_PID_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "protocol.h"
#include "usart.h"
#include "delay.h"

/*pid*/
typedef struct
{
  float target_val;           //目标值
  float actual_val;        		//实际值
  float err;             			//定义偏差值
  float err_last;          		//定义上一个偏差值
  float Kp,Ki,Kd;          		//定义比例、积分、微分系数
  float integral;          		//定义积分值
	float Last_output;			//上一次输出用于构造限幅滤波器
  float output;
}_pid;


extern _pid speed_pid,move_pid;
extern _pid speed_pid2,move_pid2;
extern _pid speed_pid3,move_pid3;
extern _pid speed_pid4,move_pid4;
extern _pid Dir_pid,Dir_pid2;//转向环

extern void PID_param_init(_pid *pid);
extern void set_pid_target(_pid *pid, float temp_val);
extern float get_pid_actual(_pid *pid);
extern void set_p_i_d(_pid *pid, float p, float i, float d);
extern float PID_realize(_pid *pid, float actual_val);
extern void time_period_fun(void);
extern void PID_Clear(_pid *pid);
#endif
