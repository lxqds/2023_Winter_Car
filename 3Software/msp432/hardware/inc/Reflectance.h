#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
void Reflectance_Init(void);

uint8_t Reflectance_Read(uint32_t time);
int32_t Reflectance_Position(uint8_t data);
void Reflectance_Start(void);
uint8_t Reflectance_End(void);

#endif /* REFLECTANCE_H_ */
