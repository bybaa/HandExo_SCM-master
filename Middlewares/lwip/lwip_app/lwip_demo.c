/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP Netconn UDP ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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

/* ���������д��ȷ��Զ��IP��ַ */
#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2                 2
#define DEST_IP_ADDR3               111

#define LWIP_DEMO_RX_BUFSIZE         200   /* �������������ݳ��� */
#define LWIP_DEMO_PORT               8080  /* �������ӵı��ض˿ں� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* ������������ */
char *g_lwip_demo_sendbuf = "ALIENT89EK DATA\r\n";
/* ���ݷ��ͱ�־λ */
uint8_t g_lwip_send_flag;
extern QueueHandle_t g_display_queue;   /* ��ʾ��Ϣ���о�� */
/**
 * @brief       lwip_demoʵ�����
 * @param       ��
 * @retval      ��
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
    
    /* ��һ��������udp���ƿ� */
    udpconn = netconn_new(NETCONN_UDP);
    /* ������ճ�ʱʱ�� */
    udpconn->recv_timeout = 10;

    if (udpconn != NULL)                                        /* �жϴ������ƿ��ͷųɹ� */
    {
        /* �ڶ������󶨿��ƿ顢����IP�Ͷ˿� */
        err = netconn_bind(udpconn, IP_ADDR_ANY, LWIP_DEMO_PORT);
        /* ����Ŀ��IP��ַ */
        IP4_ADDR(&destipaddr, DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);
        /* �����������ӻ��߽����Ի��� */
        netconn_connect(udpconn, &destipaddr, LWIP_DEMO_PORT);  /* ���ӵ�Զ������ */
		
		GetDegreeo();
		
        if (err == ERR_OK)                                      /* ����� */
        {
            while (1)
            {
                /* ���Ĳ������ָ���İ�������ʱ���ᷢ����Ϣ */
                if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA)
                {
                    sentbuf = netbuf_new();
                    netbuf_alloc(sentbuf, sizeof(angle_udp));
                    memcpy(sentbuf->p->payload, (void *)angle_udp, sizeof(angle_udp));
                    err = netconn_send(udpconn, sentbuf);               /* ��netbuf�е����ݷ��ͳ�ȥ */

                    if (err != ERR_OK)
                    {
                        printf("����ʧ��\r\n");
                        netbuf_delete(sentbuf);                         /* ɾ��buf */
                    }

                    g_lwip_send_flag &= ~LWIP_SEND_DATA;                  /* ������ݷ��ͱ�־ */
                    netbuf_delete(sentbuf);                             /* ɾ��buf */
                }
                
				                /* ���岽���������� */
                netconn_recv(udpconn, &recvbuf);

                if (recvbuf != NULL)                                    /* ���յ����� */
                {
                    memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE); /* ���ݽ��ջ��������� */

                    for (q = recvbuf->p; q != NULL; q = q->next)        /* ����������pbuf���� */
                    {
                        /* �ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ䣬������� */
                        /* �Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
                        if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* �������� */
                        else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

                        data_len += q->len;

                        if (data_len > LWIP_DEMO_RX_BUFSIZE) break;     /* ����TCP�ͻ��˽�������,���� */
                    }

                    data_len = 0;                                       /* ������ɺ�data_lenҪ���� */

                    lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
                    
                    if (lwip_err == errQUEUE_FULL)
                    {
                        printf("����Key_Queue���������ݷ���ʧ��!\r\n");
                    }
                    
                    netbuf_delete(recvbuf);                             /* ɾ��buf */
                }   
                
                else vTaskDelay(5);                                     /* ��ʱ5ms */
				
                vTaskDelay(10);
            }
        }
        else printf("UDP��ʧ��\r\n");
    }
    else printf("UDP���Ӵ���ʧ��\r\n");
}
