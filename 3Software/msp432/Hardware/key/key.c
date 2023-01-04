/*
 * @Description: 
 * @Author: Liang xiaoqi
 * @Date: 2022-07-25 14:47:54
 * @LastEditTime: 2022-07-27 16:11:27
 * @LastEditors: Liang xiaoqi
 */
/**************************************************************************************/

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "key.h"
/* define -----------------------------------------------------------------------------------------------------------------*/



//按键初始化函数
void KEY_Init(void) //IO初始化
{
	MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//注意此函数有响应优先级,KEY1>KEY2!!
uint8_t KEY_Scan(uint8_t mode)
{
	uint16_t i;
	static uint8_t key_up = 1; //按键按松开标志
	if (mode)
		key_up = 1; //支持连按
	if (key_up && (KEY2 == 0 || KEY1 == 0))
	{
		for (i = 0; i < 5000; i++)
			; //去抖动
		key_up = 0;
		if (KEY1 == 0)
			return KEY1_PRES;
		else if (KEY2 == 0)
			return KEY2_PRES;
	}
	else if (KEY2 == 1 && KEY1 == 1)
		key_up = 1;
	return 0; // 无按键按下
}

/**
 * @name	Key_Scan()
 * @brief	按键扫描
 * @param	无
* @return	按下的按键值	1:KEY1短按 2：KEY1长按 3:KEY2短按 4：KEY2长按
 */
uint8_t Key_Scan(void)
{
	static uint8_t Key_State_Flag = Key_Press_Once;//初始状态为检测按下一次
	static uint8_t Key_Value[2]={0};				//初始按键值，用于计量长按短按
	uint8_t i=0,Return_Value=0;
	
	switch(Key_State_Flag)
	{
		case Key_Press_Once:
		{
			if(KEY1 == 0)
			{
				Key_State_Flag = Key_Press_Again;
				Key_Value[0]++;
			}else if(KEY2 == 0)
			{
				Key_State_Flag = Key_Press_Again;	//进入下一个按键状态
				Key_Value[1]++;						//键值++
			}
		}
		break;
		case Key_Press_Again:
		{
			if(KEY1 == 0)
			{
				Key_State_Flag = Key_Press_Keep;
				Key_Value[0]++;
			}else if(KEY2 ==0)
			{
				Key_State_Flag = Key_Press_Keep;
				Key_Value[1]++;
			}else
			{
				Key_State_Flag = Key_Press_Release;	//返回上一个状态
				for(i=0;i<2;i++)					//消抖清零
				{
					Key_Value[i] = 0;
				}
			}
		}
		break;
		case Key_Press_Keep:
		{
			if(KEY1 == 0)
			{
				Key_State_Flag = Key_Press_Keep;	//保持原来的状态
				Key_Value[0]++;
			}else if(KEY2 == 0)
			{
				Key_State_Flag = Key_Press_Keep;
				Key_Value[1]++;
			}else
			{
				Key_State_Flag = Key_Press_Release;
			}
			
			if(Key_Value[0] > KEY_KEEP_TIMELENTH)
			{
				Key_State_Flag = Key_Press_Release;
				return KEY1_LONG_PRESS;
			}else if(Key_Value[1] > KEY_KEEP_TIMELENTH)
			{
				Key_State_Flag = Key_Press_Release;
				return KEY2_LONG_PRESS;
			}
		}
		break;
		case Key_Press_Release:
		{
			if((Key_Value[0] >= 3)&&(Key_Value[0] < KEY_KEEP_TIMELENTH))
			{
				Return_Value = KEY1_ONCE_PRESS;
			}else if((Key_Value[1] >= 3)&&(Key_Value[0] < KEY_KEEP_TIMELENTH))
			{
				Return_Value = KEY2_ONCE_PRESS;
			}
			
			if((KEY1 == 1) &&(KEY2 == 1))//没有按键按下
			{
				Key_State_Flag = Key_Press_Once;
				for(i=0;i<2;i++)
				{
					Key_Value[i] = 0;
				}
			}
		}
		break;
	}
	return Return_Value;
}
