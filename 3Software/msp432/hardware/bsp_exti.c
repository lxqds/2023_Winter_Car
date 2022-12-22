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
int8_t bianmaqi[4]={0};

void motor_PORT_IRQHandler(void)
{
    uint_fast16_t status; //uint_fast16意思是至少16位，可以更多
    status =  MAP_GPIO_getEnabledInterruptStatus(motor_Three_A_PORT); //获取中断状态
    MAP_GPIO_clearInterruptFlag(motor_Three_A_PORT, status);         //清理中断标志

	switch(status)
	{
		//电机1
		case motor_One_A_PIN:
		{
			if(motor_One_B_Value ==  1) 
				bianmaqi[0]--;
			else if(motor_One_B_Value ==  0)
				bianmaqi[0]++;
		}break;
		case motor_One_B_PIN:
		{
			if(motor_One_A_Value ==  1) 
				bianmaqi[0]++;
			else if(motor_One_A_Value ==  0)
				bianmaqi[0]--;
		}break;
		//电机2
		case motor_Two_A_PIN:
		{
			if(motor_Two_B_Value ==  1) 
				bianmaqi[1]--;
			else if(motor_Two_B_Value ==  0)
				bianmaqi[1]++;
		}break;
		case motor_Two_B_PIN:
		{
			if(motor_Two_A_Value ==  1) 
				bianmaqi[1]++;
			else if(motor_Two_A_Value ==  0)
				bianmaqi[1]--;
		}break;
		//电机3
		case motor_Three_A_PIN:
		{
			if(motor_Three_B_Value ==  1) 
				bianmaqi[2]--;
			else if(motor_Three_B_Value ==  0)
				bianmaqi[2]++;
		}break;
		case motor_Three_B_PIN:
		{
			if(motor_Three_A_Value ==  1) 
				bianmaqi[2]++;
			else if(motor_Three_A_Value ==  0)
				bianmaqi[2]--;
		}break;
		//电机4
		case motor_Four_A_PIN:
		{
			if(motor_Four_B_Value ==  1) 
				bianmaqi[3]--;
			else if(motor_Four_B_Value ==  0)
				bianmaqi[3]++;
		}break;
		case motor_Four_B_PIN:
		{
			if(motor_Four_A_Value ==  1) 
				bianmaqi[3]++;
			else if(motor_Four_A_Value ==  0)
				bianmaqi[3]--;
		}break;
	}

///******************电机1*******************/
//    if (status & motor_One_A_PIN) //对应MA
//    {
//        if (motor_One_B_Value ==  1)
//        {
//            bianmaqi[0]--;
//        }
//        else if (motor_One_B_Value ==  0)
//        {
//            bianmaqi[0]++;
//        }

//    }else if (status & motor_One_B_PIN) //对应MB
//    {
//        if (motor_One_A_Value == 1)
//        {
//            bianmaqi[0]++;
//        }
//        else if(motor_One_A_Value == 0){
//             bianmaqi[0]--;
//        }
//    }else
///******************电机1*******************/


///******************电机4*******************/
//    if (status & motor_Four_A_PIN) //对应MA
//    {
//        if (motor_Four_B_Value ==  1)
//        {
//            bianmaqi[3]--;
//        }
//        else if (motor_Four_B_Value ==  0)
//        {
//            bianmaqi[3]++;
//        }

//    }else
//    if (status & motor_Four_B_PIN) //对应MB
//    {
//        if (motor_Four_A_Value == 1)
//        {
//            bianmaqi[3]++;
//        }
//        else if(motor_Four_A_Value == 0){
//             bianmaqi[3]--;
//        }
//    }
///******************电机4*******************/
}








