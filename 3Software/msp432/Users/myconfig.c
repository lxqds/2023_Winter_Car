/**
  *************************************************************************************************************************
  * @file    myconfig.c
  * @author  lxq
  * @version V1.0
  * @date    2023-01-03
  * @brief   myconfig模块.c文件配置
  *************************************************************************************************************************
  * @attention
  *
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/

#include "myconfig.h"

/* define -----------------------------------------------------------------------------------------------------------------*/
uint8_t Delay10msCnt = 0;
uint8_t Key;
uint8_t Reflectance_Data;
int8_t Encoder[4];



/**
 * @name:TA0_0_IRQHandler
 * @brief:TA0中断回调函数
 * @param:无
 * @retuen:无
 */
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Delay10msCnt++;
	if(Delay10msCnt>50)
	{
		Delay10msCnt = 0;//延时500ms
		MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
	}
    /*开始填充用户代码*/
	Key = KEY_Scan(1);	
	Encoder[0] = bianmaqi[0];
	Encoder[1] = bianmaqi[1];
	Encoder[2] = bianmaqi[2];
	Encoder[3] = bianmaqi[3];
	
	bianmaqi[0] = 0;
	bianmaqi[1]	= 0;
	bianmaqi[2]	= 0;
	bianmaqi[3]	= 0;
    

    /*结束填充用户代码*/
}

/**
 * @name:T32_INT2_IRQHandler
 * @brief:T32中断回调函数
 * @param:无
 * @retuen:无
 */
void T32_INT2_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_1_BASE);

    /*开始填充用户代码*/

    /*结束填充用户代码*/
}
/*****************************************************END OF FILE*********************************************************/	
