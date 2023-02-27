#include "myconfig.h"


void Menudisplay(void);
void PID_Data_Send(void);
void Back_Routine(uint8_t Routine);
int main(void)
{
	{//初始化代码
		SysInit1();
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
		PID_param_init(&speed_pid);
		PID_param_init(&speed_pid2);
	}
	
	{//配置PID
		set_p_i_d(&move_pid,50,0,50);
		set_p_i_d(&move_pid2,50,0,50);
		set_p_i_d(&speed_pid,0.45,0.03,0);
		set_p_i_d(&speed_pid2,0.45,0.03,0);
		
		set_pid_target(&move_pid2,50);
		set_pid_target(&move_pid,50);
		
		set_computer_value(SEND_START_CMD,0x01,NULL,1);
	}

	
	{//判断大小端
		uint32_t data = 0x01020304;
		char *p = (char *)&data;
		printf("0x0%x\n",*p);//判断大小端，0x04为小端，0x01为大端
	}
	
	
	for(;;)
	{//主循环
		Reflectance_Data = Reflectance_Read2();
		switch(Reflectance_Data)
		{//读取循迹模块的值并判断所在位置，偏差，位置
			case 0b00000000:
			{
				Flag.Bias =0;
			}break;
			case 0b01100000:
			{//左偏1
				Flag.Bias =5;
			}break;
			case 0b01000000:
			{//左偏2
				Flag.Bias =5;
			}break;
			case 0b10000000:
			{//左偏3
				Flag.Bias =30;
			}break;
			case 0b00110000:
			{//右偏1
				Flag.Bias =-5;
			}break;
			case 0b00010000:
			{//右偏2
				Flag.Bias =-5;
			}break;
			case 0b00001000:
			{//右偏3
				Flag.Bias =-30;
			}break;
			case 0b11111000:
			{//遇到路口
				Flag.CrossRoad_Flag = 1;
			}
		}
		Menudisplay();
		PID_Data_Send();
//		{//调试区
//			while(1)
//			{
//				//Servo_Control2(1,50);//向左调整舵机
//				Servo_Control2(2,120);//向右调整舵机//110右1//120//右二
//			}
//		}
		
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
						{
							Flag.Step_Count++;
							Car_Go(50);
						}break;
						case 1:
						{
							if(Flag.Stop_Flag ==1||Flag.CrossRoad_Flag==1)
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
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Go(50);
							}
						}break;
						case 3:
						{
							if(Flag.Stop_Flag ==1)//到达位置点亮led
							{
								Flag.Step_Count++;
								LED_B_Off();
								LED_G_On();
							}
						}break;
						case 4:
						{
							if(Keys[0].Double_Flag ==1)//如果药被取走
							{
								Keys[0].Double_Flag =0;
								LED_G_Off();
								Flag.Step_Count++;
								Car_Spin(2);//自转180度
							}
						}break;
						case 5:
						{
							if(Flag.Stop_Flag ==1)//自转180度后车子停下
							{
								Flag.Step_Count++;
								Car_Go(50);
							}
						}break;
						case 6:
						{
							if(Flag.Stop_Flag ==1)
							{
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
								Car_Go(50);
								LED_G_Off();
							}
						}break;	
						case 8:
						{
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
							Car_Go(50);
						}break;
						case 1:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Spin(1);
								LED_G_Off();
								LED_B_On();
							}
						}break;
						case 2:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								Car_Go(50);
							}
						}break;
						case 3:
						{
							if(Flag.Stop_Flag ==1)//到达位置点亮led
							{
								Flag.Step_Count++;
								LED_B_Off();
								LED_G_On();
							}
						}break;
						case 4:
						{
							if(Keys[0].Double_Flag ==1)//如果药被取走
							{
								Keys[0].Double_Flag =0;
								LED_G_Off();
								Flag.Step_Count++;
								Car_Spin(2);//自转180度
							}
						}break;
						case 5:
						{
							if(Flag.Stop_Flag ==1)//自转180度后车子停下
							{
								Flag.Step_Count++;
								Car_Go(50);
							}
						}break;
						case 6:
						{
							if(Flag.Stop_Flag ==1)
							{
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
								Car_Go(50);
								LED_G_Off();
							}
						}break;	
						case 8:
						{
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
					{//直走70
						Flag.Step_Count++;
						Car_Go(70);
					}break;
					case 1:
					{//停下，向左调整
						if(Flag.Stop_Flag ==1)
						{
							Servo_Control2(2,75);//向左调整舵机
							Flag.Step_Count++;//进入下一个状态
						}
					}break;
					case 2:
					{//识别数字
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
									Flag.Recognize_Num = Temp_Num;//设置目标病房
								}
							}
							Last_Num = Temp_Num;
						}
						if(Flag.Recognize_Num_Flag ==1)
						{//识别数字后,清除标志位，记忆路线，与目标相同或不同   
							Flag.Recognize_Num_Flag =0;
							Flag.Routine[3] = Flag.Recognize_Num;//记忆3号路线的数字
							if(Flag.Recognize_Num ==Flag.Target_Num)
							{//与目标相同
								Flag.Step_Count =63;//三号路线走法
							}
							else 
							{//不同，调整舵机
								Servo_Control2(2,110);//向右调整舵机
								Flag.Step_Count ++;
							}
						}
					}break;
					case 3:
					{//看右边数字
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
									Flag.Recognize_Num = Temp_Num;//设置目标病房
								}
							}
							Last_Num = Temp_Num;
						}
						if(Flag.Recognize_Num_Flag ==1)
						{//识别数字后
							Flag.Recognize_Num_Flag =0;
							Flag.Routine[4] = Flag.Recognize_Num;//记忆为4号路线的数字
							if(Flag.Recognize_Num ==Flag.Target_Num)
							{//与目标相同
								Flag.Step_Count =74;//四号路线走法
							}
							else
							{//不同的话，直走
								Car_Go(30);//
								Flag.Step_Count ++;
							}
						}
					}break;
					case 4:
					{
						if(Flag.Stop_Flag == 1)
						{
							Servo_Control2(1,45);//向左调整舵机
							Servo_Control2(2,45);
							Flag.Step_Count++;//进入下一个状态
						}
					}break;
					case 5:
					{
						
					}break;
					case 63:
					{//3号路线走法
						Car_Go(15);
						Flag.Step_Count++;//进入下一个状态
					}break;
					case 64:
					{//转弯
						if(Flag.Stop_Flag == 1)
						{
							Car_Spin(0);
							Flag.Step_Count++;//进入下一个状态
						}
					}break;
					case 65:
					{//直行
						if(Flag.Stop_Flag == 1)
						{
							Car_Go(50);
							Flag.Step_Count++;//进入下一个状态
						}
					}break;
					case 66:
					{//点灯，返回标志位置1
						if(Flag.Stop_Flag == 1)
						{
							LED_G_On();
							Flag.Back_Flag = 1;
							Flag.Current_Position = 3;
						}
					}break;
					case 74:
					{//4号路线走法
						
					}break;
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
//		int temp3 = Encoder.Speed[3];    // 上位机需要整数参数，转换一下
//		int temp4 = Encoder.Distance[3];    // 上位机需要整数参数，转换一下
		receiving_process();
		set_computer_value(SEND_FACT_CMD, 0x02,&temp1, 1);
		set_computer_value(SEND_FACT_CMD, 0x01,&temp2, 1);
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
				
				
//				OLED_ShowBNum(0,2,speed_pid.Kp,4,16);
//				OLED_ShowBNum(48,2,speed_pid.Ki,2,16);
//				OLED_ShowBNum(72,2,speed_pid.Kd,3,16);
				
				OLED_ShowBNum(0,4,Encoder.Speed[2],3,16);
				OLED_ShowBNum(0,6,Encoder.Speed[3],3,16);
				OLED_ShowBNum(48,4,Encoder.Distance[2],3,16);
				OLED_ShowBNum(48,6,Encoder.Distance[3],3,16);
				OLED_ShowNum(96,2,SensorData1.X,3,16);
				OLED_ShowNum(96,4,SensorData1.Y,3,16);
				OLED_ShowNum(96,6,SensorData1.D.Float_Data,3,16);
				
				
				if(Keys[1].Single_Flag == 1)
				{
					Keys[1].Single_Flag = 0;
					Car_Go(-50);  
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
