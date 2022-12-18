#include "Reflectance.h"//ѭ��
#include "bsp_motor.h"
#include "Control.h"


int Motor_A,Motor_B,Target_A,Target_B;

float Velocity_KP = 8.6;
float Velocity_KI = 8.6;
float Turn_KP = 0.2;
float Turn_KD = 0.1;
//1m���ٶ�
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
	Target_B=(velocity-turn);      //���ֲ���
}
//PID�ջ�����
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b)
{
			uint8_t Flag_Way=2;
	    if(Flag_Way>=2)//Ѳ��ģʽ�£�ֻ��������ת
			{
			  if(motor_a<0)motor_a=0;
				if(motor_b<0)motor_b=0;
			}			
			motor_speed_all(0,motor_a);
		
		  motor_speed_all(3,motor_b);
     //SERVO=servo;	
}
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=950;    //===PWM������7200 ������6900
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		

}
int PI_A (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //����ƫ��
	 Pwm += Velocity_KP*(Error-Last_Error)+Velocity_KI*Error;   //����ʽPI������
	 Last_Error=Error;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int PI_B (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //����ƫ��
	 Pwm += Velocity_KP*(Error-Last_Error)+Velocity_KI*Error;   //����ʽPI������
	 Last_Error=Error;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}

int PI_A2 (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //����ƫ��
	 Pwm += Velocity_KP2*(Error-Last_Error)+Velocity_KI2*Error;   //����ʽPI������
	 Last_Error=Error;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int PI_B2 (int Encoder,int Target)
{ 	
	 static int Error,Pwm,Last_Error;
	 Error = Target-Encoder;                //����ƫ��
	 Pwm += Velocity_KP2*(Error-Last_Error)+Velocity_KI2*Error;   //����ʽPI������
	 Last_Error=Error;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}


