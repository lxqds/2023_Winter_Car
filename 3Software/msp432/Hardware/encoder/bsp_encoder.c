/**
  *************************************************************************************************************************
  * @file    encoder.c
  * @author  lxq
  * @version V1.0
  * @date    2023-01-03
  * @brief   encoder模块.c文件配置
  *************************************************************************************************************************
  * @attention
  *
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_encoder.h"

/* define -----------------------------------------------------------------------------------------------------------------*/
//int8_t bianmaqi[4]={0};
Encoder_Init Encoder ;

/**
 * @name	Motor_Init_bianmaqi
 * @brief	电机初始化
 * @param	无
 * @return	无
 */
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
/**
 * @name	motor_PORT_IRQHandler
 * @brief	电机编码器捕获外部上下沿中断
 * @param	无
 * @return	无
 */
void Encoder_Scan(void)
{
	uint8_t i=0;
//	Encoder.Encoder[0] = Encoder.Encoder_Once[0];	//读取累计的编码器值
//	Encoder.Encoder[1] = Encoder.Encoder_Once[1];
//	Encoder.Encoder[2] = Encoder.Encoder_Once[2];
//	Encoder.Encoder[3] = Encoder.Encoder_Once[3];
//	
//	Encoder.Encoder_Total[0] +=  Encoder.Encoder[0];	//累计放入到累计值中
//	Encoder.Encoder_Total[1] +=  Encoder.Encoder[1];
//	Encoder.Encoder_Total[2] +=  Encoder.Encoder[2];
//	Encoder.Encoder_Total[3] +=  Encoder.Encoder[3];
	for (i = 0;i<4; i++)
	{
		Encoder.Encoder[i] = Encoder.Encoder_Once[i];				//读取累计的编码器值
		Encoder.Encoder_Total[i] +=  Encoder.Encoder[i];			//累计放入到累计值中
		Encoder.Encoder_Once[i] = 0;								//清零
		Encoder.Distance[i] += Encoder.Encoder[i]*PER_PULSE_CM;		//距离等于脉冲数×每一个脉冲走过cm数
		Encoder.Speed[i] = Encoder.Encoder[i]*PER_PULSE_CM/0.01f;	//速度等于距离除于时间
	}
}

/**
 * @name	motor_PORT_IRQHandler
 * @brief	电机编码器捕获外部上下沿中断
 * @param	无
 * @return	无
 */
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
				Encoder.Encoder_Once[0]--;
			else if(motor_One_B_Value ==  0)
				Encoder.Encoder_Once[0]++;
		}break;
		case motor_One_B_PIN:
		{
			if(motor_One_A_Value ==  1) 
				Encoder.Encoder_Once[0]++;
			else if(motor_One_A_Value ==  0)
				Encoder.Encoder_Once[0]--;
		}break;
		//电机2
		case motor_Two_A_PIN:
		{
			if(motor_Two_B_Value ==  1) 
				Encoder.Encoder_Once[1]++;
			else if(motor_Two_B_Value ==  0)
				Encoder.Encoder_Once[1]--;
		}break;
		case motor_Two_B_PIN:
		{
			if(motor_Two_A_Value ==  1) 
				Encoder.Encoder_Once[1]--;
			else if(motor_Two_A_Value ==  0)
				Encoder.Encoder_Once[1]++;
		}break;
		//电机3
		case motor_Three_A_PIN:
		{
			if(motor_Three_B_Value ==  1) 
				Encoder.Encoder_Once[2]--;
			else if(motor_Three_B_Value ==  0)
				Encoder.Encoder_Once[2]++;
		}break;
		case motor_Three_B_PIN:
		{
			if(motor_Three_A_Value ==  1) 
				Encoder.Encoder_Once[2]++;
			else if(motor_Three_A_Value ==  0)
				Encoder.Encoder_Once[2]--;
		}break;
		//电机4
		case motor_Four_A_PIN:
		{
			if(motor_Four_B_Value ==  1) 
				Encoder.Encoder_Once[3]++;
			else if(motor_Four_B_Value ==  0)
				Encoder.Encoder_Once[3]--;
		}break;
		case motor_Four_B_PIN:
		{
			if(motor_Four_A_Value ==  1) 
				Encoder.Encoder_Once[3]--;
			else if(motor_Four_A_Value ==  0)
				Encoder.Encoder_Once[3]++;
		}break;
	}
}


/*****************************************************END OF FILE*********************************************************/	
