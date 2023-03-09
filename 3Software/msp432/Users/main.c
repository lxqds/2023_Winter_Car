#include "myconfig.h"


void Menudisplay(void);
void PID_Data_Send(void);
void Back_Routine(uint8_t Routine);
uint16_t s;
int main(void)
{
	{//初始化代码
		SysInit1();
		delay_init();
		uart_init0(115200);
		uart_init(115200);
		uart_init2(115200);
		KEY_Init();
		LED_Init();
		OLED_Init();
		Reflectance_Init2();
		TimA0_Int_Init(9999,48);//48分频，分频后1M，计数10000，10ms
		Servo_Init();
		Motor_Init();
		Motor_Init_bianmaqi();
		MPU_Init();
		
		protocol_init();
		PID_param_init(&move_pid);
		PID_param_init(&move_pid2);
		PID_param_init(&move_pid3);
		PID_param_init(&move_pid4);
		
		PID_param_init(&speed_pid);
		PID_param_init(&speed_pid2);
		PID_param_init(&speed_pid3);
		PID_param_init(&speed_pid4);
		
		PID_param_init(&Dir_pid);
		PID_param_init(&Dir_pid2);
		Servo_Control2(2,65);
	}
	
	{//配置PID
		set_p_i_d(&move_pid,50,0,50);
		set_p_i_d(&move_pid2,50,0,50);
		set_p_i_d(&move_pid3,50,0,50);
		set_p_i_d(&move_pid4,50,0,50);
		set_p_i_d(&speed_pid,0.45,0.03,0);
		set_p_i_d(&speed_pid2,0.45,0.03,0);
		set_p_i_d(&speed_pid3,0.45,0.03,0);
		set_p_i_d(&speed_pid4,0.45,0.03,0);
		
		set_p_i_d(&Dir_pid,10,0,2);
		set_p_i_d(&Dir_pid2,10,0,2);
		
		set_pid_target(&move_pid2,50);
		set_pid_target(&move_pid,50);
		set_pid_target(&Dir_pid,-0.01);
		set_pid_target(&Dir_pid2,-0.01);
		set_computer_value(SEND_START_CMD,0x01,NULL,1);
	}
	
	{//判断大小端
		uint32_t data = 0x01020304;
		char *p = (char *)&data;
		printf("0x0%x\n",*p);//判断大小端，0x04为小端，0x01为大端
	}
	for(;;)
	{//主循环
//		{//PID发送
//			PID_Data_Send();
//		}
//		Reflectance_Data = Reflectance_Read2();
//		if(Reflectance_Data!= Last_Reflectance_Data)
//		{//判断空白出线情况
//			if(Reflectance_Data==0b00000000)
//			{//如果这次读取到的值为空白，增加计数值,将上一次的值进行赋值
//				Reflectance_Data = Last_Reflectance_Data;
//				Flag.White_Count++;
//				if(Flag.White_Count>100)
//				{//如果多次遇到，判定为出界
//					Flag.White_Count = 0;
//					Reflectance_Data = 0b00000000;
//					Flag.Stop_Flag = 1;//置标志位			
//					Flag.Stop_Count = 0;//停止计时
//					LED_R_On();//点红灯
//					Flag.Is_EnMOTOR = 0;//电机失能
//				}
//			}
//			else
//			{//如果不是空白,清空计数值，将新的循迹值赋值给上一次循迹值
//				Flag.White_Count = 0;
//				Last_Reflectance_Data = Reflectance_Data;
//			}
//		}
//		switch(Reflectance_Data)
//		{//读取循迹模块的值并判断所在位置，偏差，位置
//			case 0b00000000:
//			{
//				Flag.White_Flag = 1;
//				Flag.Bias =0;
//			}break;
//			case 0b00100000:
//			{
//				Flag.Bias =0;
//			}break;
//			case 0b01100000:
//			{//左偏1
//				Flag.Bias =-10;
//			}break;
//			case 0b01000000:
//			{//左偏2
//				Flag.Bias =-25;
//			}break;
//			case 0b10000000:
//			{//左偏3
//				Flag.Bias =-60;
//			}break;
//			case 0b00110000:
//			{//右偏1
//				Flag.Bias =10;
//			}break;
//			case 0b00010000:
//			{//右偏2
//				Flag.Bias =25;
//			}break;
//			case 0b00001000:
//			{//右偏3
//				Flag.Bias =60;
//			}break;
//			case 0b11111000:
//			{//遇到路口
//				Flag.CrossRoad_Flag = 1;
//				Flag.Bias =Flag.Last_Bias;
//			}
//			case 0b11011000:
//			{//遇到路口
//				Flag.CrossRoad_Flag = 1;
//				Flag.Bias =Flag.Last_Bias;
//			}
//			default:
//			{
//				//如果没有偏差以上一次为准
//				Flag.Bias =Flag.Last_Bias;
//			} break;
//		}
//		Flag.Last_Bias = Flag.Bias;//
//		Last_Reflectance_Data = Reflectance_Data;
//		if(SensorData1.D.Float_Data)
//		{
//				Flag.Stop_Flag = 1;//置标志位
//				Flag.Spin_Start_Flag = 0;//开始转弯				
//				Flag.Stop_Count = 0;//停止计时
//				LED_G_On();//点灯
//				
//				Flag.Is_EnMOTOR = 0;//电机失能
//		}
		
		Menudisplay();
//		PID_Data_Send();
		{//调试区
//			while(1)
//			{
//				delay_ms(5000);
//				Flag.Servo_Scan_Flag =1;
//				//Servo_Scan();
////				Servo_Scan2(2,45,70);
////				delay_ms(60);
////				Servo_Control2(1,90);
////				Servo_Control2(2,90);
////				Menudisplay();
////				PID_Data_Send();
////				receiving_process();
////				Motor_Control(1,1,40);
////				Motor_Control(2,1,40);
////				set_motor_enable();
////				Set_PWM2(-50,-50);
////				//Servo_Control2(1,50);//向左调整舵机
////				Servo_Control2(2,120);//向右调整舵机//110右1//120//右二
//			}
		}
		
		if(Flag.Task == 0)
		{//第一步识别数字
			if(SensorData1.D.Float_Data&&Flag.Recognize_Num_Flag ==0)
			{//识别数字
				static uint8_t Last_Num,Temp_Num;//创建临时变量存储判断数字
				Temp_Num = SensorData1.D.Float_Data;//暂存变量
				if(Temp_Num == Last_Num)			//
				{
					Flag.Recognize_Num_Count++;
					if(Flag.Recognize_Num_Count>=10)
					{
						Flag.Recognize_Num_Flag = 1;//识别到数字后置标志位为1,待处理数字
						Flag.Target_Num = Temp_Num;//设置目标病房
						Flag.GO_Flag = 1;
						Flag.Target_Num_Flag = 1;
						Flag.Task = 1;
					}
				}
				Last_Num = Temp_Num;
			}
		}
		else if(Flag.Task ==1)
		{//第二步送药
			if(Flag.Load_drug == 1&&Flag.Target_Num_Flag == 1&&(Flag.Target_Num ==1||Flag.Target_Num ==2))//装有药以及识别到数字后开始行动
			{//装药，有目标数字，目标是1和2
				if(Flag.Target_Num ==1)
				{//前往1病房
					switch(Flag.Step_Count)
					{
						case 0:
						{//直走80cm
							Flag.Step_Count++;
							Car_Go(80);
						}break;
						case 1:
						{//当检测到路口或是车停止就再直走10cm
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(10);
							}
							if(Flag.Stop_Flag ==1)
							{//如果车子到达距离停止或者遇到十字路口
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_G_On();
								
								Flag.Is_EnMOTOR = 0;//电机失能
								Flag.Step_Count++;
								Car_Spin(0);
								LED_B_On();
							}
						}break;
						case 2:
						{//转完之后直走
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Go(60);
							}
						}break;
						case 3:
						{//检测到停止标志再走5cm
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(5);
							}
							if(Flag.Stop_Flag ==1)//到达位置点亮led
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_G_On();
								Flag.Step_Count++;
								LED_B_Off();
								LED_G_On();
							}
						}break;
						case 4:
						{//检测药被取走
							delay_ms(1000);
//							if(Keys[0].Double_Flag ==1)//如果药被取走
							{
								Keys[0].Double_Flag =0;
								LED_G_Off();
								Flag.Step_Count++;
								Car_Spin(3);//自向右转180度
							}
						}break;
						case 5:
						{//检测到转完180度后停下
							if(Flag.Stop_Flag ==1)//向右转180度后车子停下
							{
								Flag.Step_Count++;
								Car_Go(60);
							}
						}break;
						case 6:
						{//
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								
								Car_Go(10);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_G_On();
								Flag.Step_Count++;
								Car_Spin(1);
								LED_G_Off();
							}
						}break;
						case 7:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Go(70);
								LED_G_On();
							}
						}break;	
						case 8:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(5);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								LED_G_On();
							}
						}break;
					}
				}
				else if(Flag.Target_Num ==2)
				{//前往2病房
					switch(Flag.Step_Count)
					{
						case 0:
						{
							Flag.Step_Count++;
							Car_Go(80);
						}break;
						case 1:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(10);
							}
							if(Flag.Stop_Flag ==1)
							{//如果车子到达距离停止或者遇到十字路口
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_G_On();
								
								Flag.Is_EnMOTOR = 0;//电机失能
								Flag.Step_Count++;
								Car_Spin(1);
								LED_B_On();
							}
						}break;
						case 2:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Go(60);
							}
						}break;
						case 3:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(5);
							}
							if(Flag.Stop_Flag ==1)//到达位置点亮led
							{
								Flag.Step_Count++;
								LED_B_Off();
								LED_G_On();
							}
						}break;
						case 4:
						{
							delay_ms(1000);
//							if(Keys[0].Double_Flag ==1)//如果药被取走
							{
								Keys[0].Double_Flag =0;
								LED_G_Off();
								Flag.Step_Count++;
								Car_Spin(2);//自左转180度
							}
						}break;
						case 5:
						{
							if(Flag.Stop_Flag ==1)//自左转180度后车子停下
							{
								Flag.Step_Count++;
								Car_Go(60);
							}
						}break;
						case 6:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(10);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_G_On();
								Flag.Step_Count++;
								Car_Spin(0);
								LED_G_Off();
							}
						}break;
						case 7:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Go(70);
								LED_G_On();
							}
						}break;	
						case 8:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(5);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								LED_G_On();
							}
						}break;
					}
				}
			}
			else if(Flag.Load_drug == 1&&Flag.Target_Num_Flag == 1&&Flag.GO_Flag == 1)
			{//装药，识别数字，去送药
				switch(Flag.Step_Count)
				{
					case 0:
					{//直走120
						Flag.Step_Count++;
						Car_Go(120);
						Servo_Control2(2,70);
					}break;
					case 1:
					{//识别到数字停下，扫描数字
						if(SensorData1.D.Float_Data)
						{	
							Car_Go(1);
							Flag.Step_Count++;
						}
						Flag.CrossRoad_Flag = 0;
					}break;
					case 2:
					{
						if(Flag.Stop_Flag ==1)
						{//车子停下后开启舵机扫描
							Flag.Servo_Scan_Flag = 1;
							Flag.Step_Count++;
						}
					}break;
					case 3:
					{
						if(Flag.Servo_Scan_Flag ==0)
							{//舵机扫描完成后标志位置0
								Flag.Step_Count++;//进入下一个状态
								Car_Go(50);
							}
					}break;
					case 4:
					{//直走50cm
						if(Flag.CrossRoad_Flag == 1)
						{//遇到十字路口暂停
							Flag.CrossRoad_Flag = 0;
							Car_Go(10);
						}
						if(Flag.Stop_Flag ==1)	
						{
							if(Flag.Num_Recognize[0] == Flag.Target_Num)
							{//左转
								Car_Spin(0);
								Flag.Step_Count=10;//进入下一个状态
							}
							else if(Flag.Num_Recognize[1] == Flag.Target_Num)
							{//右转
								Car_Spin(1);
								Flag.Step_Count=20;//进入下一个状态
							}
							else 
							{//直走
								Servo_Control2(2,70);
								Car_Go(100);
								Flag.Step_Count=30;//进入下一个状态
							}
						}
					}break;
					{//3号走法
					case 10:
					{//车子转完弯停下来后
						if(Flag.Stop_Flag ==1)
						{
							Car_Go(60);
							Flag.Step_Count++;
						}
					}break;
					case 11:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(5);
						}
						if(Flag.Stop_Flag ==1)	
						{
							Flag.Step_Count++;
							LED_B_Off();
							LED_G_On();
						}
					}break;
					case 12:
					{
						delay_ms(1000);
//						if(Keys[0].Double_Flag ==1)//如果药被取走
						{
							Keys[0].Double_Flag =0;
							LED_G_Off();
							Flag.Step_Count++;
							Car_Spin(3);//自右转180度
						}
					}break;
					case 13:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Car_Go(60);
						}
					}break;
					case 14:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag =0;
							Car_Go(10);
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Car_Spin(1);
						}
					}break;
					case 15:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Car_Go(160);
						}
					}break;
					case 16:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag =0;
							Car_Go(80);
							Flag.Step_Count++;
						}
					}break;
					case 17:
					{
						if(Flag.Stop_Flag ==1)
						{
							LED_B_Off();
							LED_G_On();
						}
					}break;
					}
					
					{//4号走法
					case 20:
					{//车子转完弯停下来后
						if(Flag.Stop_Flag ==1)
						{
							Car_Go(60);
							Flag.Step_Count++;
						}
					}break;
					case 21:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(5);
						}
						if(Flag.Stop_Flag ==1)	
						{
							Flag.Step_Count++;
							LED_B_Off();
							LED_G_On();
						}
					}break;
					case 22:
					{
						delay_ms(1000);
//						if(Keys[0].Double_Flag ==1)//如果药被取走
						{
							Keys[0].Double_Flag =0;
							LED_G_Off();
							Flag.Step_Count++;
							Car_Spin(2);//自左转180度
						}
					}break;
					case 23:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Car_Go(60);
						}
					}break;
					case 24:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag =0;
							Car_Go(10);
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Car_Spin(0);
						}
					}break;
					case 25:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Car_Go(160);
						}
					}break;
					case 26:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag =0;
							Car_Go(80);
							Flag.Step_Count++;
						}
					}break;
					case 27:
					{
						if(Flag.Stop_Flag ==1)
						{
							LED_B_Off();
							LED_G_On();
						}
					}break;
					}
					
					{//远端走法
					case 30:
					{//识别到数字停下，扫描数字
						if(SensorData1.D.Float_Data)
						{	
							Car_Go(1);
							Flag.Step_Count++;
						}
						Flag.CrossRoad_Flag = 0;
					}break;
					case 31:
					{
						if(Flag.Stop_Flag ==1)
						{//车子停下后开启舵机扫描
							Flag.Servo_Scan_Flag2 = 1;
							Flag.Step_Count++;
						}
					}break;
					case 32:
					{
						if(Flag.Servo_Scan_Flag2 ==0)
						{//舵机扫描完成后标志位置0
							Flag.Step_Count++;//进入下一个状态
							Car_Go(50);
						}
					}break;
					case 33:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(10);
						}
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.Target_Num ==Flag.Num_Recognize2[0]||Flag.Target_Num ==Flag.Num_Recognize2[1])
							{
								Car_Spin(0);
								Flag.Step_Count = 40;
							}
							else if(Flag.Target_Num ==Flag.Num_Recognize2[2]||Flag.Target_Num ==Flag.Num_Recognize2[3])
							{
								Car_Spin(1);
								Flag.Step_Count = 80;
							}
						}
					}break;
					case 40:
					{//远端左
						 if(Flag.Stop_Flag ==1)
						 {
							Car_Go(60);
							Flag.Step_Count++;
						 }
					}break;
					case 41:
					{
						if(SensorData1.D.Float_Data)
						{	
							Car_Go(1);
							Flag.Step_Count++;
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
						}
						Flag.CrossRoad_Flag = 0;
					}break;
					case 42:
					{	
						if(Flag.Stop_Flag ==1)
						{
							Flag.Servo_Scan_Flag = 1;
							Flag.Step_Count++;
						}
					}break;
					case 43:
					{	
						if(Flag.Servo_Scan_Flag == 0)
						{
							Car_Go(50);
							Flag.Step_Count++;
						}
					}break;
					case 44:
					{	
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(10);
						}
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.Target_Num == Flag.Num_Recognize[0])
							{
								Car_Spin(0);
								Flag.Step_Count=45;
							}
							else
							{
								Car_Spin(1);
								Flag.Step_Count = 60;
							}
						}
					}break;
					{//5号路线走法
					case 45:
					{
						if(Flag.Stop_Flag ==1)
						{
							Car_Go(60);
							Flag.Step_Count++;
						}
					}break;
					case 46:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(5);
						}
						if(Flag.Stop_Flag ==1)
						{
							LED_G_On();
							Car_Spin(0);
							Flag.Step_Count++;
						}
					}break;
					case 47:
					{
						delay_ms(1000);
//						if(Keys[0].Double_Flag ==1)//如果药被取走
						{
							Keys[0].Double_Flag =0;
							LED_G_Off();
							Flag.Step_Count++;
							Car_Spin(3);//向右转180度
						}
					}break;
					case 48:
					{
						if(Flag.Stop_Flag ==1)
						{
							Car_Go(60);
							Flag.Step_Count++;
						}
					}break;
					case 49:
					{
						if(Reflectance_Data ==0b00111000)
						{
							Car_Go(10);
							Flag.Step_Count++;
						}
					}break;
					case 50:
					{
						if(Flag.Stop_Flag ==1)
						{
							Car_Spin(1);
							Flag.Step_Count++;
						}
					}break;
					case 51:
					{
						if(Flag.Stop_Flag ==1)
						{
							Car_Go(60);
							Flag.Step_Count++;
						}
					}break;
					case 52:
					{
						if(Reflectance_Data ==0b00111000)
						{
							Car_Go(10);
							Flag.Step_Count++;
						}
					}break;
					case 53:
					{
						if(Flag.Stop_Flag ==1)
						{
							Car_Spin(1);//右转
							Flag.Step_Count++;
						}
					}break;
					case 54:
					{
						if(Flag.Stop_Flag ==1)
						{
							Car_Go(120);//直走120
							Flag.Step_Count++;
						}
					}break;
					case 55:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(120);//直走120
							Flag.Step_Count++;
						}
					}break;
					case 56:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(120);//直走120
							Flag.Step_Count++;
						}
					}break;
					case 57:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(5);//直走120
							Flag.Step_Count++;
						}
					}break;
					case 58:
					{
						if(Flag.Stop_Flag == 1)
						{
							LED_G_On();
						}
					}break;
					}
					}
				}
			}
			else if(Flag.Load_drug == 0&&Flag.Back_Flag == 1)
			{//药被取走，返回标志为1
				if(Flag.Current_Position == 3)
				{
					Back_Routine(3);
				}
			}
		}
	}
	//return 0;
}
//输入返回路径
void Back_Routine(uint8_t Routine)
{
	static uint8_t Do_Count;
	uint8_t Init_Flag=0;
	if(Init_Flag == 0)
	{//初始化Do_Count
		Init_Flag = 1;
		Do_Count = 0;
	}
	switch(Routine)
	{
		case 3:
		{
			switch(Do_Count)
			{
				case  0:
				{
					Car_Spin(2);
					Do_Count++;
				}break;
				case  1:
				{
					if(Flag.Stop_Flag ==1)
					{
						Car_Go(50);
						Do_Count++;
					}
				}break;
				case  2:
				{
					if(Flag.Stop_Flag ==1)
					{
						Car_Spin(1);
						Do_Count++;
					}
				}break;
				case  3:
				{
					if(Flag.Stop_Flag ==1)
					{
						Car_Go(70);
						Do_Count++;
					}
				}break;
				case  4:
				{
					if(Flag.Stop_Flag ==1)
					{
						LED_G_On();
						Flag.Current_Position = 0;
					}
				}break;
			}
		}break;
		case 4:
		{
			switch(Do_Count)
			{
				case  0:
				{
					Car_Spin(2);
					Do_Count++;
				}break;
				case  1:
				{
					if(Flag.Stop_Flag ==1)
					{
						Car_Go(50);
						Do_Count++;
					}
				}break;
				case  2:
				{
					if(Flag.Stop_Flag ==1)
					{
						Car_Spin(0);
						Do_Count++;
					}
				}break;
				case  3:
				{
					if(Flag.Stop_Flag ==1)
					{
						Car_Go(70);
						Do_Count++;
					}
				}break;
				case  4:
				{
					if(Flag.Stop_Flag ==1)
					{
						LED_G_On();
						Flag.Current_Position = 0;
					}
				}break;
			}
		}break;
		case 5:
		{
			
		}break;
		case 6:
		{
			
		}break;
		case 7:
		{
			
		}break;
		case 8:
		{
			
		}break;
	}
}
void PID_Data_Send(void)
{
		int temp1 = Encoder.Speed[2];    // 上位机需要整数参数，转换一下
		int temp2 = Encoder.Distance[2];    // 上位机需要整数参数，转换一下
		int temp3 = Flag.Bias;
//		int temp3 = Encoder.Speed[3];    // 上位机需要整数参数，转换一下
//		int temp4 = Encoder.Distance[3];    // 上位机需要整数参数，转换一下
		receiving_process();
		set_computer_value(SEND_FACT_CMD, 0x02,&temp1, 1);
		set_computer_value(SEND_FACT_CMD, 0x01,&temp2, 1);
		set_computer_value(SEND_FACT_CMD, 0x03,&temp3, 1);
//		set_computer_value(SEND_FACT_CMD, 0x04,&temp3, 1);
//		set_computer_value(SEND_FACT_CMD, 0x03,&temp4, 1);
}
void Menudisplay(void)
{
	static uint8_t State=Main_State,New_State=Main_State;
	if(State!=New_State)
		{
			OLED_Clear();
			State = New_State;
		}
		switch(State)
		{
			case Main_State:
			{
				OLED_ShowString(0,0,"Main_Menu",16);
				OLED_ShowString(0,2,"1.Function1",16);
				OLED_ShowString(0,4,"2.Function2",16);
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					New_State = State1;
				}
				else if(Keys[0].Double_Flag == 1)
				{
					Keys[0].Double_Flag = 0;
					New_State = State2;
				}
				else if(Keys[0].Long_Press_Flag == 1)
				{
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
			case State1:
			{
				set_motor_enable();
				OLED_ShowString(0,0,"Function1",16);
				OLED_ShowBin(1,2,Reflectance_Data,8,16);
				
				{//前轮编码器
//					OLED_ShowBNum(0,4,Encoder.Speed[0],3,16);
//					OLED_ShowBNum(0,6,Encoder.Speed[1],3,16);
//					OLED_ShowBNum(48,4,Encoder.Distance[0],3,16);
//					OLED_ShowBNum(48,6,Encoder.Distance[1],3,16);
				}
				{//后轮编码器
					OLED_ShowBNum(0,4,Encoder.Speed[2],3,16);
					OLED_ShowBNum(0,6,Encoder.Speed[3],3,16);
					OLED_ShowBNum(48,4,Encoder.Distance[2],3,16);
					OLED_ShowBNum(48,6,Encoder.Distance[3],3,16);
				}
				
				OLED_ShowNum(96,2,SensorData1.X,3,16);
				OLED_ShowNum(96,4,SensorData1.Y,3,16);
				OLED_ShowNum(96,6,SensorData1.D.Float_Data,3,16);
				
				
				if(Keys[1].Single_Flag == 1)
				{
					Keys[1].Single_Flag = 0;
					Car_Go(80);  
				}
				if(Keys[1].Double_Flag == 1)
				{
					Keys[1].Double_Flag = 0;
					Car_Spin(0);
				}
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					New_State = State1;
					Keys[0].Single_Flag =0;
					Flag.Load_drug = 1;
//					Flag.Target_Num = 1;
//					Flag.Step_Count=0;
					LED_G_On();
				}
//				else if(Keys[0].Double_Flag == 1)
//				{
//					Keys[0].Double_Flag = 0;
//					New_State = State2;
//				}
				else if(Keys[0].Long_Press_Flag == 1)
				{
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
			case State2:
			{
				set_motor_disable();
				short x,y,z;
				float p,r,yow;
				MPU_Get_Gyroscope(&x,&y,&z);
				mpu_dmp_get_data(&p,&r,&yow);
				OLED_ShowString(64,0,"Function2",16);
				OLED_ShowBNum(64,2,p,6,16);
				OLED_ShowBNum(64,4,r,6,16);
				OLED_ShowBNum(64,6,yow,6,16);
				OLED_ShowBNum(0,2,x,6,16);
				OLED_ShowBNum(0,4,y,6,16);
				OLED_ShowBNum(0,6,z,6,16);
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					New_State = State1;
				}else if(Keys[0].Double_Flag == 1)
				{
					Keys[0].Double_Flag = 0;
					New_State = State2;
				}else if(Keys[0].Long_Press_Flag == 1)
				{
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
		}
}
