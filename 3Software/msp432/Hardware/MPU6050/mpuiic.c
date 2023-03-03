#include "mpuiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板V3
//MPU6050 IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
  //MPU IIC 延时函数
void MPU_IIC_Delay(void)
{
	delay_us(2);
}

//初始化IIC
void MPU_IIC_Init(void)
{					     
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN5);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN4);
	MPU6050_SCL_IO_INIT;
	MPU6050_SDA_IO_INIT;
}
//产生IIC起始信号
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //sda线输出
	MPU_SCL_Set();	  	  
	MPU_SDA_Set();
	MPU_IIC_Delay();
	MPU_IIC_Delay();
 	MPU_SDA_Clr();//START:when CLK is high,DATA change form high to low 
	MPU_IIC_Delay();
	MPU_IIC_Delay();
	MPU_SCL_Clr();//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda线输出
	MPU_SCL_Clr();
	MPU_SDA_Clr();//STOP:when CLK is high DATA change form low to high
 	MPU_IIC_Delay();
	MPU_IIC_Delay();
	MPU_SCL_Set();
	MPU_SDA_Set();//发送I2C总线结束信号
	MPU_IIC_Delay();
	MPU_IIC_Delay();	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();      //SDA设置为输入  
	MPU_SDA_Set();;MPU_IIC_Delay();	MPU_IIC_Delay();MPU_IIC_Delay();MPU_IIC_Delay();MPU_IIC_Delay();   
	MPU_SCL_Set();MPU_IIC_Delay();MPU_IIC_Delay();MPU_IIC_Delay();MPU_IIC_Delay();MPU_IIC_Delay();	 
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_SCL_Clr();//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void MPU_IIC_Ack(void)
{
	MPU_SCL_Clr();
	MPU_SDA_OUT();
	MPU_SDA_Clr();
	MPU_IIC_Delay();
	MPU_SCL_Set();
	MPU_IIC_Delay();
	MPU_SCL_Clr();
}
//不产生ACK应答		    
void MPU_IIC_NAck(void)
{
	MPU_SCL_Clr();
	MPU_SDA_OUT();
	MPU_SDA_Set();
	MPU_IIC_Delay();
	MPU_SCL_Set();
	MPU_IIC_Delay();
	MPU_SCL_Clr();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MPU_SDA_OUT(); 	    
    MPU_SCL_Clr();//拉低时钟开始数据传输
    MPU_IIC_Delay();
	for(t=0;t<8;t++)
    {              
        MPU_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    MPU_SCL_Set();
		    MPU_IIC_Delay();
			MPU_IIC_Delay();
		    MPU_SCL_Clr();	
		    MPU_IIC_Delay();
    }
	MPU_IIC_Delay();	
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        MPU_SCL_Clr(); 
        MPU_IIC_Delay();
		MPU_SCL_Set();
        receive<<=1;
        if(MPU_READ_SDA)receive++;   
		MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();//发送nACK
    else
        MPU_IIC_Ack(); //发送ACK   
    return receive;
}

























