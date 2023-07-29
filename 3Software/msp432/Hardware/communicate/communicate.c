//
// Created by 27825 on 2023/7/29.
//
#include "communicate.h"
uart_car1_data_t uart_car1_send_data1;
uart_car1_data_r uart_car1_rece_data1;

/**
 * @name	Get_Data_To_Array
 * @brief	接收来自432的数据
 * @param	字节数据
 * @return
 */
uint8_t run_rece(uint8_t ByteData)
{
    static uint8_t Rec_State =0;
    static uint8_t Rec_Buff[13]={0};
    Rec_Buff[Rec_State] = ByteData;
    switch(Rec_State)
    {
        case 0:if(ByteData ==0xAA) Rec_State =1;else Rec_State = 0;break;//判断是否为数据包的头部1
        case 1:if(ByteData ==0x55) Rec_State =2;else Rec_State = 0;break;//判断是否为数据包的头部2
        case 2:uart_car1_rece_data1.car1_rece_data.car2_start = Rec_Buff[2];Rec_State = 3;break;//存储uint8_t类型数据
        case 3:uart_car1_rece_data1.car1_rece_data.car2_circle_count = Rec_Buff[3];Rec_State = 4;break;//存储uint8_t类型数据
        case 4:Rec_State = 5;break;                                     //存储一个Byte数据
        case 5:Rec_State = 6;break;                                     //存储一个Byte数据
        case 6:Rec_State = 7;break;                                     //存储一个Byte数据
        case 7:uart_car1_rece_data1.car1_rece_data.car2_distance.Float_Data = Rec_Buff[4]<<24|Rec_Buff[5]<<16|Rec_Buff[6]<<8|Rec_Buff[7];Rec_State = 8;break;//存储float类型数据
        case 8:Rec_State = 9;break;                                     //存储一个Byte数据
        case 9:Rec_State = 10;break;                                    //存储一个Byte数据
        case 10:Rec_State = 11;break;                                   //存储一个Byte数据
        case 11:uart_car1_rece_data1.car1_rece_data.car2_encoder_route.Float_Data = Rec_Buff[8]<<24|Rec_Buff[9]<<16|Rec_Buff[10]<<8|Rec_Buff[11];Rec_State = 12;break;//存储float类型数据
        case 12:if(Rec_Buff[12]==0x66) {
                    Rec_State = 13;
                }else{
                    Rec_State =0;
                    return 1;
                }break;
        case 13:Rec_State =0;return 0;break;
        //case 2:Rec_State = 3;break;
        //case 3:SensorData1.X = Rec_Buff[3]<<8|Rec_Buff[2];Rec_State = 4;break;//把字符型数据转换成int16_t
        //case 4:Rec_State = 5;break;											//存储一个数据
        //case 5:SensorData1.Y = Rec_Buff[5]<<8|Rec_Buff[4];Rec_State =6;break;//把字符型数据转换成int16_t
        //case 6:Rec_State = 7;break;											//存储一个数据
        //case 7:Rec_State = 8;break;											//存储一个数据
        //case 8:Rec_State = 9;break;											//存储一个数据
        //case 9:SensorData1.D.byte[3] = Rec_Buff[9];SensorData1.D.byte[2] = Rec_Buff[8];SensorData1.D.byte[1] = Rec_Buff[7];SensorData1.D.byte[0]=Rec_Buff[6];Rec_State = 10;break;//把字符型数据转换成float
        //case 10:if(ByteData == 0x11) Rec_State = 11;else Rec_State = 0;break;//判断是否为数据包的尾部2
        //case 11:Rec_State =0;break;
    }
		return 0;
}

void run_send(uint8_t car1_start,uint8_t car1_circle_count,float car1_distance,float car1_encoder_route)
{
    uart_car1_send_data1.car1_send_data.head1 = 0xAA;
    uart_car1_send_data1.car1_send_data.head2 = 0x55;
    uart_car1_send_data1.car1_send_data.car1_start = car1_start;
    uart_car1_send_data1.car1_send_data.car1_circle_count = car1_circle_count;
    uart_car1_send_data1.car1_send_data.car1_distance.Float_Data = car1_distance;
    uart_car1_send_data1.car1_send_data.car1_encoder_route.Float_Data = car1_encoder_route;
    uart_car1_send_data1.car1_send_data.tail = 0x66;

    for(uint8_t i=0;i<13;i++){
        MAP_UART_transmitData(EUSCI_A2_BASE,uart_car1_send_data1.Byte[i]);//会发送13个字节
    }
}