#ifndef __ADC_FUNCTION_H__
#define __ADC_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"
#include "system_function.h"

typedef struct adc_Init_Data_Struct{
	uint8_t resolution;	//0b00 12 bits    0b01 10 bits    0b10 8 bits    0b11 6 bits
	uint8_t continus;	//0b00 conversion once    0b01 continuous conversion
	uint8_t align;	//0b00 align right    0b01 aligin left
	uint8_t common_Dual_Mode;	//0b00 independent mode
	uint8_t common_Clock_Mode;	//0b00 pllsai1 clock    0b01 AHB clk divide by 1    0b10 AHB clk divide by 2    0b11 AHB clk divide by 4
	uint8_t common_DMA_Mode;	//0b00 dma mode disable    0b10 dma mode for 12 and 10 bits resolution    0b11 dma mode for 8 and 6 bits resolution
	uint8_t channel;	//0 ref_channel 1 channel_1 ... 18 channel_18
	uint8_t sample_Time;	//0b000 2.5 cycle    0b001 6.5 cycle    0b010 12.5 cycle    0b011 24.5 cycle    0b100 47.5 cycle    0b101 92.5 cycle    0b110 247.5 cycle    0b111 640.5 cycle
}ADC_Init_Data;

void adc_Init(ADC_TypeDef *adc, ADC_Init_Data *adc_Data);
void adc_Enable(ADC_TypeDef *adc);
void adc_Disable(ADC_TypeDef *adc);
void adc_Start(ADC_TypeDef *adc);
void adc_Start_IT(ADC_TypeDef *adc);
void adc_Stop(ADC_TypeDef *adc);
void adc_Stop_IT(ADC_TypeDef *adc);
uint16_t adc_Get_Value(ADC_TypeDef *adc);

#endif
