#include "clock.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

int count_Time = {0, 0, 0};

int clock_Init(){
	system_Clock_Config(&sysclk_40Mhz);

	if(keypad_IT_Init() != 0){
		return -1;
	}

	if(init_7seg(&seg_Gpio) != 0){
		return -1;
	}

	Timer_PWM_Init_Data timer_PWM = {
			.channel = 1,
			.PSC = 40 -1,
			.ARR = 1912,
			.CCRx = 1912/2,
	};

	timer_Enable(TIM2);
	timer_PWM_Init(TIM2, &timer_PWM);
	timer_Start(TIM2);

	Timer_Init_Data timer_Display = {
			.PSC = (40*1000) - 1,
			.ARR = 10 - 1
	};

	timer_Enable(TIM4);
	timer_Init(TIM4, &timer_Display);

	NVIC_SetPriority(TIM4_IRQn, 0b1000);
	NVIC_EnableIRQ(TIM4_IRQn);

	timer_start_IT(TIM4);

	return 0;
}

void clock_Display(int *time_Data){
	send_7seg_Int(&seg_Gpio, 0, time_Data[0]%10, 0);
	send_7seg_Int(&seg_Gpio, 1, time_Data[0]/10, 0);
	send_7seg_Char(&seg_Gpio, 2, '-');

	send_7seg_Int(&seg_Gpio, 3, time_Data[1]%10, 0);
	send_7seg_Int(&seg_Gpio, 4, time_Data[1]/10, 0);
	send_7seg_Char(&seg_Gpio, 5, '-');

	send_7seg_Int(&seg_Gpio, 6, time_Data[2]%10, 0);
	send_7seg_Int(&seg_Gpio, 7, time_Data[2]/10, 0);

	return;
}

void TIM4_IRQHandler(){
	clock_Display(count_Time);

	return;
}
