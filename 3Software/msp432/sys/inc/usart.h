/*
 * @Description: 
 * @Author: Liang xiaoqi
 * @Date: 2022-07-25 14:47:54
 * @LastEditTime: 2022-07-26 11:50:33
 * @LastEditors: Liang xiaoqi
 */

#ifndef __USART_H
#define __USART_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "stdio.h" //1.61328125kb

#ifdef __TI_COMPILER_VERSION__
//CCS平台
#include "stdarg.h"
#include "string.h"
#define USART0_MAX_SEND_LEN     600                 //最大发送缓存字节数
int printf(const char *str, ...);
#endif



void uart_init(uint32_t baudRate);
void USART_SendCharArr(uint8_t *char_arr ,uint16_t length);
void AnoTc_SendUserTest(uint16_t A, uint16_t B, uint32_t C);

#endif
