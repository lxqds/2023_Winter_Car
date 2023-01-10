#ifndef __MPUIIC_H
#define __MPUIIC_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//MPU6050 IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 	   		   
//IO��������
#define MPU_SDA_IN()  {P6->DIR &= 0xEF;}  // P7.0-7.7 output;}6.4��0
#define MPU_SDA_OUT() {P6->DIR |= 0x10;;}	//p6.4��1

//#define MPU_SDA_IN()  {BITBAND_PERI(P6IN, 4);}  // P7.0-7.7 output;}6.4��0
//#define MPU_SDA_OUT() {BITBAND_PERI(P6OUT, 4);}	//p6.4��1
//IO��������	 
#define MPU_IIC_SCL    (BITBAND_PERI(P6OUT, 5)) 		//SCL
#define MPU_IIC_SDA    (BITBAND_PERI(P6OUT, 4)) 		//SDA	 
#define MPU_READ_SDA   (BITBAND_PERI(P6IN, 4)) 		//����SDA 

//MPU6050 I2C SCL P6.5
#define MPU6050_SCL_PIN_NUM (5)
#define MPU6050_SCL_IO_INIT (BITBAND_PERI(P6DIR, MPU6050_SCL_PIN_NUM) = 1) 
#define MPU_SCL_Set() (BITBAND_PERI(P6OUT, MPU6050_SCL_PIN_NUM) = 1)
#define MPU_SCL_Clr() (BITBAND_PERI(P6OUT, MPU6050_SCL_PIN_NUM) = 0)

//MPU6050 I2C SDA   P6.4
#define MPU6050_SDA_PIN_NUM (4)
#define MPU6050_SDA_IO_INIT (BITBAND_PERI(P6DIR, MPU6050_SDA_PIN_NUM) = 1)
#define MPU_SDA_Set() (BITBAND_PERI(P6OUT, MPU6050_SDA_PIN_NUM) = 1)
#define MPU_SDA_Clr() (BITBAND_PERI(P6OUT, MPU6050_SDA_PIN_NUM) = 0)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















