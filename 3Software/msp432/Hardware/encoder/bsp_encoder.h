/**
  *************************************************************************************************************************
  * @file    bsp_encoder.h
  * @author  lxq
  * @version V1.0
  * @date    2023-01-03
  * @brief   编码器模块.h文件配置
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */
  
/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* define定义 -----------------------------------------------------------------------------------------------------------------*/
#define motor_One_A_PORT        GPIO_PORT_P4
#define motor_One_A_PIN         GPIO_PIN0
#define motor_One_A_INT         INT_PORT4

#define motor_One_B_PORT        GPIO_PORT_P4
#define motor_One_B_PIN         GPIO_PIN1
#define motor_One_B_INT         INT_PORT4

#define motor_Two_A_PORT        GPIO_PORT_P4
#define motor_Two_A_PIN         GPIO_PIN2
#define motor_Two_A_INT         INT_PORT4

#define motor_Two_B_PORT        GPIO_PORT_P4
#define motor_Two_B_PIN         GPIO_PIN3
#define motor_Two_B_INT         INT_PORT4

#define motor_Three_A_PORT        GPIO_PORT_P4
#define motor_Three_A_PIN         GPIO_PIN4
#define motor_Three_A_INT         INT_PORT4

#define motor_Three_B_PORT        GPIO_PORT_P4
#define motor_Three_B_PIN         GPIO_PIN5
#define motor_Three_B_INT         INT_PORT4

#define motor_Four_A_PORT        GPIO_PORT_P4
#define motor_Four_A_PIN         GPIO_PIN6
#define motor_Four_A_INT         INT_PORT4

#define motor_Four_B_PORT        GPIO_PORT_P4
#define motor_Four_B_PIN         GPIO_PIN7
#define motor_Four_B_INT         INT_PORT4

#define motor_One_A_Value        BITBAND_PERI(P4IN, 0)//0，1相反了
#define motor_One_B_Value        BITBAND_PERI(P4IN, 1)

#define motor_Two_A_Value        BITBAND_PERI(P4IN, 2)
#define motor_Two_B_Value        BITBAND_PERI(P4IN, 3)

#define motor_Three_A_Value        BITBAND_PERI(P4IN, 4)
#define motor_Three_B_Value        BITBAND_PERI(P4IN, 5)

#define motor_Four_A_Value        BITBAND_PERI(P4IN, 6)
#define motor_Four_B_Value        BITBAND_PERI(P4IN, 7)

#define motor_PORT_IRQHandler    PORT4_IRQHandler



#define RESOLUTION 			13*20*2 							//13线30减速比2倍频的分辨率，轮子转一圈产生的脉冲数
#define OUTSIDE_DIAMETER 	2*3.14f*3.35f							//轮子的外径，用于距离测量，单位cm
#define PER_PULSE_CM		(2*3.14f*3.35f)/(13*20*2)			//一个脉冲走过的CM数

typedef struct
{
	int16_t Encoder_Once[4];	//一个定时10ms内的总计数，10ms刷新，中断累加
	int16_t Encoder[4];			//编码器显示的值，10ms取一次，不累加，仅读取
	int32_t Encoder_Total[4];	//读取完成之后，读取值加入，用于里程计数
	float Speed[4];				//记录速度
	float Distance[4];			//记录距离
}Encoder_Init;

/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
extern int8_t bianmaqi[4];
extern Encoder_Init Encoder;
void Motor_Init_bianmaqi(void);
void Encoder_Scan(void);


#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/	

