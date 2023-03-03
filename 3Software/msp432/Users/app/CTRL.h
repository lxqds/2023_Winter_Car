#ifndef __CTRL_H
#define	__CTRL_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef struct 
{
	float q1,q2,q3,q4;
	float Setpoint,Output,Intergral,Last_error;
	uint32_t timer;
	float error;
	float Actual;
}CTRL;


#endif
