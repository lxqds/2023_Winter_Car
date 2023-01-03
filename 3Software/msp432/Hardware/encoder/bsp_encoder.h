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

#define motor_One_A_Value        BITBAND_PERI(P4IN, 1)
#define motor_One_B_Value        BITBAND_PERI(P4IN, 0)

#define motor_Two_A_Value        BITBAND_PERI(P4IN, 2)
#define motor_Two_B_Value        BITBAND_PERI(P4IN, 3)

#define motor_Three_A_Value        BITBAND_PERI(P4IN, 4)
#define motor_Three_B_Value        BITBAND_PERI(P4IN, 5)

#define motor_Four_A_Value        BITBAND_PERI(P4IN, 6)
#define motor_Four_B_Value        BITBAND_PERI(P4IN, 7)

#define motor_PORT_IRQHandler    PORT4_IRQHandler

/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
extern int8_t bianmaqi[4];

void Motor_Init_bianmaqi(void);



#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/	

