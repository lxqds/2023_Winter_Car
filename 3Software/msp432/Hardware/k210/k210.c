/**
  *************************************************************************************************************************
  * @file    k210.c
  * @author  lxq
  * @version V1.0
  * @date    2023-02-21
  * @brief   k210ͨ��ģ��.c�ļ�����
  *************************************************************************************************************************
  * @attention
  *
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "k210.h"
/* define -----------------------------------------------------------------------------------------------------------------*/
SensorData SensorData1;
/**
 * @name	Get_Data_To_Array
 * @brief	接收来自K210的数据
 * @param	字节数据
 * @return	
 */
void Get_Data_To_Array(uint8_t ByteData)
{
	static uint8_t Rec_State =0;
	static uint8_t Rec_Buff[12]={0};
	Rec_Buff[Rec_State] = ByteData;
	switch(Rec_State)
	{
		case 0:if(ByteData ==0x23) Rec_State =1;else Rec_State = 0;break;//判断是否为数据包的头部1
		case 1:if(ByteData ==0x66) Rec_State =2;else Rec_State = 0;break;//判断是否为数据包的头部2
		case 2:Rec_State = 3;break;											//存储一个数据
		case 3:SensorData1.X = Rec_Buff[3]<<8|Rec_Buff[2];Rec_State = 4;break;//把字符型数据转换成int16_t
		case 4:Rec_State = 5;break;											//存储一个数据
		case 5:SensorData1.Y = Rec_Buff[5]<<8|Rec_Buff[4];Rec_State =6;break;//把字符型数据转换成int16_t
		case 6:Rec_State = 7;break;											//存储一个数据
		case 7:Rec_State = 8;break;											//存储一个数据
		case 8:Rec_State = 9;break;											//存储一个数据
		case 9:SensorData1.D.byte[3] = Rec_Buff[9];SensorData1.D.byte[2] = Rec_Buff[8];SensorData1.D.byte[1] = Rec_Buff[7];SensorData1.D.byte[0]=Rec_Buff[6];Rec_State = 10;break;//把字符型数据转换成float
		case 10:if(ByteData == 0x11) Rec_State = 11;else Rec_State = 0;break;//判断是否为数据包的尾部2
		case 11:Rec_State =0;break;
	}	
}	




/*****************************************************END OF FILE*********************************************************/	
