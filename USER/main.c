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



//KEY任务
#define KEY_TASK_PRIO 		8
//任务堆栈大小
#define KEY_STK_SIZE		128	
//任务堆栈
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);   

//LED任务
//任务优先级
#define LED_TASK_PRIO		9
//任务堆栈大小
#define LED_STK_SIZE		64
//任务堆栈
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);  

//在LCD上显示地址信息任务
//任务优先级
#define DISPLAY_TASK_PRIO	10
//任务堆栈大小
#define DISPLAY_STK_SIZE	128
//任务堆栈
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);

//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 



//在LCD上显示地址信息
//mode:1 显示DHCP获取到的地址
//	  其他 显示静态地址
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(10,105,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(10,120,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(10,135,210,16,16,buf); 
		LCD_ShowString(10,150,210,16,16,"Port:8089!"); 
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(10,105,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(10,120,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
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
	delay_init(168);       																		//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//中断分组配置
	uart_init(115200);   																			//串口波特率设置
	usmart_dev.init(84);																			//初始化USMART
	LED_Init(); 																							//LED初始化
//	KEY_Init();  																						
	LCD_Init();  																							//LCD初始化
	
	Sensor_GPIO();																		  			//传感器GPIO_INTT
	
	FSMC_SRAM_Init(); 																				//SRAM初始化
	
	mymem_init(SRAMIN);  																			//初始化内部内存池
	mymem_init(SRAMEX);  																			//初始化外部内存池
	mymem_init(SRAMCCM); 																			//初始化CCM内存池
	
	POINT_COLOR = RED; 																				
	LCD_ShowString(10,0,200,20,16, "Explorer STM32F407");
	LCD_ShowString(10,15,200,20,16, "Opening HandExo...");
	LCD_ShowString(10,30,200,20,16, "ATOM@ALIENTEK");
	LCD_ShowString(10,45,200,20,16, "SuYuan 2022/10/18");
	LCD_ShowString(10,60,200,20,16,"Address/Port Information:");
	
	POINT_COLOR = BLUE; 			//蓝色字体

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
	
	OSStatInit();  							//初始化统计任务
	OS_ENTER_CRITICAL();  			//关中断
#if	LWIP_DHCP
	lwip_comm_dhcp_creat();			//创建DHCP任务  //这里我取消创建DHCP任务 2023-04-28 YSu
#endif
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//创建LED任务
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO); 	//创建KEY任务
	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //显示任务
	OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
	OS_EXIT_CRITICAL();  //开中断
}


//显示地址等信息的任务函数
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP															//当开启DHCP的时候
		if(lwipdev.dhcpstatus != 0) 					//开启DHCP
		{
			show_address(lwipdev.dhcpstatus );	//显示地址信息
			OSTaskSuspend(OS_PRIO_SELF); 				//显示完地址信息后挂起自身任务
		}
#else
		show_address(0); 											//显示静态地址
		OSTaskSuspend(OS_PRIO_SELF); 					//显示完地址信息后挂起自身任务
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500);
	}
}

//key任务
void key_task(void *pdata)
{
	u8 key; 
	while(1)
	{
		key = KEY_Scan(0);
		if(key==KEY0_PRES) //发送数据
		{
			udp_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
		}
		OSTimeDlyHMSM(0,0,0,10);  //延时10ms
	}
}

//led任务
void led_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);  //延时500ms
 	}
}










