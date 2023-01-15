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
uint16_t Delay10msCnt = 0;
uint8_t TIMA_Count = 0;
uint8_t Car_Star_Flag = 1;
uint8_t Reflectance_Data;
uint8_t Recive_Byte;

float PWMtemp1,PWMtemp2;




/**
 * @name:TA0_0_IRQHandler
 * @brief:TA0中断回调函数
 * @param:无
 * @retuen:无
 */
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    
    /*开始填充用户代码*/
	//Key = KEY_Scan(1);
	//Key = Key_Scan();
	Key_Scan2();
	Encoder_Scan();
	
//	if(Car_Star_Flag)
	{
		TIMA_Count ++;
//		if(TIMA_Count %2)		//控制周期为20ms
		{
			CTRL_compute_Position();
			CTRL_compute_Speed();
			Set_PWM(speed_pid.output,speed_pid2.output);
		}
		
		if(TIMA_Count ==50)		//控制周期为500ms
		{
			TIMA_Count = 0;
//			CTRL_compute_Position();
		}
	}
   
    /*结束填充用户代码*/
}

/**
 * @name:T32_INT2_IRQHandler
 * @brief:T32中断回调函数
 * @param:无
 * @retuen:无
 */
void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    /*开始填充用户代码*/

    /*结束填充用户代码*/
}

void EUSCIA0_IRQHandler(void)
{

    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
	uint8_t dr;	
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) //接收中断
    {
		dr = MAP_UART_receiveData(EUSCI_A0_BASE);
		protocol_data_recv(&dr,1);

    }
}
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
		
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) //接收中断
    {
			Recive_Byte = MAP_UART_receiveData(EUSCI_A2_BASE);
    }
}

/*****************************************************END OF FILE*********************************************************/	
