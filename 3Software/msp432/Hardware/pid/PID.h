#ifndef __PID_H
#define __PID_H
/* Includes -------------------------------------------------------------------------------------------------------------*/

/* define定义 -----------------------------------------------------------------------------------------------------------------*/
//位置式PID
typedef struct _PID1
{
    float Target_Val;           //目标值
    float Actual_Val;           //实际值
    float Control_Val;        		//控制量
    float Err;             			//定义偏差值
    float Err_Last;          		//定义上一个偏差值
    float KP,KI,KD;          		//定义比例、积分、微分系数
    float Integral;          		//定义积分值
}_PID1;
//增量式PID
typedef struct _PID2
{
    float Target_Val;           //目标值
    float Actual_Val;           //实际值
    float Control_Val;        		//控制量
    float Err;             			//定义偏差值
    float Err_Last;          		//定义上一个偏差值
    float Err_LLast;                 //定义上上一个偏差值   
    float KP,KI,KD;          		//定义比例、积分、微分系数
}_PID2;

/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
extern _PID1 PID1_Position;
extern _PID2 PID2_Velocity;
void PID1_Param_Init(struct _PID1 * PID);
void PID1_Param_Input(struct _PID1 *PID,float KP,float KI,float KD);
float PID1_Run(struct _PID1 *PID,float Actual_Val,float Target_Val);
void PID2_Param_Init(struct _PID2 *PID);
void PID2_Param_Input(struct _PID2 * PID,float KP,float KI,float KD);
float PID2_Run(struct _PID2 *PID,float Actual_Val,float Target_Val);
void PID1_Input_Data(struct _PID1 *PID,float Actual_Data,float Target_Data);
void PID2_Input_Data(struct _PID2 *PID,float Actual_Data,float Target_Data);
	
#endif
