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
    MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Four_A_PORT,motor_Four_A_PIN);
	MAP_GPIO_setAsInputPinWithPullUpResistor(motor_Four_B_PORT,motor_Four_B_PIN);


    MAP_GPIO_clearInterruptFlag(motor_One_A_PORT, motor_One_A_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_One_B_PORT, motor_One_B_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Four_A_PORT, motor_Four_A_PIN); //清理中断标志
    MAP_GPIO_clearInterruptFlag(motor_Four_B_PORT, motor_Four_B_PIN); //清理中断标志

    MAP_GPIO_enableInterrupt(motor_One_A_PORT, motor_One_A_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_One_B_PORT, motor_One_B_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Four_A_PORT, motor_Four_A_PIN);    //使能中断端口
    MAP_GPIO_enableInterrupt(motor_Four_B_PORT, motor_Four_B_PIN);    //使能中断端口

    GPIO_interruptEdgeSelect(motor_One_A_PORT,motor_One_A_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(motor_One_B_PORT,motor_One_B_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(motor_Four_A_PORT,motor_Four_A_PIN,GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(motor_Four_B_PORT,motor_Four_B_PIN,GPIO_LOW_TO_HIGH_TRANSITION);

    MAP_Interrupt_enableInterrupt(motor_Three_A_INT);             //端口总使能 
	MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

}
int bianmaqi[4]={0};

void motor_PORT_IRQHandler(void)
{
    uint32_t status;
    status =  GPIO_getEnabledInterruptStatus(motor_Three_A_PORT); //获取中断状态
     GPIO_clearInterruptFlag(motor_Three_A_PORT, status);         //清理中断标志

/******************电机1*******************/
    if (status & motor_One_A_PIN) //对应MA
    {
        if (motor_One_B_Value ==  1)
        {
            bianmaqi[0]--;
        }
        else if (motor_One_B_Value ==  0)
        {
            bianmaqi[0]++;
        }

    }else if (status & motor_One_B_PIN) //对应MB
    {
        if (motor_One_A_Value == 1)
        {
            bianmaqi[0]++;
        }
        else if(motor_One_A_Value == 0){
             bianmaqi[0]--;
        }
    }else
/******************电机1*******************/


/******************电机4*******************/
    if (status & motor_Four_A_PIN) //对应MA
    {
        if (motor_Four_B_Value ==  1)
        {
            bianmaqi[3]--;
        }
        else if (motor_Four_B_Value ==  0)
        {
            bianmaqi[3]++;
        }

    }else
    if (status & motor_Four_B_PIN) //对应MB
    {
        if (motor_Four_A_Value == 1)
        {
            bianmaqi[3]++;
        }
        else if(motor_Four_A_Value == 0){
             bianmaqi[3]--;
        }
    }
/******************电机4*******************/
}








