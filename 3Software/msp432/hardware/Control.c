#include "Reflectance.h"//循迹
#include "bsp_motor.h"
#include "Control.h"


int Motor_A,Motor_B,Target_A,Target_B;

float Velocity_KP = 8.6;
float Velocity_KI = 8.6;
float Turn_KP = 0.2;
float Turn_KD = 0.1;
//1m的速度
float Velocity_KP2 = 8.6;
float Velocity_KI2 = 8.6;
float Turn_KP2 = 0.32;
float Turn_KD2 = 0.22;



float Turn_PD(int8_t Bias)
{
	static int8_t Last_Bias;float Turn;
	Turn = Bias *Turn_KP +(Bias - Last_Bias)*Turn_KD;
	Last_Bias = Bias;
	return Turn;
}

float Turn_PD2(int8_t Bias)
{
	static int8_t Last_Bias;float Turn;
	Turn = Bias *Turn_KP2 +(Bias - Last_Bias)*Turn_KD2;
	Last_Bias = Bias;
	return Turn;
}

void K_A(float velocity,float turn)
{
	Target_A=(velocity+turn); 
	Target_B=(velocity-turn);      //后轮差速
}
//PID闭环控制
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b)
{
			uint8_t Flag_Way=2;
	    if(Flag_Way>=2)//巡线模式下，只允许电机正转
			{
			  if(motor_a<0)motor_a=0;
				if(motor_b<0)motor_b=0;
			}			
			motor_speed_all(0,motor_a);
		
		  motor_speed_all(3,motor_b);
     //SERVO=servo;	
}
/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=950;    //===PWM满幅是7200 限制在6900
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		

}
int PI_A (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //计算偏差
	 Pwm += Velocity_KP*(Error-Last_Error)+Velocity_KI*Error;   //增量式PI控制器
	 Last_Error=Error;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int PI_B (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //计算偏差
	 Pwm += Velocity_KP*(Error-Last_Error)+Velocity_KI*Error;   //增量式PI控制器
	 Last_Error=Error;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}

int PI_A2 (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //计算偏差
	 Pwm += Velocity_KP2*(Error-Last_Error)+Velocity_KI2*Error;   //增量式PI控制器
	 Last_Error=Error;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int PI_B2 (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //计算偏差
	 Pwm += Velocity_KP2*(Error-Last_Error)+Velocity_KI2*Error;   //增量式PI控制器
	 Last_Error=Error;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}


