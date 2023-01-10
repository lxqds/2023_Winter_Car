#ifndef __MPUIIC_H
#define __MPUIIC_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//MPU6050 IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 	   		   
//IO方向设置
#define MPU_SDA_IN()  {MAP_GPIO_setAsInputPin(GPIO_PORT_P6,GPIO_PIN4);}  // P7.0-7.7 output;}6.4置0
#define MPU_SDA_OUT() {MAP_GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN4);}	//p6.4置1

//#define MPU_SDA_IN()  {BITBAND_PERI(P6IN, 4);}  // P7.0-7.7 output;}6.4置0
//#define MPU_SDA_OUT() {BITBAND_PERI(P6OUT, 4);}	//p6.4置1
//IO操作函数	 
#define MPU_IIC_SCL    (BITBAND_PERI(P6OUT, 5)) 		//SCL
#define MPU_IIC_SDA    (BITBAND_PERI(P6OUT, 4)) 		//SDA	 
#define MPU_READ_SDA   (BITBAND_PERI(P6IN, 4)) 		//输入SDA 

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

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















