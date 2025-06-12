/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
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
 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include "./BSP/LCD/lcd.h"
#include <lwip/sockets.h>
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip_demo.h"
#include "BSP/SENSOR/sensor.h"
#include "BSP/AD7616/AD7616.h"

/* 这个必须填写正确，远程IP地址 */
#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2                 1
#define DEST_IP_ADDR3               10

#define LWIP_DEMO_RX_BUFSIZE         200   /* 定义最大接收数据长度 */
#define LWIP_DEMO_PORT               1111  /* 定义连接的本地端口号 */

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* 发送数据内容 */
char *g_lwip_demo_sendbuf = "ALIENT89EK DATA\r\n";
/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
extern QueueHandle_t g_display_queue;   /* 显示消息队列句柄 */


/**
 * @brief       lwip_demo实验入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
	extern float angle[6];
	extern float angle2[6];
	extern float angle3[6];	
	extern float angle_udp[18];
	extern float ad7616f_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM];
	
	float udp_data[34];
	
	int i = 0;
    err_t err;
	
    static struct netconn *udpconn;
    static struct netbuf  *sentbuf;
    ip_addr_t destipaddr;

    
    /* 第一步：创建udp控制块 */
    udpconn = netconn_new(NETCONN_UDP);
    /* 定义接收超时时间 */
    udpconn->recv_timeout = 10;

    if (udpconn != NULL)                                        /* 判断创建控制块释放成功 */
    {
        /* 第二步：绑定控制块、本地IP和端口 */
        err = netconn_bind(udpconn, IP_ADDR_ANY, LWIP_DEMO_PORT);
        /* 构造目的IP地址 */
        IP4_ADDR(&destipaddr, DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);
        /* 第三步：连接或者建立对话框 */
        netconn_connect(udpconn, &destipaddr, LWIP_DEMO_PORT);  /* 连接到远端主机 */
		
		GetDegreeo();
		
        if (err == ERR_OK)                                      /* 绑定完成 */
        {
            while (1)
            {
				for(i = 0;i<34;++i)
				{
					if(i < 18) 
					{
						udp_data[i] = angle_udp[i];
						
						printf("%d: %.2f ",i,angle_udp[i]);
					}
					else 
					{
						udp_data[i] = ad7616f_data[i-18];
					}
				}
				
				printf("\n");
//				for (i = 0 ; i <16 ;++i)
//				{
//					udp_data[i] = ad7616f_data[i];
//				}
				
				if (1)
                {
                    sentbuf = netbuf_new();
                    netbuf_alloc(sentbuf, sizeof(udp_data));
                    memcpy(sentbuf->p->payload, (void *)udp_data, sizeof(udp_data));
                    err = netconn_send(udpconn, sentbuf);               /* 将netbuf中的数据发送出去 */

                    if (err != ERR_OK)
                    {
                        printf("发送失败\r\n");
                        netbuf_delete(sentbuf);                         /* 删除buf */
                    }

                    g_lwip_send_flag &= ~LWIP_SEND_DATA;                  /* 清除数据发送标志 */
                    netbuf_delete(sentbuf);                             /* 删除buf */
                }
				
                vTaskDelay(1);
            }
        }
        else printf("UDP绑定失败\r\n");
    }
    else printf("UDP连接创建失败\r\n");
}
