#include "myconfig.h"


void Menudisplay(void);
void PID_Data_Send(void);
void Back_Routine(uint8_t Routine);
void track_car_task();
void Send_medicine_car_task();

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
		
		set_p_i_d(&Dir_pid,1,0,2);
		set_p_i_d(&Dir_pid2,1,0,2);
		
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
		
		//菜单显示
		Menudisplay();
//		PID_Data_Send();
		//循迹双车
		track_car_task();
		//送药车
		Send_medicine_car_task();
	}
	
}
void track_car_task()
{
	if(Flag.car1_start_flag	==1){
		Flag.car2_start_flag	=0;
		Flag.car3_start_flag	=0;
		Flag.car4_start_flag	=0;
		
        if(flag_car1_task1 == car_none1){
						run_send(Flag.car1_start_flag,Flag.car1_circle_count,Flag.car1_distance,Encoder.Distance[0]);
						Car_Go(578);
						flag_car1_task1 = car_run1;
        }
        if(flag_car1_task1 == car_run1){
            if(Encoder.Distance[0] > 550&&Flag.CrossRoad_Flag ==1){
								LED_RED_Off();
                flag_car1_task1 = car_stop1;
            }else{
							
                        if(Flag.CrossRoad_Flag ==1){
                            LED_R_On();
                        }
                        else{
                            LED_R_Off();
                        }
                }
        }
				if(flag_car1_task1 == car_stop1){
                    LED_RED_On();
					Flag.car1_start_flag = 0;
					Flag.Is_EnMOTOR = 0;
					run_send(Flag.car1_start_flag,Flag.car1_circle_count,Flag.car1_distance,Encoder.Distance[0]);
				}

		
	}else if(Flag.car2_start_flag == 1){
		
	}else if(Flag.car3_start_flag == 1){
		
	}else if(Flag.car4_start_flag == 1){
		
	}
	
}
void Send_medicine_car_task()
{
			{//识别数字滤波
			if(SensorData1.D.Float_Data)
			{
				static uint8_t Last_Num2,Temp_Num2;//创建临时变量存储判断数字
				Temp_Num2 = SensorData1.D.Float_Data;//暂存变量
				if(Temp_Num2 == Last_Num2)			//
				{
					Flag.Recognize_Num_Count++;
					if(Flag.Recognize_Num_Count>=10)
					{
						Flag.Recognize_Num2 = Temp_Num2;//识别数字滤波
					}
				}
				Last_Num2 = Temp_Num2;
			}
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
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(70);
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
								LED_R_On();
								
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
								Car_Go(40);
								LED_B_Off();
								LED_R_On();
							}
						}break;
						case 3:
						{//检测到停止标志再走1cm
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);
							}
							if(Flag.Stop_Flag ==1)//到达位置点亮led
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_R_On();
								Flag.Step_Count++;
								LED_B_Off();
								LED_R_On();
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
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
								Flag.Step_Count++;
								Car_Go(50);
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
								LED_R_On();
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
								Car_Go(60);
								LED_R_On();
							}
						}break;	
						case 8:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
								LED_R_On();
							}
						}break;
					}
				}
				else if(Flag.Target_Num ==2)
				{//前往2病房
					switch(Flag.Step_Count)
					{
						case 0:
						{//直走80cm
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(70);
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
								LED_R_On();
								
								Flag.Is_EnMOTOR = 0;//电机失能
								Flag.Step_Count++;
								Car_Spin(1);
								LED_B_On();
							}
						}break;
						case 2:
						{//转完之后直走
							if(Flag.Stop_Flag ==1)
							{
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
								Flag.Step_Count++;
								Car_Go(40);
								LED_B_Off();
								LED_R_On();
							}
						}break;
						case 3:
						{//检测到停止标志再走5cm
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);
							}
							if(Flag.Stop_Flag ==1)//到达位置点亮led
							{//记得清零
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_R_On();
								Flag.Step_Count++;
								LED_B_Off();
								LED_R_On();
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
								Car_Spin(2);//自向右转180度
							}
						}break;
						case 5:
						{//检测到转完180度后停下
							if(Flag.Stop_Flag ==1)//向右转180度后车子停下
							{
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
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
								LED_R_On();
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
								Car_Go(60);
								LED_R_On();
							}
						}break;	
						case 8:
						{
							if(Flag.CrossRoad_Flag == 1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Flag.Step_Count++;
								LED_R_On();
							}
						}break;
					}
				}
			}
			else if(Flag.Load_drug == 1&&Flag.Target_Num_Flag == 1&&Flag.GO_Flag == 1)
			{//装药，识别数字，去送药
				switch(Flag.Step_Count)
				{
					{//3-8号路线走法
					case 0:
					{//直走125
						Flag.Step_Count++;
						Car_Go(130);
						Servo_Control2(2,70);
					}break;
					case 1:
					{//识别到数字停下，扫描数字
						if(SensorData1.D.Float_Data)
						{	
							Flag.Step_Count++;
							Car_Go(1);
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
							Flag.Stop_Flag =0;
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
								Car_Go(40);
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
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							if(Flag.Num_Recognize[0] == Flag.Target_Num)
							{//左转
								Flag.Step_Count=10;//进入下一个状态
								Car_Spin(0);
							}
							else if(Flag.Num_Recognize[1] == Flag.Target_Num)
							{//右转
								Flag.Step_Count=20;//进入下一个状态
								Car_Spin(1);

							}
							else 
							{//直走
								Flag.Step_Count=30;//进入下一个状态
								Servo_Control2(2,70);
								Car_Go(60);
							}
						}
					}break;
					}
					{//3号走法
					case 10:
					{//车子转完弯停下来后
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(40);
						}
					}break;
					case 11:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(1);
						}
						if(Flag.Stop_Flag ==1)	
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Flag.Step_Count++;
							LED_B_Off();
							LED_R_On();
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
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(40);
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
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Flag.Step_Count++;
							Car_Spin(1);
						}
					}break;
					case 15:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(80);
						}
					}break;
					case 16:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag =0;
							Car_Go(70);
							Flag.Step_Count++;
						}
					}break;
					case 17:
					{
						if(Flag.Stop_Flag ==1)
						{
							LED_B_Off();
							LED_R_On();
						}
					}break;
					}
					{//4号走法
					case 20:
					{//车子转完弯停下来后
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(40);
							Flag.Step_Count++;
						}
					}break;
					case 21:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(1);
						}
						if(Flag.Stop_Flag ==1)	
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Flag.Step_Count++;
							LED_B_Off();
							LED_R_On();
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
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(40);
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
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Flag.Step_Count++;
							Car_Spin(0);
						}
					}break;
					case 25:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;
							Car_Go(80);
						}
					}break;
					case 26:
					{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.CrossRoad_Flag =0;
							Car_Go(70);
							Flag.Step_Count++;
						}
					}break;
					case 27:
					{
						if(Flag.Stop_Flag ==1)
						{
							LED_B_Off();
							LED_R_On();
						}
					}break;
					}
					
					{//远端走法
					{//远端走法
					case 30:
					{//识别到数字停下，扫描数字
						if(SensorData1.D.Float_Data&&Encoder.Distance[2]>=40)
						{	
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(1);
							Flag.Step_Count++;
						}
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
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Step_Count++;//进入下一个状态
							Car_Go(30);
						}
					}break;
					case 33:{
						if(Flag.CrossRoad_Flag == 1)
						{
							Flag.Step_Count++;//进入下一个状态
							Flag.CrossRoad_Flag = 0;
							Car_Go(8);
							Servo_Control2(2,70);
						}
					}break;
					case 34:
					{
						if(Flag.Stop_Flag ==1)
						{
							
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
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
					}
					{//远端左
					case 40:
					{//远端左
						 if(Flag.Stop_Flag ==1)
						 {
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(50);
							Flag.Step_Count++;
						 }
					}break;
					case 41:
					{
						if((SensorData1.D.Float_Data&&Encoder.Distance[0]>40)||Flag.Stop_Flag ==1)
						{	
							Car_Go(1);
							Flag.Step_Count++;
						}
					}break;
					case 42:
					{	
						if(Flag.Stop_Flag ==1)
						{
							Flag.Servo_Scan_Flag3 = 1;
							Flag.Step_Count++;
						}
					}break;
					case 43:
					{	
						if(Flag.Servo_Scan_Flag3 == 0)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Servo_Control2(2,70);
							Car_Go(30);
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
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							if(Flag.Target_Num == Flag.Num_Recognize3[0])
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
					}
					{//远端右
					case 80:
					{//远端右
						 if(Flag.Stop_Flag ==1)
						 {
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(50);
							Flag.Step_Count++;
						 }
					}break;
					case 81:
					{
						if(SensorData1.D.Float_Data||Flag.Stop_Flag ==1)
						{	
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(1);
							Flag.Step_Count++;
						}
					}break;
					case 82:
					{	
						if(Flag.Stop_Flag ==1)
						{
							Flag.Servo_Scan_Flag3 = 1;
							Flag.Step_Count++;
						}
					}break;
					case 83:
					{	
						if(Flag.Servo_Scan_Flag3 == 0)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Servo_Control2(2,70);
							Car_Go(30);
							Flag.Step_Count++;
						}
					}break;
					case 84:
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
							if(Flag.Target_Num == Flag.Num_Recognize3[0])
							{
								Car_Spin(0);
								Flag.Step_Count=85;
							}
							else
							{
								Car_Spin(1);
								Flag.Step_Count = 100;
							}
						}
					}break;
					}
					
					{//5号路线走法
					case 45:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(40);
							Flag.Step_Count++;
						}
					}break;
					case 46:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(1);
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							LED_R_On();
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
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(40);
							Flag.Step_Count++;
						}
					}break;
					case 49:
					{//右转
						if(Reflectance_Data ==0b11100000||Reflectance_Data ==0b11000000||Reflectance_Data ==0b11110000)
						{
							Car_Go(10);
							Flag.Step_Count++;
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
						}
					}break;
					case 50:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Car_Spin(1);
							Flag.Step_Count++;
						}
					}break;
					case 51:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(80);
							Flag.Step_Count++;
						}
					}break;
					case 52:
					{
						if(Reflectance_Data ==0b11100000||Reflectance_Data ==0b11000000||Reflectance_Data ==0b11110000)
						{
							Car_Go(10);
							Flag.Step_Count++;
						}
					}break;
					case 53:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Car_Spin(1);//右转
							Flag.Step_Count++;
						}
					}break;
					case 54:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Car_Go(250);//直走240
						}
						if(Encoder.Distance[2]>=230.f)
						{
							Flag.CrossRoad_Flag =0;//标志位清零
							Flag.Step_Count++;
						}
					}break;
					case 55:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(1);//到点停止
							LED_R_On();
						}
					}break;

					}
					{//6号路线走法
						case 60:
						{
							if(Flag.Stop_Flag ==1)
							{
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
								Car_Go(40);
								Flag.Step_Count++;
							}
						}break;
						case 61:
						{
							if(Flag.CrossRoad_Flag ==1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_R_On();
								Flag.Step_Count++;
							}
						}break;
						case 62:
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
						case 63:
						{
							if(Flag.Stop_Flag ==1)
							{
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
								Car_Go(40);
								Flag.Step_Count++;
							}
						}break;
						case 64:
						{//左转
							if(Reflectance_Data ==0b00111000||Reflectance_Data ==0b00011000||Reflectance_Data ==0b01111000)
							{
								Car_Go(10);
								Flag.Step_Count++;
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
							}
						}break;
						case 65:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Car_Spin(0);
								Flag.Step_Count++;
							}
						}break;
						case 66:
						{
							if(Flag.Stop_Flag ==1)
							{
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
								Car_Go(80);
								Flag.Step_Count++;
							}
						}break;
						case 67:
						{
							if(Reflectance_Data ==0b11100000||Reflectance_Data ==0b11000000||Reflectance_Data ==0b11110000)
							{
								Car_Go(10);
								Flag.Step_Count++;
							}
						}break;
						case 68:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Car_Spin(1);//右转
								Flag.Step_Count++;
							}
						}break;
						case 69:
						{
							if(Flag.Stop_Flag ==1)
							{
								if(Flag.CrossRoad_Flag == 1){
									Flag.CrossRoad_Flag = 0;
								}
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Car_Go(250);//直走250
							}
							if(Encoder.Distance[2]>=230.f)
							{
								Flag.CrossRoad_Flag =0;//标志位清零
								Flag.Step_Count++;
							}
						}break;
						case 70:
						{
							if(Flag.CrossRoad_Flag ==1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);//到点停止
								LED_R_On();
							}
						}break;
					}
					
					{//7号路线走法
					case 85:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(40);
							Flag.Step_Count++;
						}
					}break;
					case 86:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(1);
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							LED_R_On();
							Flag.Step_Count++;
						}
					}break;
					case 87:
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
					case 88:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(40);
							Flag.Step_Count++;
						}
					}break;
					case 89:
					{//右转
						if(Reflectance_Data ==0b11100000||Reflectance_Data ==0b11000000||Reflectance_Data ==0b11110000)
						{
							Car_Go(10);
							Flag.Step_Count++;
						}
						if(Flag.Stop_Flag ==1)
						{
							Flag.Step_Count++;
						}
					}break;
					case 90:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Car_Spin(1);
							Flag.Step_Count++;
						}
					}break;
					case 91:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Car_Go(80);
							Flag.Step_Count++;
						}
					}break;
					case 92:
					{//左转
						if(Reflectance_Data ==0b00111000||Reflectance_Data ==0b00011000||Reflectance_Data ==0b01111000)
						{
							Car_Go(10);
							Flag.Step_Count++;
						}
					}break;
					case 93:
					{
						if(Flag.Stop_Flag ==1)
						{
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Car_Spin(0);//左转
							Flag.Step_Count++;
						}
					}break;
					case 94:
					{
						if(Flag.Stop_Flag ==1)
						{
							if(Flag.CrossRoad_Flag == 1){
								Flag.CrossRoad_Flag = 0;
							}
							Flag.Stop_Flag = 1;//置标志位
							Flag.Start_Line_Flag = 0;
							Flag.Stop_Count = 0;
							Car_Go(250);//直走250
						}
						if(Encoder.Distance[2]>=230.f)
						{
							Flag.CrossRoad_Flag =0;//标志位清零
							Flag.Step_Count++;
						}
					}break;
					case 95:
					{
						if(Flag.CrossRoad_Flag ==1)
						{
							Flag.CrossRoad_Flag = 0;
							Car_Go(1);//到点停止
							LED_R_On();
						}
					}break;
					}
					
					
					{//8号路线走法
						case 100:
						{
							if(Flag.Stop_Flag ==1)
							{
								Car_Go(40);
								Flag.Step_Count++;
							}
						}break;
						case 101:
						{
							if(Flag.CrossRoad_Flag ==1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(5);
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								LED_R_On();
								Flag.Step_Count++;
							}
						}break;
						case 102:
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
						case 103:
						{
							if(Flag.Stop_Flag ==1)
							{
								Car_Go(40);
								Flag.Step_Count++;
							}
						}break;
						case 104:
						{//左转
							if(Reflectance_Data ==0b00111000||Reflectance_Data ==0b00011000||Reflectance_Data ==0b01111000)
							{
								Car_Go(10);
								Flag.Step_Count++;
							}
							if(Flag.Stop_Flag ==1)
							{
								Flag.Step_Count++;
							}
						}break;
						case 105:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Car_Spin(0);
								Flag.Step_Count++;
							}
						}break;
						case 106:
						{
							if(Flag.Stop_Flag ==1)
							{
								Car_Go(80);
								Flag.Step_Count++;
							}
						}break;
						case 107:
						{
							if(Reflectance_Data ==0b00111000||Reflectance_Data ==0b00011000||Reflectance_Data ==0b01111000)
							{
								Car_Go(10);
								Flag.Step_Count++;
							}
						}break;
						case 108:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Car_Spin(0);//左转
								Flag.Step_Count++;
							}
						}break;
						case 109:
						{
							if(Flag.Stop_Flag ==1)
							{
								Flag.Stop_Flag = 1;//置标志位
								Flag.Start_Line_Flag = 0;
								Flag.Stop_Count = 0;
								Car_Go(250);//直走250
							}
							if(Encoder.Distance[2]>=230.f)
							{
								Flag.CrossRoad_Flag =0;//标志位清零
								Flag.Step_Count++;
							}
						}break;
						case 110:
						{
							if(Flag.CrossRoad_Flag ==1)
							{
								Flag.CrossRoad_Flag = 0;
								Car_Go(1);//到点停止
								LED_R_On();
							}
						}break;
					}
					}
				}
			}
			
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
				else if(Keys[1].Single_Flag == 1){
					Keys[1].Single_Flag = 0;
					New_State = State3;
				}
				else if(Keys[1].Double_Flag == 1){
					Keys[1].Double_Flag = 0;
					New_State = State4;
				}
			}break;
			case State1:
			{
				set_motor_enable();
				OLED_ShowString(0,0,"Function1",16);
				OLED_ShowBin(0,2,Reflectance_Data,8,16);
				
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
				OLED_ShowNum(72,2,Flag.Num_Recognize2[0],1,16);
				OLED_ShowNum(80,2,Flag.Num_Recognize2[1],1,16);
				OLED_ShowNum(88,2,Flag.Num_Recognize2[2],1,16);
				OLED_ShowNum(96,2,Flag.Num_Recognize2[3],1,16);
				OLED_ShowNum(104,2,Flag.Num_Recognize2[4],1,16);
				OLED_ShowNum(112,2,Flag.Num_Recognize2[5],1,16);
				OLED_ShowNum(120,2,Flag.Num_Recognize2[6],1,16);
				OLED_ShowNum(128,2,Flag.Num_Recognize2[7],1,16);
				
				OLED_ShowNum(96,0,Flag.Step_Count,3,16);
				OLED_ShowNum(96,4,Flag.Num_Recognize[0],1,16);
				OLED_ShowNum(104,4,Flag.Num_Recognize[1],1,16);
				OLED_ShowNum(112,4,Flag.Num_Recognize3[0],1,16);
				OLED_ShowNum(120,4,Flag.Num_Recognize3[1],1,16);
				OLED_ShowNum(96,6,SensorData1.D.Float_Data,1,16);
				OLED_ShowNum(120,6,Flag.Target_Num,1,16);
				
				
				if(Keys[1].Single_Flag == 1)
				{
					Keys[1].Single_Flag = 0;
					Car_Go(578);				
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
					LED_R_On();
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
			case State3:
			{
				set_motor_enable();
				OLED_ShowString(0,0,"Function3",16);
				OLED_ShowBin(0,2,Reflectance_Data,8,16);
				
				OLED_ShowNum(96,0,flag_car1_task1,3,16);
				
				{//后轮编码器
					OLED_ShowBNum(0,4,Encoder.Speed[2],3,16);
					OLED_ShowBNum(0,6,Encoder.Speed[3],3,16);
					OLED_ShowBNum(48,4,Encoder.Distance[2],3,16);
					OLED_ShowBNum(48,6,Encoder.Distance[3],3,16);
				}
				{//车辆状态
					OLED_ShowNum(80,4,Flag.car1_start_flag,2,16);
					OLED_ShowNum(80,6,Flag.car1_circle_count,2,16);
					OLED_ShowNum(104,4,(uint32_t)Flag.car1_distance,2,16);
					//OLED_ShowNum(104,6,(uint32_t)Encoder.Distance[0],2,16);
				}
				
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					Flag.car1_start_flag	=1;				
				}else if(Keys[0].Double_Flag == 1){
					Keys[0].Double_Flag = 0;
					Flag.car2_start_flag	=1;
				}
				else if(Keys[1].Single_Flag == 1){
					Keys[1].Single_Flag = 0;
					Flag.car3_start_flag	=1;
				}
				else if(Keys[1].Double_Flag == 1){
					Keys[1].Double_Flag = 0;
					Flag.car4_start_flag	=1;
				}
				else if(Keys[0].Long_Press_Flag == 1){
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
				else if(Keys[1].Long_Press_Flag == 1){
					Keys[1].Long_Press_Flag = 0;
					New_State = Main_State;
				}
				
			}break;
			case State4:
			{
				set_motor_enable();
				OLED_ShowString(0,0,"Function4",16);
				OLED_ShowBin(0,2,Reflectance_Data,8,16);
				
				{//后轮编码器
					OLED_ShowBNum(0,4,Encoder.Speed[2],3,16);
					OLED_ShowBNum(0,6,Encoder.Speed[3],3,16);
					OLED_ShowBNum(48,4,Encoder.Distance[2],3,16);
					OLED_ShowBNum(48,6,Encoder.Distance[3],3,16);
				}
				
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					Car_Go(578);				
				}else if(Keys[0].Double_Flag == 1){
					Keys[0].Double_Flag = 0;
					Car_Spin(0);
				}
				else if(Keys[1].Single_Flag == 1){
					Keys[1].Single_Flag = 0;
					Car_Spin(1);
				}
				else if(Keys[1].Double_Flag == 1){
					Keys[1].Double_Flag = 0;
					Car_Spin(2);
				}
				else if(Keys[0].Long_Press_Flag == 1){
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
				else if(Keys[1].Long_Press_Flag == 1){
					Keys[1].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
				
		}
}
