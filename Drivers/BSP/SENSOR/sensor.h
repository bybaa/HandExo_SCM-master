#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "./SYSTEM/sys/sys.h"
#include <stdio.h>
#include "math.h"


#define CSn1_H   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)    //PB3:CSN1										
#define CSn1_L   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define DO1      HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)                   //PB4:DO1
#define CLK1_H   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)    //PB5:CLK1
#define CLK1_L   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)  
		
#define PROG1_H  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)    //PB6:PROG1
#define PROG1_L  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)  //PB6:PROG1

/// ---------------------------------------------------------
/// Recreated by Anakin_Bao(a1282319520@163.com) on 2024/1/15
///	Add another two fingers
/// ---------------------------------------------------------
#define CSn2_H   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)    //PC2:CSN2										
#define CSn2_L   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET)
#define DO2      HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)                   //PC0:DO2
#define CLK2_H   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)    //PC3:CLK2
#define CLK2_L   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)

#define CSn3_H   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)    //PB12:CSN3										
#define CSn3_L   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define DO3      HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)                   //PC6:DO3
#define CLK3_H   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)    //PB13:CLK3
#define CLK3_L   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)


void send_bytes(void); 															// 发送单传感器数据字符串

void Sensor_GPIO(void);															// 初始化IO口
void GetDegree(void);																// 获得传感器实施角度
void GetDegreeo(void);															// 获得传感器初始角度
void read_change(void);

#endif

