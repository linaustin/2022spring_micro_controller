#include "stm32l476xx.h"
#include "adc_function.h"
#include "timer_function.h"
#include "7seg_function.h"

Seg_TypeDef seg_Gpio= {
		.gpio = GPIOC,
		.DIN = 5,
		.CS = 6,
		.CLK = 8
};



void  TIM2_IRQHandler();

int main(){

	system_Clock_Config(&sysclk_40Mhz);
	FPU_Init();

	if(init_7seg(&seg_Gpio) != 0){
		return -1;
	}

	ADC_Init_Data adc_Data ={
			.resolution = 0b00,
			.continus = 0b01,
			.align = 0b00,
			.common_Dual_Mode = 0b00,
			.common_Clock_Mode = 0b11,
			.common_DMA_Mode = 0b00,
			.channel = 1,
			.sample_Time = 0b010
	};

	adc_Init(ADC1, &adc_Data);
	adc_Enable(ADC1);
	adc_Start(ADC1);


	NVIC_SetPriorityGrouping(0b101);
	NVIC_SetPriority(TIM2_IRQn, 0b0000);
	NVIC_EnableIRQ(TIM2_IRQn);

	Timer_Init_Data tim_Data ={
			.PSC = (40*1000) - 1,
			.ARR = 1000 - 1
	};

	timer_Enable(TIM2);
	timer_Init(TIM2, &tim_Data);
	timer_Start_IT(TIM2);

	while(1){

	}

	return 0;
}

void  TIM2_IRQHandler(){
	TIM2->SR &= ~TIM_SR_UIF;

	float adc_Value = (float)adc_Get_Value(ADC1);

	float voltage = (adc_Value/4096)*3.3;

	float resistance = ((3.3-voltage)*2000)/voltage;

	int display_Number = (int)(resistance*100);

	display_Number_Float2(&seg_Gpio, display_Number);

	return;
}
