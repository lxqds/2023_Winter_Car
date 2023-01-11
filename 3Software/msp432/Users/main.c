#include "myconfig.h"

int temp=200;
int data2=20;
void Menudisplay();

int main(void)
{
	SysInit1();
	uart_init0(115200);
	uart_init(115200);
	KEY_Init();
	LED_Init();
	OLED_Init();
	Reflectance_Init2();
	TimA0_Int_Init(9999,48);//48分频，分频后1M，计数10000，
	Servo_Init();
	Motor_Init();
	Motor_Init_bianmaqi();
	MPU_Init();
	
	protocol_init();
	PID_param_init();
	set_pid_target(&move_pid,500);
	uint32_t data = 0x01020304;
	char *p = (char *)&data;
	printf("0x0%x\n",*p);//判断大小端，0x04为小端，0x01为大端
	set_computer_value(SEND_START_CMD,0x01,NULL,1);
	for(;;)
	{
		receiving_process();
		set_computer_value(SEND_TARGET_CMD, 0x01, &temp, 1);
		set_computer_value(SEND_FACT_CMD, 0x01, &data2, 1);
		Reflectance_Data = Reflectance_Read2();
		Menudisplay();
		
	}
	return 0;
}

void Menudisplay()
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
			case State1:
			{
				OLED_ShowString(0,0,"Function1",16);
				OLED_ShowNum(0,2,ble_com,3,16);
				//OLED_ShowBin(1,4,Reflectance_Data,8,16);

				OLED_ShowBNum(0,4,Encoder.Speed[2],6,16);
				OLED_ShowBNum(0,6,Encoder.Speed[3],6,16);
				OLED_ShowBNum(64,4,Encoder.Distance[2],6,16);
				OLED_ShowBNum(64,6,Encoder.Distance[3],6,16);
//				OLED_ShowNum(96,4,(uint8_t)Encoder.Speed[2],3,16);
//				OLED_ShowNum(96,6,(uint8_t)Encoder.Speed[3],3,16);
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
			case State2:
			{
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