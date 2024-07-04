#include "./BSP/SENSOR/sensor.h" 
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

uint8_t ADDR=1;//������ַ 



//��ʼ�Ƕ�
unsigned int out[6];
unsigned int out2[6];
unsigned int out3[6];

float anglef[6];//��ʼ�Ƕ�
float anglef2[6];
float anglef3[6];


float anglem[6];//�м�Ƕ�

float angle[6];
float angle2[6];
float angle3[6];

float angle_udp[18];

uint8_t  ZF_flag[6];	//��ǰ�Ƕȼ�����


unsigned char xun;//forѭ������


//�˿ڽ����йر���
uint8_t Rx_Buffer[6];//��λ������6�ֽڣ�֡ͷ+֡��+��������+����+У��+֡β
uint8_t Rx_Cnt=0;
uint8_t Rx_Cmd=0;
uint8_t Rx_Num=0;
uint8_t Rx_Flag=0;

//uint8_t  err1_flag=0;
//uint8_t  err2_flag=0;


/* Private variables ---------------------------------------------------------*/
/*******************************************************************/	
/*�������ƣ�Sensor_GPIO                                            */
/*��    �룺��   												   */
/*��    ������                                                     */
/*����˵��������I/O�ڡ�                                            */
/*                                                                 */
/*******************************************************************/
void Sensor_GPIO(void)
{
	
    GPIO_InitTypeDef GPIO_InitStructure;
		//ʱ�ӳ�ʼ��
		// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
		
		//__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
		
	
		//�ű�����������1I/O����
	//PB3:CSn1, PB5:CLK1, PC2:CSn2, PC3:CLK2, PB12:CSn3, PB13:CLK3
	
	GPIO_InitStructure.Pin  = GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_6;      
  	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin   = GPIO_PIN_2;									//PB4:DIO1
  	GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;       
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin   = GPIO_PIN_0;									//PC0:DIO2
  	GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;       
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin   = GPIO_PIN_6;									//PC6:DIO3
  	GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;       
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin = GPIO_PIN_2|GPIO_PIN_3;          //PC2,PC3
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		//��ʼ����prog����Ϊ��
		PROG1_L;
	
	
/// ---------------------------------------------------------
/// Created by Anakin_Bao(a1282319520@163.com) on 2024/1/15
///	Add another two fingers
/// ---------------------------------------------------------

			
}


/*******************************************************************/	
/*�������ƣ�read_change                                            */
/*��    �룺��   												   */
/*��    ������                                                     */
/*����˵�������ű���������                                            */
/*                                                                 */
/*******************************************************************/

void read_change(void)
{
	
  	unsigned int tempdat[6];
		unsigned int tempdat2[6];
		unsigned int tempdat3[6];
		
  	uint8_t Ocf[6],Cof[6],Lin[6];							//OCFΪƫ�����ɱ�ʶ,COFΪCARDIC��Ԫ�����ʶ��LINΪ�ų����Զȱ���
		uint8_t Ocf2[6],Cof2[6],Lin2[6];
		uint8_t Ocf3[6],Cof3[6],Lin3[6];
	
  	CSn1_H; CSn2_H;	CSn3_H;								//CSnƬѡ�ȱ��ָߵ�ƽ
  	CLK1_H; CLK2_H;	CLK3_H;								//ʱ�ӱ��ָߵ�ƽ
  	delay_us(1);
	
  	CSn1_L;	CSn2_L;	CSn3_L; 							//���CSn��Ϊ�͵�ƽ���������DO���Ӹ���״̬��Ϊ�ߵ�ƽ��������ȡ����
  	delay_us(1);
  	CLK1_L; CLK2_L; CLK3_L;
  	delay_us(1);
		
  	for(xun=0;xun<15;xun++)											//daisy chain 1 18bit
  	{
		CLK1_H;	CLK2_H; CLK3_H;									//���ݽ���CLK�ĵ�һ�������������������λ�Ĵ�����
  		delay_us(1);

    	if(DO1)
    	{
    		tempdat[0]=tempdat[0]<<1; 						//finger1��һ��������
        	tempdat[0]|=0x0001;
				//printf("xun:%d,do1:1\n\r",xun);
    	}
    	else 
    	{
        	tempdat[0]=tempdat[0]<<1;
        	tempdat[0]&=0xfffe;
				//printf("xun:%d,do1:0\n\r",xun);
    	}
			
/// ---------------------------------------------------------
/// Created by YuanSu(yuan_sue@yeah.net) on 2022/10/10
///	finger2/finger3 first sensor
/// ---------------------------------------------------------	
			
		if(DO2)
		{
			tempdat2[0]=tempdat2[0]<<1;		
			tempdat2[0]|=0x0001;
		}
		else
		{
			tempdat2[0]=tempdat2[0]<<1;
			tempdat2[0]&=0xfffe;
		}
			
		if(DO3)
		{
			tempdat3[0]=tempdat3[0]<<1;
			tempdat3[0]|=0x0001;
		}
		else
		{
			tempdat3[0]=tempdat3[0]<<1;
			tempdat3[0]&=0xfffe;
		}
						
		delay_us(1);
    	CLK1_L; CLK2_L; CLK3_L;
  		delay_us(1);
  	};
	
  	for(xun=0;xun<4;xun++)						//���ʱ��ΪʲôҪ������������4�Σ�����
  	{   
		CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
		CLK1_L; CLK2_L; CLK3_L;
		delay_us(1);
	};
	
	
	/////////////////////////////////////////////////////////////////////////////////////////
  	for(xun=0;xun<15;xun++)						//daisy chain 1 18bit
  	{
		
      	CLK1_H;	CLK2_H; CLK3_H;											//���ݽ���CLK�ĵ�һ�������������������λ�Ĵ�����
		delay_us(1);
      
		if(DO1)
      	{
        	tempdat[1]=tempdat[1]<<1;		//������������
        	tempdat[1]|=0x0001;
      	}
      	else 
      	{
        	tempdat[1]=tempdat[1]<<1;
        	tempdat[1]&=0xfffe;
      	}
			
		if(DO2)
		{
			tempdat2[1]=tempdat2[1]<<1;
			tempdat2[1]|=0x0001;
		}
		else
		{
			tempdat2[1]=tempdat2[1]<<1;
			tempdat2[1]&=0xfffe;
		}
			
		if(DO3)
		{
			tempdat3[1]=tempdat3[1]<<1;
			tempdat3[1]|=0x0001;
		}
		else
		{
			tempdat3[1]=tempdat3[1]<<1;
			tempdat3[1]&=0xfffe;
		}
					
  		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
  		delay_us(1);
  	}
	
  	for(xun=0;xun<4;xun++)
  	{    
		CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
      	CLK1_L;	CLK2_L;	CLK3_L;
		delay_us(1);
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
  	for(xun=0;xun<15;xun++)						//daisy chain 1 18bit
  	{
		
      	CLK1_H;	CLK2_H; CLK3_H;											//���ݽ���CLK�ĵ�һ�������������������λ�Ĵ�����
		delay_us(1);
      
		if(DO1)
      	{
        	tempdat[2]=tempdat[2]<<1;		//������������
        	tempdat[2]|=0x0001;
      	}
     	else 
      	{
        	tempdat[2]=tempdat[2]<<1;
        	tempdat[2]&=0xfffe;
      	}
			
		if(DO2)
		{
			tempdat2[2]=tempdat2[2]<<1;
			tempdat2[2]|=0x0001;
		}
		else
		{
			tempdat2[2]=tempdat2[2]<<1;
			tempdat2[2]&=0xfffe;
		}
			
		if(DO3)
		{
			tempdat3[2]=tempdat3[2]<<1;
			tempdat3[2]|=0x0001;
		}
		else
		{
			tempdat3[2]=tempdat3[2]<<1;
			tempdat3[2]&=0xfffe;
		}
					
  		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
  		delay_us(1);
  	}
	
  	for(xun=0;xun<4;xun++)
  	{    
		CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
      	CLK1_L;	CLK2_L;	CLK3_L;
		delay_us(1);
	};
	
	
	////////////////////////////////////////////////////////////////////////////////////////
  	for(xun=0;xun<15;xun++)//daisy chain 1 18bit
  	{
		
      	CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
      	if(DO1)
      	{
        	tempdat[3]=tempdat[3]<<1;//���ĸ�������
        	tempdat[3]|=0x0001;
      	}
      	else 
      	{
        	tempdat[3]=tempdat[3]<<1;
        	tempdat[3]&=0xfffe;
      	}
			
		if(DO2)
		{
			tempdat2[3]=tempdat2[3]<<1;
			tempdat2[3]|=0x0001;
		}
		else
		{
			tempdat2[3]=tempdat2[3]<<1;
			tempdat2[3]&=0xfffe;
		}
			
		if(DO3)
		{
			tempdat3[3]=tempdat3[3]<<1;
			tempdat3[3]|=0x0001;
		}
		else
		{
			tempdat3[3]=tempdat3[3]<<1;
			tempdat3[3]&=0xfffe;
		}
		
		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
		delay_us(1);
  	}
  	for(xun=0;xun<4;xun++)
  	{    
		CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
		delay_us(1);
	};
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
  	for(xun=0;xun<15;xun++)//daisy chain 1 18bit
  	{
		
      	CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
      	if(DO1)
      	{
        	tempdat[4]=tempdat[4]<<1;//���ĸ�������
        	tempdat[4]|=0x0001;
      	}
      	else 
      	{
        	tempdat[4]=tempdat[4]<<1;
        	tempdat[4]&=0xfffe;
      	}
			
		if(DO2)
		{
			tempdat2[4]=tempdat2[4]<<1;
			tempdat2[4]|=0x0001;
		}
		else
		{
			tempdat2[4]=tempdat2[4]<<1;
			tempdat2[4]&=0xfffe;
		}
			
		if(DO3)
		{
			tempdat3[4]=tempdat3[4]<<1;
			tempdat3[4]|=0x0001;
		}
		else
		{
			tempdat3[4]=tempdat3[4]<<1;
			tempdat3[4]&=0xfffe;
		}
		
		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
		delay_us(1);
  	}
  	for(xun=0;xun<4;xun++)
  	{    
		CLK1_H; CLK2_H; CLK3_H;
		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
		delay_us(1);
	};
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
  	for(xun=0;xun<15;xun++)//daisy chain 1 18bit
  	{
		
      	CLK1_H;	CLK2_H; CLK3_H;
		delay_us(1);
      	if(DO1)
      	{
        	tempdat[5]=tempdat[5]<<1;	//������������
        	tempdat[5]|=0x0001;
      	}
      	else 
      	{
        	tempdat[5]=tempdat[5]<<1;
        	tempdat[5]&=0xfffe;
      	}
			
		if(DO2)
		{
        	tempdat2[5]=tempdat2[5]<<1;	
        	tempdat2[5]|=0x0001;			
		}
		else
		{
        	tempdat2[5]=tempdat2[5]<<1;
        	tempdat2[5]&=0xfffe;			
		}
			
		if(DO3)
		{
			tempdat3[5]=tempdat3[5]<<1;
			tempdat3[5]|=0x0001;
		}
		else
		{
			tempdat3[5]=tempdat3[5]<<1;
			tempdat3[5]&=0xfffe;
		}
					
		delay_us(1);
      	CLK1_L; CLK2_L; CLK3_L;
		delay_us(1);
  	}
	
	delay_us(1);
	
	CSn1_H; CSn2_H; CSn3_H; 		//ͨ��һ��CSN�ĸ�����������������
	CLK1_H;	CLK2_H; CLK3_H;
	delay_us(1);
	
	
	//degree out
	for(xun=0;xun<6;xun++)
	{
	
  		Ocf[xun]=(tempdat[xun] & (1<<2))>>2;	Ocf2[xun]=(tempdat2[xun] & (1<<2))>>2;	Ocf3[xun]=(tempdat3[xun] & (1<<2))>>2;
  		Cof[xun]=(tempdat[xun] & (1<<1))>>1;	Cof2[xun]=(tempdat2[xun] & (1<<1))>>1;	Cof3[xun]=(tempdat3[xun] & (1<<1))>>1;
  		Lin[xun]=(tempdat[xun] & (1<<0))>>0;	Lin2[xun]=(tempdat2[xun] & (1<<0))>>0;	Lin3[xun]=(tempdat3[xun] & (1<<0))>>0;
	
  		if(Ocf[xun]==1 && Cof[xun]==0 && Lin[xun]==0)
		{
			out[xun]=tempdat[xun]>> 3;
		}									//�����㷨���OCF=1��û�����COF=0�����Զȱ���LIN=0
  		else
		{	
			out[xun]=4096;
		}															//AS5045��Χ0-4095������4096��˵��������װ�����ʣ���Ҫ������
	
		if(Ocf2[xun]==1 && Cof2[xun]==0 && Lin2[xun]==0)
		{
			out2[xun]=tempdat2[xun]>> 3;
		}								
  		else
		{
			out2[xun]=4096;
		}	

		if(Ocf3[xun]==1 && Cof3[xun]==0 && Lin3[xun]==0)
		{
			out3[xun]=tempdat3[xun]>> 3;
		}									
  		else
		{
			out3[xun]=4096;
		}													
		
	}

}

/*******************************************************************/	
/*�������ƣ�zero_otp                                            */
/*��    �룺��   												   */
/*��    ������                                                     */
/*����˵�����ű���������������λ����                                            */
/*                                                                 */
/*******************************************************************/
//void zero_otp(void)
//{
//  unsigned int tmpdata[5];
//  unsigned int ssivalue=0;
//  unsigned char i;
//  for(i=0;i<5;i++){tmpdata[i]=read_change();}//��ȡ��ǰ�ǶȲɼ�ֵ 
////  tmpdata[4]|=0x0004;//PWM��ֹ����ֹLIN����
//  ssivalue=(((tmpdata[4]<<3) | (0<<15))| 0x3);
//  delay_ms(10);//��ʱ10ms
//  CSn1_L;
//  CLK1_L;//ʱ����Ϊ��
//  PROG1_L;
//  delay_ms(10);//��ʱ10ms
//  PROG1_H;
//  delay_ms(10);//��ʱ10ms
//  CSn1_H; //CSnƬѡ�ȱ��ָߵ�ƽ
//  delay_ms(10);//��ʱ10ms
//  for(i=0;i<16;i++)//daisy chain 1 18bit
//  {
//   if(ssivalue&0x8000){PROG1_H;}
//   else{PROG1_L;}
//   delay_ms(1);//��ʱ1ms
//   ssivalue=ssivalue<<1;
//   CLK1_H;//���ݽ���CLK�ĵ�һ�������������������λ�Ĵ�����
//   delay_ms(2);//��ʱ2ms
//   CLK1_L;
//   delay_ms(1);//��ʱ1ms
// }
//   PROG1_L;
//   CLK1_L;
//   delay_ms(10);//��ʱ10ms
//   CSn1_L;//ͨ��һ��CSN�ĵ�����������������
//}
/*******************************************************************/	


/*******************************************************************/	
/*�������ƣ�GetDegreeo                                       */
/*��    �룺��   												   */
/*��    ������                                                     */
/*����˵������ȡ6���Ŵ������Ƕȳ�ʼֵ                                            */
/*                                                                 */
/*******************************************************************/
void GetDegreeo(void)
{

	read_change();
	for(xun=0;xun<6;xun++)
	{
  	anglef[xun]=out[xun]*45.0/512.0; 				//finger1 ��ʼ�Ƕ�	value*360/4096 =value=45/512
		anglef2[xun]=out2[xun]*45.0/512.0;			//finger2 ��ʼ�Ƕ�
		anglef3[xun]=out3[xun]*45.0/512.0;			//finger3 ��ʼ�Ƕ�
	};
	
//	printf("hand1 angle1:%f\n\r",anglef[0]);
//	printf("hand1 angle2:%f\n\r",anglef[1]);
//	printf("hand1 angle3:%f\n\r",anglef[2]);
//	printf("hand1 angle4:%f\n\r",anglef[3]);
//	printf("hand1 angle5:%f\n\r",anglef[4]);
//	printf("hand1 angle6:%f\n\r",anglef[5]);
}
/*******************************************************************/	
/*�������ƣ�Get_LegsDegree                                       */
/*��    �룺��   												   */
/*��    ������                                                     */
/*����˵������ȡ6���Ŵ������Ƕ�ֵ                                            */
/*                                                                 */
/*******************************************************************/

void GetDegree(void)
{
	
  	float angletmp[6];
	float angletmp2[6];
	float angletmp3[6];
	
	read_change();
	
	for(xun=0;xun<6;xun++)
	{  
	
		angletmp[xun]=out[xun]*45.0/512.0;																//��ǰ������6���������Ƕ�
		angletmp2[xun]=out2[xun]*45.0/512.0;
		angletmp3[xun]=out3[xun]*45.0/512.0;
	
  		angletmp[xun]=angletmp[xun]-anglef[xun];													//��ǰ�Ƕ�-��ʼ�Ƕ� = ��ԽǶ�
		angletmp2[xun]=angletmp2[xun]-anglef2[xun];
		angletmp3[xun]=angletmp3[xun]-anglef3[xun];
		
		if(fabs(angletmp[xun])<180) {angle[xun]=angletmp[xun];}						//����ԽǶȸ���angle[6]
  		else if (angletmp[xun]<-180) {angle[xun]=angletmp[xun]+360;}				//�ɼ�ֵ���������
  		else if (angletmp[xun]>180) {angle[xun]=angletmp[xun]-360;}				//�ɼ�ֵ���������
  		else{;}
		
		if(fabs(angletmp2[xun])<180){angle2[xun]=angletmp2[xun];}
		else if (angletmp2[xun]<-180){angle2[xun]=angletmp2[xun]+360;}	
		else if (angletmp2[xun]>180){angle2[xun]=angletmp2[xun]-360;}
		else{;}
		
		if(fabs(angletmp3[xun])<180){angle3[xun]=angletmp3[xun];}
		else if (angletmp3[xun]<-180){angle3[xun]=angletmp3[xun]+360;}
		else if (angletmp3[xun]>180){angle3[xun]=angletmp3[xun]-360;}
		else{;}
	
			
		if(angle[xun]<-180){angle[xun]+=360;}  													//�ɼ�ֵ��������㣬��֤�ɼ�ֵ��-180��~180��
		if(angle[xun]>180){angle[xun]+=-360;}; 													//�ɼ�ֵ���������
		
		if(angle2[xun]<-180){angle2[xun]+=360;}
		if(angle2[xun]>180){angle2[xun]+=-360;};
	
		if(angle3[xun]<-180){angle3[xun]+=360;}
		if(angle3[xun]>180){angle3[xun]+=-360;};
	
		
	}; 
  
//  if(OUT1)err1_flag=0;//����Ϊ0������Ϊ1
//  else err1_flag=1;
//  if(OUT2)err2_flag=0;
//  else err2_flag=1;

	angle_udp[0] = angle[0];	angle_udp[1] = angle[1];	angle_udp[2] = angle[2];	angle_udp[3] = angle[3];	angle_udp[4] = angle[4];	angle_udp[5] = angle[5];
	angle_udp[6] = angle2[0];	angle_udp[7] = angle2[1];	angle_udp[8] = angle2[2];	angle_udp[9] = angle2[3];	angle_udp[10] = angle2[4];	angle_udp[11] = angle2[5];
	angle_udp[12] = angle3[0];	angle_udp[13] = angle3[1];	angle_udp[14] = angle3[2];	angle_udp[15] = angle3[3];	angle_udp[16] = angle3[4];	angle_udp[17] = angle3[5];
//	printf("\n\r");
//	printf("index angle 1 : %0.2f\n",angle[0]);
//	printf("index angle 2 : %0.2f\n",angle[1]);
//	printf("index angle 3 : %0.2f\n",angle[2]);
//	printf("index angle 4 : %0.2f\n",angle[3]);
//	printf("index angle 5 : %0.2f\n",angle[4]);
//	printf("index angle 6 : %0.2f\n",angle[5]);
}

/*                                                                 */
/*******************************************************************/
void send_bytes(void) //���͵������������ֽڴ�
{
		
  	uint8_t dat=0;
		
//  dat=angle[0];//�ȷ����ݸ��ֽ�
//printf("%d %f\r\n",ZF_flag[0],angle[0]);
//  dat=angle[1];//�ٷ����ݵ��ֽ�
	printf("%f\r\n",angle[5]);
//  dat=err1_flag;//�ٷ�������ָʾλ1
//printf("%d\r\n",dat);
//  dat=err2_flag;//�ٷ�����Զָʾλ2
//printf("%d\r\n",dat);
//  dat=0;//�ٷ�0
//printf("%d\r\n",dat); 
}
	
 
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
//void GPIO_Configuration(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  /* CAN Periph clock enable */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO ,ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE); 
//  /* Configure CAN pin: RX */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);   
//  /* Configure CAN pin: TX */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);	     

//}
