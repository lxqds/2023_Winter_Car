#ifndef __Control_H
#define __Control_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>



float Turn_PD(int8_t Bias);
void K_A(float velocity,float turn);
void Set_Pwm(int motor_a,int motor_b);
void Xianfu_Pwm(void);
int PI_A (int Encoder,int Target);
int PI_B (int Encoder,int Target);

float Turn_PD2(int8_t Bias);
int PI_A2 (int Encoder,int Target);
int PI_B2 (int Encoder,int Target);	

#endif
