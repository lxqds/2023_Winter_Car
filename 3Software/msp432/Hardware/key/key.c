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
Keys_Init Keys[2]={0,0};


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
			}else
			{
				Key_State_Flag = Key_Press_Once;
				Return_Value = 0;
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
//				for(i=0;i<2;i++)					//消抖清零
//				{
//					Key_Value[i] = 0;
//				}
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
				Key_State_Flag = Key_Press_Release;	//松手进入下一个状态
			}
			
			if(Key_Value[0] > KEY_KEEP_TIMELENTH)
			{
				Key_State_Flag = Key_Press_Keep;	//未松手保持状态
				return KEY1_LONG_PRESS;
			}else if(Key_Value[1] > KEY_KEEP_TIMELENTH)
			{
				Key_State_Flag = Key_Press_Keep;
				return KEY2_LONG_PRESS;
			}
		}
		break;
		case Key_Press_Release:
		{
			if((Key_Value[0] >= 1)&&(Key_Value[0] < KEY_KEEP_TIMELENTH))	//按键检测到按的过程是短按
			{
				Return_Value = KEY1_ONCE_PRESS;
			}else if((Key_Value[1] >= 1)&&(Key_Value[0] < KEY_KEEP_TIMELENTH))	//按键2短按
			{
				Return_Value = KEY2_ONCE_PRESS;
			}else if((Key_Value[0] >= KEY_KEEP_TIMELENTH))						//按键1长按
			{
				Return_Value = KEY1_LONG_PRESS;
			}else if((Key_Value[1] >= KEY_KEEP_TIMELENTH))						//按键2长按
			{
				Return_Value = KEY2_LONG_PRESS;
			}else
			{
				Return_Value = 0;
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

/**
 * @name	Key_Scan2()
 * @brief	按键扫描
 * @param	无
 * @return	按下的按键值	1:KEY1短按 2：KEY1长按 3:KEY2短按 4：KEY2长按
 */
void Key_Scan2(void)
{
	Keys[0].Key_State = KEY1;//读取按键的状态
	Keys[1].Key_State = KEY2;
	
	for(uint8_t i = 0;i < 2;i++)
	{
		switch(Keys[i].Jump_State)
		{
			case 0:
			{
				if(Keys[i].Key_State == 0 )
				{
					Keys[i].Jump_State = 1;
					Keys[i].Key_Down_Time = 0;//开启按键按下的时间判断
				}
			}
			break;
			case 1:
			{
				if(Keys[i].Key_State == 0 )
				{
					Keys[i].Jump_State = 2;
				}
				else
				{
					Keys[i].Jump_State = 0;
				}
			}
			break;
			case 2:
			{
				if(Keys[i].Key_State == 1 &&Keys[i].Key_Down_Time < 100)
				{
					if(Keys[i].Doule_Time_Flag == 0) //按键按下的第一次
					{
						Keys[i].Doule_Time_Flag =1;	//开启在双击判断
						Keys[i].Double_Time = 0;		//初始化时间
					}
					else
					{
						Keys[i].Double_Flag = 1;		//在定时时间内按键按下两次
						Keys[i].Doule_Time_Flag = 0;
					}
					Keys[i].Jump_State = 0;				//回到状态0
				}else if(Keys[i].Key_State == 1 && Keys[i].Key_Down_Time >100)
				{
					Keys[i].Jump_State = 0;
				}
				else
				{
					if(Keys[i].Key_Down_Time == 100)	//如果时间到就置1
					{
						Keys[i].Long_Press_Flag = 1;
					}
					Keys[i].Key_Down_Time++;
				}
			}
			break;
		}
		if(Keys[i].Doule_Time_Flag ==1)
		{
			Keys[i].Double_Time++;					//检测到双击检测开启，计时
			if(Keys[i].Double_Time > 50)				//在相应计时内没有按键按下，则是单击
			{
				Keys[i].Single_Flag = 1;
				Keys[i].Doule_Time_Flag = 0;			//将标志位恢复
			}
		}
	}
}

