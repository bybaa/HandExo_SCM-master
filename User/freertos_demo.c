/**
 ****************************************************************************************************
 * @file        freertos_demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       lwIP Netconn UDP 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "freertos_demo.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./MALLOC/malloc.h"
#include "./BSP/KEY/key.h"
#include "lwip_comm.h"
#include "lwip_demo.h"
#include "lwipopts.h"
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "BSP/SENSOR/sensor.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

//#include "./BSP/LCD/lcd.h"
#include "./BSP/AD7616/AD7616.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         5           /* 任务优先级 */
#define START_STK_SIZE          128         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;             /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* LWIP_DEMO 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LWIP_DMEO_TASK_PRIO     11          /* 任务优先级 */
#define LWIP_DMEO_STK_SIZE      1024        /* 任务堆栈大小 */
TaskHandle_t LWIP_Task_Handler;             /* 任务句柄 */
void lwip_demo_task(void *pvParameters);    /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO           10          /* 任务优先级 */
#define LED_STK_SIZE            128         /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;               /* 任务句柄 */
void led_task(void *pvParameters);          /* 任务函数 */

/* KEY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define KEY_TASK_PRIO           11          /* 任务优先级 */
#define KEY_STK_SIZE            128         /* 任务堆栈大小 */
TaskHandle_t KEYTask_Handler;               /* 任务句柄 */
void key_task(void *pvParameters);          /* 任务函数 */


/* DISPLAY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define ADC_TASK_PRIO       13          /* 任务优先级 */
#define ADC_STK_SIZE        128         /* 任务堆栈大小 */  
TaskHandle_t ADCTask_Handler;           /* 任务句柄 */
void adc_task(void *pvParameters);      /* 任务函数 */


/* 显示消息队列的数量 */
#define DISPLAYMSG_Q_NUM    20   /* 显示消息队列的数量 */
QueueHandle_t g_display_queue;     /* 显示消息队列句柄 */

#define LWIP_DEMO_PORT 8089

//#define ADC_DMA_BUF_SIZE        50 * 2      /* ADC DMA采集 BUF大小, 应等于ADC通道数的整数倍 */
//uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */

//extern uint8_t g_adc_dma_sta;               /* DMA传输状态标志, 0, 未完成; 1, 已完成 */
//float fBuffer[ADC_CH_NUM];



/**
 * @breif       加载UI
 * @param       mode :  bit0:0,不加载;1,加载前半部分UI
 *                      bit1:0,不加载;1,加载后半部分UI
 * @retval      无
 */
void lwip_test_ui(uint8_t mode)
{
    uint8_t speed;
    
    if (mode & 1<< 0)
    {
		printf("STM32F407.\n");
		printf("Opening HandExo...\n");
    }
    
    if (mode & 1 << 1)
    {
		printf("lwIP Init Successed.\n");
        
        if (g_lwipdev.dhcpstatus == 2)
        {

			printf("DHCP IP:%d.%d.%d.%d\n",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);
			
			printf("DHCP GW:%d.%d.%d.%d\n",g_lwipdev.gateway[0],g_lwipdev.gateway[1],g_lwipdev.gateway[2],g_lwipdev.gateway[3]);
			
			printf("NET MASK:%d.%d.%d.%d\n",g_lwipdev.netmask[0],g_lwipdev.netmask[1],g_lwipdev.netmask[2],g_lwipdev.netmask[3]);	//打印子网掩码地址
			
		}
        else
        {
            printf("Static IP:%d.%d.%d.%d\n",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);    /* 打印静态IP地址 */
               
			printf("Static GW:%d.%d.%d.%d\n",g_lwipdev.gateway[0],g_lwipdev.gateway[1],g_lwipdev.gateway[2],g_lwipdev.gateway[3]);
			
			printf("NET MASK:%d.%d.%d.%d\n",g_lwipdev.netmask[0],g_lwipdev.netmask[1],g_lwipdev.netmask[2],g_lwipdev.netmask[3]);	//打印子网掩码地址
			
			
		}
        
		printf("Remote Ip:%d.%d.%d.%d\n",g_lwipdev.remoteip[0],g_lwipdev.remoteip[1],g_lwipdev.remoteip[2],g_lwipdev.remoteip[3]);
		printf("Port: %d\n",LWIP_DEMO_PORT );
		speed = ethernet_chip_get_speed();      /* 得到网速 */
  
        if (speed)
        {
			printf("Ethernet Speed:100M\n");
        }
        else
        {
            printf("Ethernet Speed:10M\n");
        }
	
    }
}

/**
 * @breif       freertos_demo
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
    /* start_task任务 */
    xTaskCreate((TaskFunction_t )start_task,
                (const char *   )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t * )&StartTask_Handler);

    vTaskStartScheduler(); /* 开启任务调度 */
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    g_lwipdev.lwip_display_fn = lwip_test_ui;
    
    lwip_test_ui(1);    /* 加载后前部分UI */
    
    while (lwip_comm_init() != 0)
    {
		printf("lwIP Init failed!!\n");
        delay_ms(500);
		printf("Retrying...\n");
        delay_ms(500);
        LED1_TOGGLE();
    }
    
    while (!ethernet_read_phy(PHY_SR))  /* 检查MCU与PHY芯片是否通信成功 */
    {
        printf("MCU与PHY芯片通信失败，请检查电路或者源码！！！！\r\n");
    }
    
    while ((g_lwipdev.dhcpstatus != 2)&&(g_lwipdev.dhcpstatus != 0XFF))  /* 等待DHCP获取成功/超时溢出 */
    {
        vTaskDelay(5);
    }
    

    
    taskENTER_CRITICAL();           /* 进入临界区 */
    
    g_display_queue = xQueueCreate(DISPLAYMSG_Q_NUM,200);      /* 创建消息Message_Queue,队列项长度是200长度 */
    
    /* 创建lwIP任务 */
    xTaskCreate((TaskFunction_t )lwip_demo_task,
                (const char*    )"lwip_demo_task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* key任务 */
    xTaskCreate((TaskFunction_t )key_task,
                (const char *   )"key_task",
                (uint16_t       )KEY_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t * )&KEYTask_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

	xTaskCreate((TaskFunction_t )adc_task,
			(const char*    )"adc_task",
			(uint16_t       )ADC_STK_SIZE,
			(void*          )NULL,
			(UBaseType_t    )ADC_TASK_PRIO,
			(TaskHandle_t*  )&ADCTask_Handler);
				
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
    
}

/**
 * @brief       lwIP运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;

    lwip_demo();            /* lwip测试代码 */
    
    while (1)
    {
        vTaskDelay(5);
    }
}

/**
 * @brief       key_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void key_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    uint8_t key;
  
    while (1)
    {
        key = key_scan(0);
        
		if (KEY1_PRES == key)
		{
			GetDegreeo();
			printf("Encoder has been reset!\n");
			vTaskDelay(100);
		}
        vTaskDelay(10);
    }
}

/**
 * @brief       系统再运行
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
	extern float angle[6];
    pvParameters = pvParameters;

    while (1)
    {
        LED1_TOGGLE();
		GetDegree();
		//printf("angle: %.2f\n",angle[0]);
        vTaskDelay(100);
    }
}

void adc_task(void *pvParameters)
{
	extern uint16_t ad7616_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM] ;
	extern float ad7616f_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM];
    pvParameters = pvParameters;
	
#if USE_SOFTWARE_MODE
	AD7616_Init(Software_Mode);
#if USE_PARALLEL_MODE
#else
	AD7616_Set_Serial_Output_Format(Serial_Line_2_Output);
#endif
	AD7616_Reset();
	AD7616_Set_Range(Range_10_V);
	AD7616_Channel_Group_Select(Channel_Group_0);
#else
	AD7616_Init(Hardware_Mode);
#if USE_PARALLEL_MODE
#else
	AD7616_Set_Serial_Output_Format(Serial_Line_2_Output);
#endif
	AD7616_Set_Range(Range_10_V);
	AD7616_Channel_Group_Select(Channel_Group_0);
	AD7616_Reset();
#endif

  while (1) {
	  
    /* 芯片手册31页图51可知。切换通道后，数据读取的是上一次CONVST的值。 */
    /* 如切换到通道1，得到的数值是通道8的值 */
    for (uint8_t channel_group = 0; channel_group < AD7616_CHANNEL_GROUP_MAX;
         channel_group++) {
      AD7616_Channel_Group_Select(channel_group);
      AD7616_Conversion();
      if (!channel_group) {
        AD7616_Read_Data(&ad7616_data[Channel_Group_7 * 2]);
      } else {
        AD7616_Read_Data(&ad7616_data[(channel_group - 1) * 2]);
      }
    }
    for (uint8_t i = 0; i < AD7616_CHANNEL_GROUP_MAX; i++) {
//      printf("A%d: %.1f mv 0x%x %ld B%d: %.1f mv 0x%x %ld\r\n", i,
//             AD7616_Digital2Voltage(ad7616_data[i * 2]), ad7616_data[i * 2],
//             ad7616_data[i * 2], i,
//             AD7616_Digital2Voltage(ad7616_data[i * 2 + 1]),
//             ad7616_data[i * 2 + 1], ad7616_data[i * 2 + 1]);
		
		ad7616f_data[i*2] = AD7616_Digital2Voltage(ad7616_data[i*2]);
		ad7616f_data[i*2+1] = AD7616_Digital2Voltage(ad7616_data[i*2+1]);
		
		// printf("A%d: %.1f B%d: %.1f\n",i,ad7616f_data[i*2], i, ad7616f_data[i*2+1]);
    }
	
    vTaskDelay(50);
  }


}
   
