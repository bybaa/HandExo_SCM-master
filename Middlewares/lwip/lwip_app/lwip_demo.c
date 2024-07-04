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

/* 这个必须填写正确，远程IP地址 */
#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2                 2
#define DEST_IP_ADDR3               111

#define LWIP_DEMO_RX_BUFSIZE         200   /* 定义最大接收数据长度 */
#define LWIP_DEMO_PORT               8080  /* 定义连接的本地端口号 */

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
	
    err_t err;
    static struct netconn *udpconn;
    static struct netbuf  *recvbuf;
    static struct netbuf  *sentbuf;
    ip_addr_t destipaddr;
    uint32_t data_len = 0;
    struct pbuf *q;
    BaseType_t lwip_err;
    
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
                /* 第四步：如果指定的按键按下时，会发送信息 */
                if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA)
                {
                    sentbuf = netbuf_new();
                    netbuf_alloc(sentbuf, sizeof(angle_udp));
                    memcpy(sentbuf->p->payload, (void *)angle_udp, sizeof(angle_udp));
                    err = netconn_send(udpconn, sentbuf);               /* 将netbuf中的数据发送出去 */

                    if (err != ERR_OK)
                    {
                        printf("发送失败\r\n");
                        netbuf_delete(sentbuf);                         /* 删除buf */
                    }

                    g_lwip_send_flag &= ~LWIP_SEND_DATA;                  /* 清除数据发送标志 */
                    netbuf_delete(sentbuf);                             /* 删除buf */
                }
                
				                /* 第五步：接收数据 */
                netconn_recv(udpconn, &recvbuf);

                if (recvbuf != NULL)                                    /* 接收到数据 */
                {
                    memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE); /* 数据接收缓冲区清零 */

                    for (q = recvbuf->p; q != NULL; q = q->next)        /* 遍历完整个pbuf链表 */
                    {
                        /* 判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于 */
                        /* 的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据 */
                        if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* 拷贝数据 */
                        else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

                        data_len += q->len;

                        if (data_len > LWIP_DEMO_RX_BUFSIZE) break;     /* 超出TCP客户端接收数组,跳出 */
                    }

                    data_len = 0;                                       /* 复制完成后data_len要清零 */

                    lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
                    
                    if (lwip_err == errQUEUE_FULL)
                    {
                        printf("队列Key_Queue已满，数据发送失败!\r\n");
                    }
                    
                    netbuf_delete(recvbuf);                             /* 删除buf */
                }   
                
                else vTaskDelay(5);                                     /* 延时5ms */
				
                vTaskDelay(10);
            }
        }
        else printf("UDP绑定失败\r\n");
    }
    else printf("UDP连接创建失败\r\n");
}
