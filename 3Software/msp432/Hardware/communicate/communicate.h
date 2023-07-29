//
// Created by 27825 on 2023/7/29.
//

#ifndef INC_2021EC_F_CAR_COMMUNICATE_H
#define INC_2021EC_F_CAR_COMMUNICATE_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
typedef union float_data
{
    unsigned char byte[4];
    float Float_Data;
}float_byte_t;
typedef struct send_pack
{
    uint8_t head1;
    uint8_t head2;
    uint8_t car1_start;
    uint8_t car1_circle_count;
    float_byte_t car1_distance;
    float_byte_t   car1_encoder_route;
    uint8_t tail;
}car1_data_t;

typedef struct rece_pack
{
    uint8_t head1;
    uint8_t head2;
    uint8_t car2_start;
    uint8_t car2_circle_count;
    float_byte_t car2_distance;
    float_byte_t   car2_encoder_route;
    uint8_t tail;
}car1_data_r;

typedef union uart_data
{
    unsigned char Byte[13];
    car1_data_t car1_send_data;
}uart_car1_data_t;

typedef union uart_data2
{
    unsigned char Byte[13];
    car1_data_r car1_rece_data;
}uart_car1_data_r;

extern uart_car1_data_t uart_car1_send_data1;
extern uart_car1_data_r uart_car1_rece_data1;


void run_send(uint8_t car1_start,uint8_t car1_circle_count,float car1_distance,float car1_encoder_route);
uint8_t run_rece(uint8_t ByteData);
#endif //INC_2021EC_F_CAR_COMMUNICATE_H
