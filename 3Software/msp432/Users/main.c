#include "myconfig.h"


void Menudisplay(void);
void PID_Data_Send(void);

int main(void)
{
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
	
	set_p_i_d(&move_pid,50,0,50);
	set_p_i_d(&move_pid2,50,0,50);
	set_p_i_d(&speed_pid,0.45,0.03,0);
	set_p_i_d(&speed_pid2,0.45,0.03,0);
	
	set_pid_target(&move_pid2,50);
	set_pid_target(&move_pid,50);

	uint32_t data = 0x01020304;
	char *p = (char *)&data;
	printf("0x0%x\n",*p);//判断大小端，0x04为小端，0x01为大端
	set_computer_value(SEND_START_CMD,0x01,NULL,1);
	for(;;)
	{
		Reflectance_Data = Reflectance_Read2();
		Menudisplay();
		PID_Data_Send();
		
		if(SensorData1.D.Float_Data&&Flag.Recognize_Num_Flag ==0)
		{
			static uint8_t Last_Num,Temp_Num;//创建临时变量存储判断数字
			Temp_Num = SensorData1.D.Float_Data;//暂存变量
			if(Temp_Num == Last_Num)			//
			{
				Flag.Recognize_Num_Count++;
				if(Flag.Recognize_Num_Count>=10)
				{
					Flag.Recognize_Num_Flag = 1;//识别到数字后置标志位为1,待处理数字
					Flag.Target_Num = Temp_Num;//设置目标病房
				}
			}
			Last_Num = Temp_Num;
		}
		

		if(Flag.Load_drug == 1&&Flag.Recognize_Num_Flag == 1)//装有药以及识别到数字后开始行动
		{
			
			if(Flag.Target_Num ==1)
			{
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
							Car_Spin(0);
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
			
		}
		
		
		
	}
	//return 0;
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
				//OLED_ShowBin(1,4,Reflectance_Data,8,16);
				
//				OLED_ShowBNum(0,2,move_pid.Kp,4,16);
//				OLED_ShowBNum(48,2,move_pid.Ki,2,16);
//				OLED_ShowBNum(72,2,move_pid.Kd,3,16);
				
				OLED_ShowBNum(0,2,speed_pid.Kp,4,16);
				OLED_ShowBNum(48,2,speed_pid.Ki,2,16);
				OLED_ShowBNum(72,2,speed_pid.Kd,3,16);
				
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
					Car_Go(50);  
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
