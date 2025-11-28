/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP SOCKET UDP 实验
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
 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdint.h>
#include <stdio.h>
#include <lwip/sockets.h>
#include "./BSP/LCD/lcd.h"
#include "./MALLOC/malloc.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip_demo.h"
#include "BSP/SENSOR/sensor.h"
#include "BSP/AD7616/AD7616.h"

/* 需要自己设置远程IP地址 */
#define IP_ADDR   "192.168.1.10"

#define LWIP_DEMO_PORT               1111   /* 连接的本地端口号 */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 3 ) /* 发送数据线程优先级 */

/* 发送数据内容 */
char g_lwip_demo_sendbuf[] = "ALIENTEK DATA \r\n";
/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
struct sockaddr_in g_local_info;              /* 定义Socket地址信息结构体 */
socklen_t g_sock_fd;                          /* 定义一个Socket接口 */
static void lwip_send_thread(void *arg);

extern QueueHandle_t g_display_queue;         /* 显示消息队列句柄 */

/**
 * @brief       发送数据线程
 * @param       无
 * @retval      无
 */
void lwip_data_send(void)
{
    sys_thread_new("lwip_send_thread", lwip_send_thread, NULL, 512, LWIP_SEND_THREAD_PRIO );
}

/**
 * @brief       lwip_demo实验入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    BaseType_t lwip_err;
	
	float fbuf[3];
    char *tbuf;
    lwip_data_send();                                   /* 创建发送数据线程 */
    memset(&g_local_info, 0, sizeof(struct sockaddr_in)); /* 将服务器地址清空 */
    g_local_info.sin_len = sizeof(g_local_info);
    g_local_info.sin_family = AF_INET;                    /* IPv4地址 */
    g_local_info.sin_port = htons(LWIP_DEMO_PORT);        /* 设置端口号 */
    g_local_info.sin_addr.s_addr = htons(INADDR_ANY);     /* 设置本地IP地址 */

    g_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);           /* 建立一个新的socket连接 */
    
    tbuf = malloc(200); /* 申请内存 */
    sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* 客户端端口号 */
    lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
    
    /* 建立绑定 */
    bind(g_sock_fd, (struct sockaddr *)&g_local_info, sizeof(struct sockaddr_in));

    while (1)
    {	
		memset(fbuf, 0, sizeof(fbuf)); 
		recv(g_sock_fd, (void *)fbuf, sizeof(fbuf), 0);
		
		lwip_err = xQueueSend(g_display_queue,&fbuf,0);
		
		// printf("data: %.3f\n", fbuf[0]);
        
        if (lwip_err == errQUEUE_FULL)
        {
            printf("队列Key_Queue已满，数据发送失败!\r\n");
        }
		
		vTaskDelay(1);
    }
}

/**
 * @brief       发送数据线程函数
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_send_thread(void *pvParameters)
{
    pvParameters = pvParameters;
    extern float angle[6];
	extern float angle2[6];
	extern float angle3[6];
	extern float angle_udp[18];
	extern float ad7616f_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM];
	
	float udp_data[34];
	int i ;
    g_local_info.sin_addr.s_addr = inet_addr(IP_ADDR);                /* 需要发送的远程IP地址 */
	
	
	GetDegreeo();
    while (1)
    {	
		for (i=0; i<34; ++i) 
		{
			if (i < 18)
			{
				udp_data[i] = angle_udp[i];
//				printf("%.2f , ", udp_data[i]);
			}
			else 
			{
				udp_data[i] = ad7616f_data[i-18];
			}
		}
//		printf("\n");
		
        if (1)     /* 有数据要发送 */
        {
            sendto(g_sock_fd,                                         /* scoket */
                  (float *)udp_data,                        /* 发送的数据 */
                  sizeof(udp_data), 0,                     /* 发送的数据大小 */
                  (struct sockaddr *)&g_local_info,                   /* 接收端地址信息 */ 
                  sizeof(g_local_info));                              /* 接收端地址信息大小 */

            g_lwip_send_flag &= ~LWIP_SEND_DATA;
        }
        
        vTaskDelay(1);
   }
}
