/**
  *************************************************************************************************************************
  * @file    k210.h
  * @author  lxq
  * @version V1.0
  * @date    2023-02-21
  * @brief   k210模块.h文件配置
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */


/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __K210_H
#define	__K210_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* define定义 -----------------------------------------------------------------------------------------------------------------*/
/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/

typedef union float_Data
{
	unsigned char byte[4];
	float Float_Data;
}Float_Byte;
typedef struct Sensor_Data
{
	int16_t X;
	int16_t Y;
	Float_Byte D;
}SensorData;



/* extern提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
void Get_Data_To_Array(uint8_t ByteData);


#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/	