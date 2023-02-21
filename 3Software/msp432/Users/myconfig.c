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

Flag_Init Flag;

float PWMtemp1,PWMtemp2;
float g_fTargetJourney = 50;

uint8_t TempData;
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
	
	if(Flag.Start_Line_Flag == 1)
	{
		//判断距离是否达到实际的距离
		if(Encoder.Distance[2] >= (Flag.Target_Distance_Left ) ||Encoder.Distance[3] >= (Flag.Target_Distance_Right ) )
		{
			Flag.Stop_Count++;
			if(Flag.Stop_Count>100)
			{
				Flag.Stop_Flag = 1;//置标志位
				Flag.Start_Line_Flag = 0;
				Flag.Stop_Count = 0;
				LED_G_On();
				
				Flag.Is_EnMOTOR = 0;//电机失能
			}
		}
		else
		{
			Flag.Stop_Flag = 0;
			Flag.Stop_Count = 0;
			LED_G_Off();
		}
		
		if(Flag.Is_EnMOTOR == 1)
		{
			{
				CTRL_compute_Position();
				CTRL_compute_Speed();
				
				//巡线补偿
				if(Flag.Bias_Left == 1)
				{
					PWMtemp1 = speed_pid.output + 5;
					PWMtemp2 = speed_pid2.output -5;
				}else if(Flag.Bias_Right == 1)
				{
					PWMtemp1 = speed_pid.output - 5;
					PWMtemp2 = speed_pid2.output + 5;
				}
				else
				{
					PWMtemp1 = speed_pid.output;
					PWMtemp2 = speed_pid2.output;
				}
				Set_PWM(PWMtemp1 ,PWMtemp2);
			}
		}
	}
	
	if(Flag.Spin_Start_Flag == 1)
	{
//		if((Encoder.Distance[2] >= (Flag.Target_Distance_Left-0.1 )&&(Encoder.Distance[2] <= (Flag.Target_Distance_Left+0.1 )))||(Encoder.Distance[3] >= (Flag.Target_Distance_Right-0.1 )&&Encoder.Distance[3] >= (Flag.Target_Distance_Right+0.1 )))
//		{
//			Flag.Target_Distance_Arrive = 1;
//		}
		//判断距离是否达到实际的距离
//		if(Flag.Target_Distance_Arrive ==1)
		if((Encoder.Distance[2] >= (Flag.Target_Distance_Left-0.1 )&&(Encoder.Distance[2] <= (Flag.Target_Distance_Left+0.1 )))||(Encoder.Distance[3] >= (Flag.Target_Distance_Right-0.1 )&&Encoder.Distance[3] >= (Flag.Target_Distance_Right+0.1 )))
		{
			Flag.Stop_Count++;
			if(Flag.Stop_Count>100)
			{
				Flag.Stop_Flag = 1;//置标志位
				Flag.Spin_Start_Flag = 0;//开始转弯				Flag.Stop_Count = 0;//停止计时
				LED_G_On();//点灯
				
				Flag.Is_EnMOTOR = 0;//电机失能
			}	
		}
		else
		{
			Flag.Stop_Flag = 0;
			Flag.Stop_Count = 0;
			LED_G_Off();
		}
		
		if(Flag.Is_EnMOTOR == 1)
		{
			{
				CTRL_compute_Position();
				CTRL_compute_Speed();
				
				PWMtemp1 = speed_pid.output;
				PWMtemp2 = speed_pid2.output;
				Set_PWM(PWMtemp1 ,PWMtemp2);
			}
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

void EUSCIA3_IRQHandler(void)
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A3_BASE);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) //接收中断
    {
			TempData = MAP_UART_receiveData(EUSCI_A3_BASE);
			Get_Data_To_Array(TempData);
    }
}
/*****************************************************END OF FILE*********************************************************/	
