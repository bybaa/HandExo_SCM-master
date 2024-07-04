/**
 ****************************************************************************************************
 * @file        freertos_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
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

/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO         5           /* �������ȼ� */
#define START_STK_SIZE          128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;             /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* LWIP_DEMO ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LWIP_DMEO_TASK_PRIO     11          /* �������ȼ� */
#define LWIP_DMEO_STK_SIZE      1024        /* �����ջ��С */
TaskHandle_t LWIP_Task_Handler;             /* ������ */
void lwip_demo_task(void *pvParameters);    /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO           10          /* �������ȼ� */
#define LED_STK_SIZE            128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;               /* ������ */
void led_task(void *pvParameters);          /* ������ */

/* KEY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define KEY_TASK_PRIO           11          /* �������ȼ� */
#define KEY_STK_SIZE            128         /* �����ջ��С */
TaskHandle_t KEYTask_Handler;               /* ������ */
void key_task(void *pvParameters);          /* ������ */

/* DISPLAY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define DISPLAY_TASK_PRIO       12          /* �������ȼ� */
#define DISPLAY_STK_SIZE        512         /* �����ջ��С */
TaskHandle_t DISPLAYTask_Handler;           /* ������ */
void display_task(void *pvParameters);      /* ������ */

/* ��ʾ��Ϣ���е����� */
#define DISPLAYMSG_Q_NUM    20   /* ��ʾ��Ϣ���е����� */
QueueHandle_t g_display_queue;     /* ��ʾ��Ϣ���о�� */

/******************************************************************************************************/


/**
 * @breif       ����UI
 * @param       mode :  bit0:0,������;1,����ǰ�벿��UI
 *                      bit1:0,������;1,���غ�벿��UI
 * @retval      ��
 */
void lwip_test_ui(uint8_t mode)
{
    uint8_t speed;
    uint8_t buf[30];
    
    if (mode & 1<< 0)
    {
        lcd_show_string(6, 10, 200, 32, 32, "STM32F407", RED);
        lcd_show_string(6, 40, lcddev.width, 24	, 24, "Opening HandExo...", DARKBLUE);
        lcd_show_string(6, 70, 200, 16, 16, "ATOM@ALIENTEK", DARKBLUE);
		lcd_show_string(6, 90, 200, 16, 16, "BaoYangbin 2024/5/4", BLACK);
		
		lcd_show_string(5, 680, 400, 50, 16, "Author: BaoYangbin(Yangbin.Bao@zju.edu.cn)",MAGENTA);
		lcd_show_string(5, 700, 300, 16, 16, "Date: 2024-05-24",MAGENTA);
		lcd_show_string(5, 720, 300, 40, 16, "Project: HandExo for telerobotics",MAGENTA);
		lcd_show_string(5, 740, 400, 40, 16, "Thanks to Su Yuan and Gaofeng Li",MAGENTA);
		lcd_show_string(5, 760, 400, 40, 16, "for their contributions to this project!",MAGENTA);
    }
    
    if (mode & 1 << 1)
    {
        lcd_show_string(5, 110, 200, 16, 16, "lwIP Init Successed", MAGENTA);
        
        if (g_lwipdev.dhcpstatus == 2)
        {
            sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);     /* ��ʾ��̬IP��ַ */
        
			lcd_show_string(5, 130, 200, 16, 16, (char*)buf, MAGENTA);
			
			sprintf((char*)buf,"DHCP GW:%d.%d.%d.%d",g_lwipdev.gateway[0],g_lwipdev.gateway[1],g_lwipdev.gateway[2],g_lwipdev.gateway[3]);
		
		    lcd_show_string(5, 150, 200, 16, 16, (char*)buf, MAGENTA);
			
			sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",g_lwipdev.netmask[0],g_lwipdev.netmask[1],g_lwipdev.netmask[2],g_lwipdev.netmask[3]);	//��ӡ���������ַ
			
			lcd_show_string(5, 170, 210, 16, 16, (char*)buf, MAGENTA);
		}
        else
        {
            sprintf((char*)buf,"Static IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);    /* ��ӡ��̬IP��ַ */
                
			lcd_show_string(5, 130, 200, 16, 16, (char*)buf, MAGENTA);
			
			sprintf((char*)buf,"Static GW:%d.%d.%d.%d",g_lwipdev.gateway[0],g_lwipdev.gateway[1],g_lwipdev.gateway[2],g_lwipdev.gateway[3]);
		
		    lcd_show_string(5, 150, 200, 16, 16, (char*)buf, MAGENTA);
			
			sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",g_lwipdev.netmask[0],g_lwipdev.netmask[1],g_lwipdev.netmask[2],g_lwipdev.netmask[3]);	//��ӡ���������ַ
			
			lcd_show_string(5, 170, 210, 16, 16, (char*)buf, MAGENTA);
		}
        
		sprintf((char*)buf,"Remote Ip:%d.%d.%d.%d",g_lwipdev.remoteip[0],g_lwipdev.remoteip[1],g_lwipdev.remoteip[2],g_lwipdev.remoteip[3]);
        lcd_show_string(5, 190, 210, 16, 16, (char*)buf, MAGENTA);
		
		speed = ethernet_chip_get_speed();      /* �õ����� */
  
        if (speed)
        {
            lcd_show_string(5, 240, 200, 16, 16, "Ethernet Speed:100M", MAGENTA);
        }
        else
        {
            lcd_show_string(5, 240, 200, 16, 16, "Ethernet Speed:10M", MAGENTA);
        }
        
        lcd_show_string(5, 260, 200, 16, 16, "KEY0:Send data", MAGENTA);
        lcd_show_string(5, 280, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", BLUE); /* ��ʾ��Ϣ */
		
    }
}

/**
 * @breif       freertos_demo
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{
    /* start_task���� */
    xTaskCreate((TaskFunction_t )start_task,
                (const char *   )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t * )&StartTask_Handler);

    vTaskStartScheduler(); /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    g_lwipdev.lwip_display_fn = lwip_test_ui;
    
    lwip_test_ui(1);    /* ���غ�ǰ����UI */
    
    while (lwip_comm_init() != 0)
    {
        lcd_show_string(30, 110, 200, 16, 16, "lwIP Init failed!!", RED);
        delay_ms(500);
        lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
        lcd_show_string(30, 110, 200, 16, 16, "Retrying...       ", RED);
        delay_ms(500);
        LED1_TOGGLE();
    }
    
    while (!ethernet_read_phy(PHY_SR))  /* ���MCU��PHYоƬ�Ƿ�ͨ�ųɹ� */
    {
        printf("MCU��PHYоƬͨ��ʧ�ܣ������·����Դ�룡������\r\n");
    }
    
    while ((g_lwipdev.dhcpstatus != 2)&&(g_lwipdev.dhcpstatus != 0XFF))  /* �ȴ�DHCP��ȡ�ɹ�/��ʱ��� */
    {
        vTaskDelay(5);
    }
    

    
    taskENTER_CRITICAL();           /* �����ٽ��� */
    
    g_display_queue = xQueueCreate(DISPLAYMSG_Q_NUM,200);      /* ������ϢMessage_Queue,���������200���� */
    
    /* ����lwIP���� */
    xTaskCreate((TaskFunction_t )lwip_demo_task,
                (const char*    )"lwip_demo_task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* key���� */
    xTaskCreate((TaskFunction_t )key_task,
                (const char *   )"key_task",
                (uint16_t       )KEY_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t * )&KEYTask_Handler);

    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    /* ��ʾ���� */
    xTaskCreate((TaskFunction_t )display_task,
                (const char*    )"display_task",
                (uint16_t       )DISPLAY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )DISPLAY_TASK_PRIO,
                (TaskHandle_t*  )&DISPLAYTask_Handler);

    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    
}

/**
 * @brief       lwIP��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;

    lwip_demo();            /* lwip���Դ��� */
    
    while (1)
    {
        vTaskDelay(5);
    }
}

/**
 * @brief       key_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void key_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    uint8_t key;
  
    while (1)
    {
        key = key_scan(0);

        if (KEY0_PRES ==key)
        {
            g_lwip_send_flag |= LWIP_SEND_DATA; /* ���LWIP������Ҫ���� */
        }
        
		if (KEY1_PRES == key)
		{
			GetDegreeo();
			lcd_show_string(5,400,200,16,16,"has been reset", RED);
			vTaskDelay(100);
			lcd_fill(5, 400, 300, 420, WHITE);
		}
        vTaskDelay(10);
    }
}

/**
 * @brief       ϵͳ������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while (1)
    {
        LED1_TOGGLE();
        vTaskDelay(100);
    }
}

/**
 * @brief       ��ʾ����
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void display_task(void *pvParameters)
{
    pvParameters = pvParameters;
    uint8_t *buffer;
    
    while (1)
    {
        buffer = mymalloc(SRAMIN,200);
        
        if (g_display_queue != NULL)
        {
            memset(buffer,0,200);       /* ��������� */
            
            if (xQueueReceive(g_display_queue,buffer,portMAX_DELAY))
            {
                lcd_fill(30, 300, 300, 320, WHITE); /* ����һ������ */
                /* ��ʾ���յ������� */
                lcd_show_string(30, 300, lcddev.width - 30, lcddev.height - 230, 16, (char *)buffer, RED); 
            }
        }
        
        myfree(SRAMIN,buffer);          /*�ͷ��ڴ� */
        
        vTaskDelay(5);
    }
}
