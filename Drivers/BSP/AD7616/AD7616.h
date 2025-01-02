#ifndef __AD7616_H__
#define __AD7616_H__

#include <inttypes.h>

/* 串行模式：   0   并行模式：  1 */
#define USE_PARALLEL_MODE 0
/* 硬件模式：   0   软件模式：  1 */
#define USE_SOFTWARE_MODE 0
/* 输入通道组 */
#define AD7616_CHANNEL_GROUP_MAX 8

#define AD7616_CHANNEL_GROUP_NUM 2

#define AD7616_RNG0_Pin GPIO_PIN_4
#define AD7616_RNG0_GPIO_Port GPIOA
#define AD7616_RNG1_Pin GPIO_PIN_5
#define AD7616_RNG1_GPIO_Port GPIOA

#define AD7616_CHS0_Pin GPIO_PIN_7
#define AD7616_CHS0_GPIO_Port GPIOC
#define AD7616_CHS1_Pin GPIO_PIN_8
#define AD7616_CHS1_GPIO_Port GPIOC
#define AD7616_CHS2_Pin GPIO_PIN_9
#define AD7616_CHS2_GPIO_Port GPIOC

#define AD7616_SER_Pin GPIO_PIN_10
#define AD7616_SER_GPIO_Port GPIOC

#define AD7616_D4_Pin GPIO_PIN_11
#define AD7616_D4_GPIO_Port GPIOC
#define AD7616_D11_Pin GPIO_PIN_12
#define AD7616_D11_GPIO_Port GPIOC
#define AD7616_D12_Pin GPIO_PIN_13
#define AD7616_D12_GPIO_Port GPIOC

#define AD7616_WR_Pin GPIO_PIN_0
#define AD7616_WR_GPIO_Port GPIOB
#define AD7616_RD_Pin GPIO_PIN_1
#define AD7616_RD_GPIO_Port GPIOB

#define AD7616_CS_Pin GPIO_PIN_6
#define AD7616_CS_GPIO_Port GPIOB
#define AD7616_RST_Pin GPIO_PIN_7
#define AD7616_RST_GPIO_Port GPIOB

#define AD7616_CONV_Pin GPIO_PIN_12
#define AD7616_CONV_GPIO_Port GPIOB

#define AD7616_BUSY_Pin GPIO_PIN_13
#define AD7616_BUSY_GPIO_Port GPIOB


/* USER CODE END Private defines */

enum AD7616_Register_Address {
  Config_Register = 0x02,
  Channel_Register = 0x03,
  Input_Range_Register_A1,
  Input_Range_Register_A2,
  Input_Range_Register_B1,
  Input_Range_Register_B2,
};

/* 电压量程 */
enum AD7616_Range {
  Range_10_V = 0x00,
  Range_2_5_V,
  Range_5_V,
};

/* AD7616工作模式 */
enum AD7616_Working_Mode {
  Hardware_Mode = 0,
  Software_Mode,
};

enum AD7616_Channel_Group {
  Channel_Group_0 = 0x00,
  Channel_Group_1,
  Channel_Group_2,
  Channel_Group_3,
  Channel_Group_4,
  Channel_Group_5,
  Channel_Group_6,
  Channel_Group_7,
};

/* DoutX输出格式选择 */
enum AD7606B_Serial_Output_Format {
  Serial_Line_1_Output = 0x00,
  Serial_Line_2_Output,
};

void AD7616_Serial_GPIO_Init(void);
void AD7616_Init(uint8_t mode);

void AD7616_Reset(void);
void AD7616_Conversion(void);
void AD7616_Set_Range(uint8_t range);
void AD7616_Read_Data(uint16_t *data);
void AD7616_Channel_Group_Select(uint8_t channel);
double AD7616_Digital2Voltage(uint16_t data);
void AD7616_Set_Serial_Output_Format(uint8_t format);
void AD7616_Get_Serial_Data(uint16_t *data);

void adcTasks(void);
#endif
