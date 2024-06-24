#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "usmart.h"
#include "timer.h"
#include "lcd.h"
#include "sram.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "includes.h"
#include "lwipopts.h"
#include "udp_demo.h"
#include "sensor.h"



//KEY����
#define KEY_TASK_PRIO 		8
//�����ջ��С
#define KEY_STK_SIZE		128	
//�����ջ
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
void key_task(void *pdata);   

//LED����
//�������ȼ�
#define LED_TASK_PRIO		9
//�����ջ��С
#define LED_STK_SIZE		64
//�����ջ
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);  

//��LCD����ʾ��ַ��Ϣ����
//�������ȼ�
#define DISPLAY_TASK_PRIO	10
//�����ջ��С
#define DISPLAY_STK_SIZE	128
//�����ջ
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//������
void display_task(void *pdata);

//START����
//�������ȼ�
#define START_TASK_PRIO		11
//�����ջ��С
#define START_STK_SIZE		128
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata); 



//��LCD����ʾ��ַ��Ϣ
//mode:1 ��ʾDHCP��ȡ���ĵ�ַ
//	  ���� ��ʾ��̬��ַ
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
		LCD_ShowString(10,105,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
		LCD_ShowString(10,120,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
		LCD_ShowString(10,135,210,16,16,buf); 
		LCD_ShowString(10,150,210,16,16,"Port:8089!"); 
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
		LCD_ShowString(10,105,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
		LCD_ShowString(10,120,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
		LCD_ShowString(10,135,210,16,16,buf); 
		LCD_ShowString(10,150,210,16,16,"Port:8089!"); 
	}	
}
	extern float angle_udp[18];
	extern unsigned int out2[6];

int main(void)
{
	u8 angle;
	u8 t,i;
	u8 len;	
	u16 times=0;  
	delay_init(168);       																		//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//�жϷ�������
	uart_init(115200);   																			//���ڲ���������
	usmart_dev.init(84);																			//��ʼ��USMART
	LED_Init(); 																							//LED��ʼ��
//	KEY_Init();  																						
	LCD_Init();  																							//LCD��ʼ��
	
	Sensor_GPIO();																		  			//������GPIO_INTT
	
	FSMC_SRAM_Init(); 																				//SRAM��ʼ��
	
	mymem_init(SRAMIN);  																			//��ʼ���ڲ��ڴ��
	mymem_init(SRAMEX);  																			//��ʼ���ⲿ�ڴ��
	mymem_init(SRAMCCM); 																			//��ʼ��CCM�ڴ��
	
	POINT_COLOR = RED; 																				
	LCD_ShowString(10,0,200,20,16, "Explorer STM32F407");
	LCD_ShowString(10,15,200,20,16, "Opening HandExo...");
	LCD_ShowString(10,30,200,20,16, "ATOM@ALIENTEK");
	LCD_ShowString(10,45,200,20,16, "SuYuan 2022/10/18");
	LCD_ShowString(10,60,200,20,16,"Address/Port Information:");
	
	POINT_COLOR = BLUE; 			//��ɫ����

	OSInit(); 																								//UCOS init

//	while(1)
//	{
//		t = 1;
//		for(i=0;i<18;i++)
//		{
//			printf("%d",t);
//			printf("%f",angle_udp[i]);
//			delay_ms(1000);
//		}
//		printf("\r\n");
//	}
//		for(i=0;i<17;i++)
//		{
//			angle = angle_udp[i];
//			USART_SendData(USART1, USART_RX_BUF[angle]); 

//		}

	while(lwip_comm_init()) 																	//lwip init 
	{
		LCD_ShowString(10,75,200,20,16,"Lwip Init failed!"); 	
		delay_ms(500);
		LCD_Fill(30,130,230,150,WHITE);
		delay_ms(500);
	}
	LCD_ShowString(10,75,200,20,16,"Lwip Init Success!"); 		
	
	
	while(udp_demo_init()) 																		//create UDP thread
	{
		LCD_ShowString(10,90,200,20,16,"Created UDP thread fail !"); 				
		delay_ms(500);
		LCD_Fill(30,150,230,170,WHITE);
		delay_ms(500);
	}	
	LCD_ShowString(10,90,200,20,16,"Created UDP thread success!"); 	

	POINT_COLOR = BLACK;
	LCD_ShowString(10,175,300,40,16,"Author: YuanSu(yuan_sue@yeah.net)"); 
	LCD_ShowString(10,190,300,40,16,"Date: 2022-10-01");
	LCD_ShowString(10,205,300,20,16,"Project: HandExo for telerobotics"); 
	LCD_ShowString(10,220,400,20,16,"Thanks to Zuqiang Yuan and Gaofeng Li");
	LCD_ShowString(10,235,400,20,16,"for their contributions to this project!");
	
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);

	OSStart(); //start UCOS

}


//start task
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  							//��ʼ��ͳ������
	OS_ENTER_CRITICAL();  			//���ж�
#if	LWIP_DHCP
	lwip_comm_dhcp_creat();			//����DHCP����  //������ȡ������DHCP���� 2023-04-28 YSu
#endif
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//����LED����
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO); 	//����KEY����
	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //��ʾ����
	OSTaskSuspend(OS_PRIO_SELF); //����start_task����
	OS_EXIT_CRITICAL();  //���ж�
}


//��ʾ��ַ����Ϣ��������
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP															//������DHCP��ʱ��
		if(lwipdev.dhcpstatus != 0) 					//����DHCP
		{
			show_address(lwipdev.dhcpstatus );	//��ʾ��ַ��Ϣ
			OSTaskSuspend(OS_PRIO_SELF); 				//��ʾ���ַ��Ϣ�������������
		}
#else
		show_address(0); 											//��ʾ��̬��ַ
		OSTaskSuspend(OS_PRIO_SELF); 					//��ʾ���ַ��Ϣ�������������
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500);
	}
}

//key����
void key_task(void *pdata)
{
	u8 key; 
	while(1)
	{
		key = KEY_Scan(0);
		if(key==KEY0_PRES) //��������
		{
			udp_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
		}
		OSTimeDlyHMSM(0,0,0,10);  //��ʱ10ms
	}
}

//led����
void led_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);  //��ʱ500ms
 	}
}










