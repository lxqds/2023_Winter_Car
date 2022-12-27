/*
 * @Description: odi 
 * @Author: Liang xiaoqi
 * @Date: 2022-07-20 13:55:23
 * @LastEditTime: 2022-07-27 10:18:37
 * @LastEditors: Liang xiaoqi
 */
#include "bsp_exti.h"

void Motor_Init_bianmaqi(void){
    MAP_GPIO_setAsInputPinWithPullUpResistor(motor_One_A_PORT,motor_One_A_PIN);
	MAP_GPIO_setAsInputPinWithPullUpResistor(motor_One_B_PORT,motor_One_B_PIN);
	MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Two_A_PORT,motor_Two_A_PIN);
	MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Two_B_PORT,motor_Two_B_PIN);
    MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Three_A_PORT,motor_Three_A_PIN);
	MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Three_B_PORT,motor_Three_B_PIN);
    MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Four_A_PORT,motor_Four_A_PIN);
	MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Four_B_PORT,motor_Four_B_PIN);


    MAP_GPIO_clearInterruptFlag(motor_One_A_PORT, motor_One_A_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_One_B_PORT, motor_One_B_PIN); //清理中断标志
	MAP_GPIO_clearInterruptFlag(motor_Two_A_PORT, motor_Two_A_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Two_B_PORT, motor_Two_B_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Three_A_PORT, motor_Three_A_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Three_B_PORT, motor_Three_B_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Four_A_PORT, motor_Four_A_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Four_B_PORT, motor_Four_B_PIN); //清理中断标志

    MAP_GPIO_enableInterrupt(motor_One_A_PORT, motor_One_A_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_One_B_PORT, motor_One_B_PIN);    //使能中断端口
	MAP_GPIO_enableInterrupt(motor_Two_A_PORT, motor_Two_A_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Two_B_PORT, motor_Two_B_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Three_A_PORT, motor_Three_A_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Three_B_PORT, motor_Three_B_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Four_A_PORT, motor_Four_A_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Four_B_PORT, motor_Four_B_PIN);    //使能中断端口

    MAP_GPIO_interruptEdgeSelect(motor_One_A_PORT,motor_One_A_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_interruptEdgeSelect(motor_One_B_PORT,motor_One_B_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
	MAP_GPIO_interruptEdgeSelect(motor_Two_A_PORT,motor_Two_A_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_interruptEdgeSelect(motor_Two_B_PORT,motor_Two_B_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_interruptEdgeSelect(motor_Three_A_PORT,motor_Three_A_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_interruptEdgeSelect(motor_Three_B_PORT,motor_Three_B_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_interruptEdgeSelect(motor_Four_A_PORT,motor_Four_A_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_interruptEdgeSelect(motor_Four_B_PORT,motor_Four_B_PIN,GPIO_LOW_TO_HIGH_TRANSITION);

    MAP_Interrupt_enableInterrupt(INT_PORT4);             //端口总使能 
	MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

}
static uint_fast16_t status; //uint_fast16意思是至少16位，可以更多，配合中断函数使用
typedef struct
{
	int16_t EncoderA;
	int16_t EncoderB;
	int16_t EncoderC;
	int16_t EncoderD;
}Encoder;
void GetEncoder(Encoder*p) //p指针传入，传完记得周期置零
{
	if(status!=0x0000)
	{
		switch(status)
	{
		//电机1
		case motor_One_A_PIN:
		{
			if(motor_One_B_Value ==  1) 
				p->EncoderA--;
			else if(motor_One_B_Value ==  0)
				p->EncoderA++;
		}break;
		case motor_One_B_PIN:
		{
			if(motor_One_A_Value ==  1) 
				p->EncoderA++;
			else if(motor_One_A_Value ==  0)
				p->EncoderA--;
		}break;
		//电机2
		case motor_Two_A_PIN:
		{
			if(motor_Two_B_Value ==  1) 
				p->EncoderB--;
			else if(motor_Two_B_Value ==  0)
				p->EncoderB++;
		}break;
		case motor_Two_B_PIN:
		{
			if(motor_Two_A_Value ==  1) 
				p->EncoderB++;
			else if(motor_Two_A_Value ==  0)
				p->EncoderB--;
		}break;
		//电机3
		case motor_Three_A_PIN:
		{
			if(motor_Three_B_Value ==  1) 
				p->EncoderC--;
			else if(motor_Three_B_Value ==  0)
				p->EncoderC++;
		}break;
		case motor_Three_B_PIN:
		{
			if(motor_Three_A_Value ==  1) 
				p->EncoderC++;
			else if(motor_Three_A_Value ==  0)
				p->EncoderC--;
		}break;
		//电机4
		case motor_Four_A_PIN:
		{
			if(motor_Four_B_Value ==  1) 
				p->EncoderD--;
			else if(motor_Four_B_Value ==  0)
				p->EncoderD++;
		}break;
		case motor_Four_B_PIN:
		{
			if(motor_Four_A_Value ==  1) 
				p->EncoderD++;
			else if(motor_Four_A_Value ==  0)
				p->EncoderD--;
		}break;
	}
	status = 0x0000;//有中断后不在是这个数
	}
}

void motor_PORT_IRQHandler(void)
{
	//uint_fast16_t status; //uint_fast16意思是至少16位，可以更多
    status =  MAP_GPIO_getEnabledInterruptStatus(motor_Three_A_PORT); //获取中断状态
    MAP_GPIO_clearInterruptFlag(motor_Three_A_PORT, status);         //清理中断标志
//	switch(status)
//	{
//		//电机1
//		case motor_One_A_PIN:
//		{
//			if(motor_One_B_Value ==  1) 
//				bianmaqi[0]--;
//			else if(motor_One_B_Value ==  0)
//				bianmaqi[0]++;
//		}break;
//		case motor_One_B_PIN:
//		{
//			if(motor_One_A_Value ==  1) 
//				bianmaqi[0]++;
//			else if(motor_One_A_Value ==  0)
//				bianmaqi[0]--;
//		}break;
//		//电机2
//		case motor_Two_A_PIN:
//		{
//			if(motor_Two_B_Value ==  1) 
//				bianmaqi[1]--;
//			else if(motor_Two_B_Value ==  0)
//				bianmaqi[1]++;
//		}break;
//		case motor_Two_B_PIN:
//		{
//			if(motor_Two_A_Value ==  1) 
//				bianmaqi[1]++;
//			else if(motor_Two_A_Value ==  0)
//				bianmaqi[1]--;
//		}break;
//		//电机3
//		case motor_Three_A_PIN:
//		{
//			if(motor_Three_B_Value ==  1) 
//				bianmaqi[2]--;
//			else if(motor_Three_B_Value ==  0)
//				bianmaqi[2]++;
//		}break;
//		case motor_Three_B_PIN:
//		{
//			if(motor_Three_A_Value ==  1) 
//				bianmaqi[2]++;
//			else if(motor_Three_A_Value ==  0)
//				bianmaqi[2]--;
//		}break;
//		//电机4
//		case motor_Four_A_PIN:
//		{
//			if(motor_Four_B_Value ==  1) 
//				bianmaqi[3]--;
//			else if(motor_Four_B_Value ==  0)
//				bianmaqi[3]++;
//		}break;
//		case motor_Four_B_PIN:
//		{
//			if(motor_Four_A_Value ==  1) 
//				bianmaqi[3]++;
//			else if(motor_Four_A_Value ==  0)
//				bianmaqi[3]--;
//		}break;
//	}
}








