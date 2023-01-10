/*
 * @Description: PID
 * @Author: Liang xiaoqi
 * @Date: 2022-08-20 08:37:40
 * @LastEditTime: 2022-08-20 18:24:45
 * @LastEditors: Liang xiaoqi
 */

#include "PID.h"
#include "stdio.h"

/* 
    PID使用调节步骤：
    1.通过—PID1定义一个PID结构体PID1
    2.将定义的PID1传入初始化函数PID1_Param_Init()中进行初始化 
    3.通过函数PID1_Patam_Input()进行KP，KI，KD的调节
    4.调用PID1_Input_Data()输入实际值和目标值到PID1中
    5.把PID1_Run()放入定时器中周期性运行

 */

_PID1 PID1_Position;//位置式PID
_PID2 PID2_Velocity;//增量式PID

/**
 * @brief: 初始化位置式PID
 * @param {PID1} PID
 * @retval: 无
 */
void PID1_Param_Init(struct _PID1 * PID)
{
    /* 初始化参数 */
    printf("PID1_Init Begin\n");
    (*PID).Target_Val = 0.0;//使用（*PID）是因为. 的优先级比*高
    (*PID).Control_Val = 0.0;
    (*PID).Err = 0.0;
    (*PID).Err_Last = 0.0;
    (*PID).Integral = 0.0;

    (*PID).KP = 0;
    (*PID).KI = 0;
    (*PID).KD = 0;
    printf("PID1_Init End \n");
}
/**
 * @brief:修改PID的参数
 * @param {_PID1 *} PID
 * @param {float} KP
 * @param {float} KI
 * @param {float} KD
 * @retval: 无 
 */
void PID1_Param_Input(struct _PID1 *PID,float KP,float KI,float KD)
{
    PID->KP = KP;
    PID->KI = KI;
    PID->KD = KD;
}
/**
 * @brief: PID运行
 * @param {_PID1} *PID
 * @param {float} Actual_Val
 * @param {float} Target_Val
 * @retval: PID的控制量
 */
float PID1_Run(struct _PID1 *PID,float Actual_Val,float Target_Val)
{
    /* 目标值与实际值的误差 */
    (*PID).Target_Val =  Target_Val;
    (*PID).Err = (*PID).Target_Val - Actual_Val;//
    /* 误差累积 */
    (*PID).Integral += (*PID).Err;
	if((*PID).Integral >2000)
		(*PID).Integral =2000;
	if((*PID).Integral<-2000)
		(*PID).Integral =-2000;
    /* PID 算法的实现 */
    (*PID).Control_Val = (*PID).KP * (*PID).Err + (*PID).KI *(*PID).Integral + (*PID).KD *((*PID).Err - (*PID).Err_Last);
    /* 误差传递 */
    (*PID).Err_Last = (*PID).Err;
    /* 返回的是经过PID运算的控制值 */
    return (*PID).Control_Val;
}

void PID2_Param_Init(struct _PID2 *PID)
{
    /* 初始化参数 */
    printf("PID2_Init Begin\n");
    (*PID).Target_Val = 0.0;
    (*PID).Control_Val = 0.0;
    (*PID).Err = 0.0;
    (*PID).Err_Last = 0.0;
    (*PID).Err_LLast = 0.0;
    
    (*PID).KP = 0;
    (*PID).KI = 0;
    (*PID).KD = 0;
    printf("PID_Init End \n");
}

void PID2_Param_Input(struct _PID2 *PID,float KP,float KI,float KD)
{
    PID->KP = KP;
    PID->KI = KI;
    PID->KD = KD;
}

/**
 * @brief: PID运行
 * @param {_PID2} *PID 放入PID结构体
 * @param {float} Actual_Val 传感器获取的值
 * @param {float} Target_Val 目标调节的值   
 * @retval: PID的控制量  为了达到目标而控制的值   
 */
float PID2_Run(struct _PID2 *PID,float Actual_Val,float Target_Val)
{
    /* 目标值与实际值产生的误差 */
    (*PID).Target_Val = Target_Val;
    (*PID).Err = (*PID).Target_Val - Actual_Val;
    /* PID算法的实现 */
    float Increment_Val = (*PID).KP * ((*PID).Err - (*PID).Err_Last)  + (*PID).KI * (*PID).Err + (*PID).KD * ((*PID).Err - 2 * (*PID).Err_Last + (*PID).Err_LLast);
	//增量限幅
	if(Increment_Val >100)
		Increment_Val = 100;
	if(Increment_Val <-100)
		Increment_Val = -100;
    /* 累加 */
    PID->Control_Val += Increment_Val; /* 发现->比（*PID）好用，少括号 */
    /* 传递误差  */
    PID->Err_LLast = PID->Err_Last;
    PID->Err_Last = PID->Err;
    /* 返回经过PID运算的值 */
    return PID->Control_Val;
}   

void PID1_Input_Data(struct _PID1 *PID,float Actual_Data,float Target_Data)
{
	PID->Actual_Val = Actual_Data;
	PID->Target_Val = Target_Data;
}

void PID2_Input_Data(struct _PID2 *PID,float Actual_Data,float Target_Data)
{
	PID->Actual_Val = Actual_Data;
	PID->Target_Val = Target_Data;
}
