#include "./BSP/AD7616/AD7616.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
uint8_t ad7616_range;
uint16_t ad7616_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM] = {0};
float ad7616f_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM] = {0};

void AD7616_Serial_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, AD7616_RNG1_Pin|AD7616_RNG0_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(AD7616_D4_GPIO_Port, AD7616_D4_Pin|AD7616_D11_Pin|AD7616_D12_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(AD7616_WR_GPIO_Port, AD7616_WR_Pin|AD7616_RD_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, AD7616_CHS0_Pin|AD7616_CHS1_Pin|AD7616_CHS2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(AD7616_SER_GPIO_Port, AD7616_SER_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, AD7616_CS_Pin|AD7616_RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, AD7616_CONV_Pin|AD7616_BUSY_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PFPin PFPin PFPin PFPin */
	
	GPIO_InitStruct.Pin = AD7616_RNG1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_RNG1_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_RNG0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_RNG0_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_CHS0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_CHS0_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_CHS1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_CHS1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AD7616_CHS2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_CHS2_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_CS_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_BUSY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_BUSY_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_RD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_RD_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_WR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_RD_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_RST_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_CONV_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_CONV_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_SER_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_SER_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_D11_Pin|AD7616_D12_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_D11_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD7616_D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(AD7616_D4_GPIO_Port, &GPIO_InitStruct);
	
}


static void AD7616_Working_Mode(uint8_t mode)
{
	if (mode == Hardware_Mode) {
		HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_SET);
	} else if (mode == Software_Mode) {
		HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_RESET);
  }
}


void AD7616_Init(uint8_t mode) {
	
	AD7616_Serial_GPIO_Init();
	HAL_GPIO_WritePin(AD7616_SER_GPIO_Port, AD7616_SER_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(AD7616_CS_GPIO_Port, AD7616_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD7616_RD_GPIO_Port, AD7616_RD_Pin, GPIO_PIN_SET);

	AD7616_Working_Mode(mode);
}

void AD7616_Reset(void) {
	HAL_GPIO_WritePin(AD7616_RST_GPIO_Port, AD7616_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(AD7616_RST_GPIO_Port, AD7616_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
}

void AD7616_Conversion(void) {
	HAL_GPIO_WritePin(AD7616_CONV_GPIO_Port, AD7616_CONV_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(AD7616_CONV_GPIO_Port, AD7616_CONV_Pin, GPIO_PIN_SET);
}

void AD7616_Set_Range(uint8_t range) {
	ad7616_range = range;
	
	if (ad7616_range == Range_2_5_V) {
		
		HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_RESET);
		
	} else if (ad7616_range == Range_5_V) {
		
		HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_SET);
		
	} else if (ad7616_range == Range_10_V) {
		
		HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_SET);
		
	}
}

static void AD7616_Get_Serial_Data(uint16_t *data) {
	  uint16_t shift = 0x8000;
	  uint16_t input_data_a = 0;
	  uint16_t input_data_b = 0;
	
	  GPIO_PinState state = GPIO_PIN_RESET;
	
	  for (uint16_t i = 0; i < 16; i++) {
		  
		HAL_GPIO_WritePin(AD7616_RD_GPIO_Port, AD7616_RD_Pin, GPIO_PIN_RESET);
		state = HAL_GPIO_ReadPin(AD7616_D12_GPIO_Port, AD7616_D12_Pin);
		  
		if (state == GPIO_PIN_SET) {
			
		  input_data_a |= shift;
		} else if (state == GPIO_PIN_RESET) {
		  input_data_a &= (~shift);
			
		}
		
		state = HAL_GPIO_ReadPin(AD7616_D11_GPIO_Port, AD7616_D11_Pin);
		
		if (state == GPIO_PIN_SET) {
			
		  input_data_b |= shift;
		} else if (state == GPIO_PIN_RESET) {
		  input_data_b &= (~shift);
			
		}
		
		shift >>= 1;
		HAL_GPIO_WritePin(AD7616_RD_GPIO_Port, AD7616_RD_Pin, GPIO_PIN_SET);
		
	  }
	  data[0] = input_data_a;
	  data[1] = input_data_b;
}

void AD7616_Read_Data(uint16_t *data) {
	GPIO_PinState state = GPIO_PIN_SET;
	do {
		state = HAL_GPIO_ReadPin(AD7616_BUSY_GPIO_Port, AD7616_BUSY_Pin);
	} while (state);
		HAL_GPIO_WritePin(AD7616_CS_GPIO_Port, AD7616_CS_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);

	AD7616_Get_Serial_Data(data);

	HAL_GPIO_WritePin(AD7616_CS_GPIO_Port, AD7616_CS_Pin, GPIO_PIN_SET);
}

void AD7616_Channel_Group_Select(uint8_t channel) {
#if USE_SOFTWARE_MODE
  uint8_t channel_data = (channel << 4) | channel;
#endif
  switch (channel) {
  case Channel_Group_0:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_1:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_2:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_3:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_4:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  case Channel_Group_5:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  case Channel_Group_6:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  case Channel_Group_7:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  }
}

double AD7616_Digital2Voltage(uint16_t data) {
	  int16_t signed_data = data;
	
	  switch (ad7616_range) {
	  case Range_2_5_V:
		return signed_data * 5.0 / 65536.0 * 1000.0;
	  case Range_5_V:
		return signed_data * 10.0 / 65536.0 * 1000.0;
	  case Range_10_V:
		return signed_data * 20.0 / 65536.0 * 1000.0;
	  }
	  return 0;
}

void AD7616_Set_Serial_Output_Format(uint8_t format) {
	
  if (format == Serial_Line_1_Output) {
	  
    HAL_GPIO_WritePin(AD7616_D4_GPIO_Port, AD7616_D4_Pin, GPIO_PIN_RESET);
	  
  } else if (format == Serial_Line_2_Output) {
	  
    HAL_GPIO_WritePin(AD7616_D4_GPIO_Port, AD7616_D4_Pin, GPIO_PIN_SET);
	  
  }
}

void adcTasks(void){

	uint16_t ad7616_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM] = {0};
	AD7616_Init(Hardware_Mode);
	
	AD7616_Set_Serial_Output_Format(Serial_Line_2_Output);
	
	AD7616_Set_Range(Range_10_V);
	AD7616_Channel_Group_Select(Channel_Group_0);
	
	AD7616_Reset();
	
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

    HAL_Delay(500);
  }
}
