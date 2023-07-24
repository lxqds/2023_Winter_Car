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
#include "math.h"
/* define -----------------------------------------------------------------------------------------------------------------*/
#define ABS(x) ((x)>=0?(x):-(x)))

uint16_t Delay10msCnt = 0;
uint8_t TIMA_Count = 0;
uint8_t Car_Star_Flag = 1;
uint8_t Reflectance_Data;
uint8_t Last_Reflectance_Data;
uint8_t Recive_Byte;

Flag_Init Flag;

float PWMtemp1,PWMtemp2,PWMtemp3,PWMtemp4;
float g_fTargetJourney = 50;
float Average_Distance;


uint8_t TempData;
uint8_t Temp_Angle;
float Temp_Num;
float Sensor_Num_Temp;
float Last_Num;
uint8_t Num_Count=0;//识别到的数字计数
uint8_t Num_Count2=0;//识别到的数字计数
uint8_t Num_Count3=0;//识别到的数字计数
uint8_t Num_Same_Flag;//数字一样标志位
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
	Reflectance_Data = Reflectance_Read2();
	
	switch(Reflectance_Data)
		{//读取循迹模块的值并判断所在位置，偏差，位置
			case 0b00000000:
			{
				Flag.White_Flag = 1;
				Flag.Bias =0;
			}break;
			case 0b00100000:
			{
				Flag.Bias =0;
			}break;
			case 0b01100000:
			{//左偏1
				Flag.Bias =-10;
			}break;
			case 0b01000000:
			{//左偏2
				Flag.Bias =-25;
			}break;
			case 0b10000000:
			{//左偏3
				Flag.Bias =-60;
			}break;
			case 0b00110000:
			{//右偏1
				Flag.Bias =10;
			}break;
			case 0b00010000:
			{//右偏2
				Flag.Bias =25;
			}break;
			case 0b00001000:
			{//右偏3
				Flag.Bias =60;
			}break;
			case 0b11111000:
			{//遇到路口
				Flag.CrossRoad_Flag = 1;
				Flag.Bias =Flag.Last_Bias;
			}
			case 0b11011000:
			{//遇到路口
				Flag.CrossRoad_Flag = 1;
				Flag.Bias =Flag.Last_Bias;
			}
			case 0b01010000:
			{//遇到路口
				Flag.CrossRoad_Flag = 1;
				Flag.Bias =Flag.Last_Bias;
			}
			default:
			{
				//如果没有偏差以上一次为准
				Flag.Bias =Flag.Last_Bias;
			} break;
		}
		Flag.Last_Bias = Flag.Bias;//
		Last_Reflectance_Data = Reflectance_Data;
		
	if(Flag.Servo_Scan_Flag ==1)
	{//路口1检测到舵机开始扫描
		Delay10msCnt++;
		if(Delay10msCnt==5)
		{//50ms进一次
			Temp_Num = 0;//初始化
			Delay10msCnt=0;
			Temp_Angle = Servo_Scan2(2,45,135);
			Sensor_Num_Temp = SensorData1.D.Float_Data;
			if(Sensor_Num_Temp&&Temp_Angle>45)
			{//识别数字
				static uint8_t Last_Num2,Temp_Num2;//创建临时变量存储判断数字
				Temp_Num2 = Sensor_Num_Temp;//暂存变量
				if(Temp_Num2 == Last_Num2)			//
				{
					Flag.Recognize_Num_Count++;
					if(Flag.Recognize_Num_Count>=10)
					{
						Temp_Num = Temp_Num2;//设置目标病房
					}
				}
				Last_Num2 = Temp_Num2;
			}
			if(Temp_Num)
			{//识别到有数字后存储到数组中
				if(Temp_Num !=Flag.Num_Recognize[0]&&
						Temp_Num !=Flag.Num_Recognize[1]&&
							Temp_Num !=Flag.Num_Recognize[2]&&
								Temp_Num !=Flag.Num_Recognize[3]&&
									Temp_Num !=Flag.Num_Recognize[4]&&
										Temp_Num !=Flag.Num_Recognize[5]&&
											Temp_Num !=Flag.Num_Recognize[6]&&
												Temp_Num !=Flag.Num_Recognize[7])
				{//如果识别到的数字和上一次不相等
					Flag.Num_Angle[Num_Count] = Temp_Angle;
					Flag.Num_Recognize[Num_Count] = Temp_Num;
					Num_Count++;
					Last_Num = Temp_Num;
				}
			}
			if(Temp_Angle ==255)
			{//如果扫描完了
				Last_Num = 0;
				Flag.Servo_Scan_Flag =0;
				Num_Count = 0;
			}
			
		}
		}
	if(Flag.Servo_Scan_Flag2 ==1)
	{//路口2检测到舵机开始扫描
		Delay10msCnt++;
		if(Delay10msCnt==5)
		{//50ms进一次
			Temp_Num = 0;
			Delay10msCnt=0;
			Temp_Angle = Servo_Scan2(2,30,150);
			Sensor_Num_Temp = SensorData1.D.Float_Data;
			if(Sensor_Num_Temp&&Temp_Angle>35)
			{//识别数字
				static uint8_t Last_Num3,Temp_Num3;//创建临时变量存储判断数字
				Temp_Num3 = Sensor_Num_Temp;//暂存变量
				if(Temp_Num3 == Last_Num3)			//
				{
					Flag.Recognize_Num_Count++;
					if(Flag.Recognize_Num_Count>=10)
					{
						Temp_Num = Temp_Num3;//记录数字
					}
				}
				else
				{
					Flag.Recognize_Num_Count=0;
				}
				Last_Num3 = Temp_Num3;
			}
			if(Temp_Num)
			{//识别到有数字后存储到数组中
				if(Temp_Num !=Flag.Num_Recognize2[0]&&
						Temp_Num !=Flag.Num_Recognize2[1]&&
							Temp_Num !=Flag.Num_Recognize2[2]&&
								Temp_Num !=Flag.Num_Recognize2[3]&&
									Temp_Num !=Flag.Num_Recognize2[4]&&
										Temp_Num !=Flag.Num_Recognize2[5]&&
											Temp_Num !=Flag.Num_Recognize2[6]&&
												Temp_Num !=Flag.Num_Recognize2[7]&&
													Temp_Num !=Flag.Num_Recognize[0]&&
														Temp_Num !=Flag.Num_Recognize[1])
				{//如果识别到的数字和上一次不相等
					Flag.Num_Angle2[Num_Count2] = Temp_Angle;
					Flag.Num_Recognize2[Num_Count2] = Temp_Num;
					Num_Count2++;
					Last_Num = Temp_Num;
				}
			}
			if(Temp_Angle ==255)
			{//如果扫描完了
				Last_Num = 0;
				Flag.Servo_Scan_Flag2 =0;
				Num_Count2 = 0;
			}
		}
	}
	if(Flag.Servo_Scan_Flag3 ==1)
	{//路口3检测到舵机开始扫描
		Delay10msCnt++;
		if(Delay10msCnt==5)
		{//50ms进一次
			Temp_Num = 0;//初始化
			Delay10msCnt=0;
			Temp_Angle = Servo_Scan2(2,45,70);
			Sensor_Num_Temp = SensorData1.D.Float_Data;//先赋值，再判断
			if(Sensor_Num_Temp&&Temp_Angle>47)
			{//识别数字
				static uint8_t Last_Num4,Temp_Num4;//创建临时变量存储判断数字
				Temp_Num4 = Sensor_Num_Temp;//暂存变量
				if(Temp_Num4 == Last_Num4)			//
				{
					Flag.Recognize_Num_Count++;
					if(Flag.Recognize_Num_Count>=10)
					{
						Temp_Num = Temp_Num4;//设置目标病房
					}
				}
				else
				{
					Flag.Recognize_Num_Count = 0;
				}
				Last_Num4 = Temp_Num4;
			}
			if(Temp_Num)
			{//识别到有数字后存储到数组中
//				if(Temp_Num !=Flag.Num_Recognize3[0]&&
//						Temp_Num !=Flag.Num_Recognize3[1]&&
//							Temp_Num !=Flag.Num_Recognize3[2]&&
//								Temp_Num !=Flag.Num_Recognize3[3]&&
//									Temp_Num !=Flag.Num_Recognize3[4]&&
//										Temp_Num !=Flag.Num_Recognize3[5]&&
//											Temp_Num !=Flag.Num_Recognize3[6]&&
//												Temp_Num !=Flag.Num_Recognize3[7]&&
//													Temp_Num !=Flag.Num_Recognize[0]&&
//														Temp_Num !=Flag.Num_Recognize[1])
				{//如果识别到的数字和上一次不相等
					Flag.Num_Angle3[Num_Count] = Temp_Angle;
					Flag.Num_Recognize3[Num_Count] = Temp_Num;
					Num_Count3++;
					Last_Num = Temp_Num;
				}
			}
			if(Temp_Angle ==255)
			{//如果扫描完了
				Last_Num = 0;
				Flag.Servo_Scan_Flag3 =0;
				Num_Count3 = 0;
			}
			
		}
		}	
	if(Flag.Start_Line_Flag == 1)
	{//巡线
		//判断距离是否达到实际的距离
		if(Reflectance_Data ==0b00100000)
		{//归中取平均
			float Distance_Averge;
			Distance_Averge= (Encoder.Distance[2]+Encoder.Distance[3])/2;
			Encoder.Distance[2] = Distance_Averge;
			Encoder.Distance[3] = Distance_Averge;
		}
		if((Encoder.Speed[2]==0||Encoder.Speed[3]==0)&&((Encoder.Distance[2]>fabs(Flag.Target_Distance_Left-0.5f))&&(Encoder.Distance[3]>fabs(Flag.Target_Distance_Right-0.5f))))
		{
			Flag.Stop_Count++;
			if(Flag.Stop_Count>100)
			{
				Flag.Stop_Flag = 1;//置标志位
				Flag.Spin_Start_Flag = 0;//开始转弯				
				Flag.Stop_Count = 0;//停止计时
				LED_G_On();//点灯
				
				Flag.Is_EnMOTOR = 0;//电机失能
				PID_Clear(&speed_pid);
				PID_Clear(&speed_pid2);
				PID_Clear(&speed_pid3);
				PID_Clear(&speed_pid4);
				PID_Clear(&move_pid);
				PID_Clear(&move_pid2);
				PID_Clear(&move_pid3);
				PID_Clear(&move_pid4);
			}	
		}
		else
		{
			Flag.Stop_Flag = 0;
			LED_G_Off();
		}
//		if((Encoder.Distance[2] >= (Flag.Target_Distance_Left-0.5f )&&(Encoder.Distance[2] <= (Flag.Target_Distance_Left+0.5f )))||(Encoder.Distance[3] >= (Flag.Target_Distance_Right-0.5f )&&Encoder.Distance[3] >= (Flag.Target_Distance_Right+0.5f )))
//		{
//			Flag.Stop_Count++;
//			if(Flag.Stop_Count>100)
//			{
//				Flag.Stop_Flag = 1;//置标志位
//				Flag.Start_Line_Flag = 0;
//				Flag.Stop_Count = 0;
//				LED_G_On();
//				
//				Flag.Is_EnMOTOR = 0;//电机失能
//			}
//		}
//		else
//		{
//			Flag.Stop_Flag = 0;
//			Flag.Stop_Count = 0;
//			LED_G_Off();
//		}
		
		if(Flag.Is_EnMOTOR == 1)
		{
			{
				{
				{
				PWMtemp1  =0;
				PWMtemp2  =0;
				PWMtemp3  =0;
				PWMtemp4  =0;
				CTRL_compute_Position();
				CTRL_compute_Direction(Flag.Bias);
				CTRL_compute_Speed();
				//转向环
				}
				PWMtemp1 = speed_pid.output ;
                PWMtemp2 = speed_pid2.output;
                PWMtemp3 = speed_pid3.output;
				PWMtemp4 = speed_pid4.output;	
				}
				Set_PWM(PWMtemp1 ,PWMtemp2);
				Set_PWM2(PWMtemp1 ,PWMtemp2);
			}
		}
		else
		{
			Set_PWM(0,0);
			Set_PWM2(0,0);
		}
	}
	
	if(Flag.Spin_Start_Flag == 1)
	{//转弯
		
		//判断1.电机是否停转以及编码器的距离超过10cm或者2.编码器到达10cm以上同时巡线检测到中线
//		if(((Encoder.Speed[2]==0||Encoder.Speed[3]==0)&&((fabs(Encoder.Distance[2])>=13.f)&&(fabs(Encoder.Distance[3])>13.f)))||(((fabs(Encoder.Distance[2])>13.f)&&(fabs(Encoder.Distance[3])>13.f))&&Reflectance_Data==0b00100000))
//		if(((fabs(Encoder.Distance[2])>=14.f)&&(fabs(Encoder.Distance[3])>14.f))||((fabs(Encoder.Distance[2])>=10.0f)&&(fabs(Encoder.Distance[2])>=10.0f)&&Reflectance_Data==0b00100000))
		if((fabs(Encoder.Distance[2])>=10.f||fabs(Encoder.Distance[3])>=10.f)&&(Reflectance_Data==0b00100000||Reflectance_Data==0b01100000||Reflectance_Data==0b00110000))
		{
			Flag.Stop_Count++;
			if(Flag.Stop_Count>=1)
			{
				Flag.Stop_Flag = 1;//置标志位
				Flag.Spin_Start_Flag = 0;//停止转弯				
				Flag.Stop_Count = 0;//停止计时
				LED_G_On();//点灯
				
				Flag.Is_EnMOTOR = 0;//电机失能
				PID_Clear(&speed_pid);
				PID_Clear(&speed_pid2);
				PID_Clear(&speed_pid3);
				PID_Clear(&speed_pid4);
			}	
		}
		else
		{
			Flag.Stop_Flag = 0;
			LED_G_Off();
		}
//		if((Encoder.Distance[2] >= (Flag.Target_Distance_Left-0.5f )&&(Encoder.Distance[2] <= (Flag.Target_Distance_Left+0.5f )))||(Encoder.Distance[3] >= (Flag.Target_Distance_Right-0.5f )&&Encoder.Distance[3] >= (Flag.Target_Distance_Right+0.5f )))
//		{
//			Flag.Stop_Count++;
//			if(Flag.Stop_Count>100)
//			{
//				Flag.Stop_Flag = 1;//置标志位
//				Flag.Spin_Start_Flag = 0;//开始转弯				
//				Flag.Stop_Count = 0;//停止计时
//				LED_G_On();//点灯
//				
//				Flag.Is_EnMOTOR = 0;//电机失能
//			}	
//		}
//		else
//		{
//			Flag.Stop_Flag = 0;
//			Flag.Stop_Count = 0;
//			LED_G_Off();
//		}
		
		if(Flag.Is_EnMOTOR == 1)
		{
			{
				PWMtemp1  =0;
				PWMtemp2  =0;
				PWMtemp3  =0;
				PWMtemp4  =0;
				
//				CTRL_compute_Position();
//				if(((fabs(Encoder.Distance[2])>=10.f)&&(fabs(Encoder.Distance[3])>10.f)))
//				CTRL_compute_Direction(Flag.Bias);//开启巡线
//				CTRL_compute_Speed();
//				
//				PWMtemp1 = speed_pid.output;
//				PWMtemp2 = speed_pid2.output;
//				PWMtemp3 = speed_pid3.output;
//				PWMtemp4 = speed_pid4.output;
				if(Flag.Target_Distance_Left<0&&Flag.Target_Distance_Right>0)
				{//转的时候会设置目标，以此设置方向、
					set_pid_target(&speed_pid,-25);
					set_pid_target(&speed_pid2,25);
					set_pid_target(&speed_pid3,-25);
					set_pid_target(&speed_pid4,25);
					CTRL_compute_Speed();
					PWMtemp1 = speed_pid.output;
					PWMtemp2 = speed_pid2.output;
					PWMtemp3 = speed_pid3.output;
					PWMtemp4 = speed_pid4.output;
					Set_PWM(PWMtemp1 ,PWMtemp2);
					Set_PWM2(PWMtemp3 ,PWMtemp4);
				}
				else if(Flag.Target_Distance_Left>0&&Flag.Target_Distance_Right<0)
				{
					set_pid_target(&speed_pid,25);
					set_pid_target(&speed_pid2,-25);
					set_pid_target(&speed_pid3,25);
					set_pid_target(&speed_pid4,-25);
					CTRL_compute_Speed();
					PWMtemp1 = speed_pid.output;
					PWMtemp2 = speed_pid2.output;
					PWMtemp3 = speed_pid3.output;
					PWMtemp4 = speed_pid4.output;
					Set_PWM(PWMtemp1 ,PWMtemp2);
					Set_PWM2(PWMtemp3 ,PWMtemp4);
				}
				
			}
		}
		else
		{
			Set_PWM(0,0);
			Set_PWM2(0,0);
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
