#include "stm32l476xx.h"
#include "adc_function.h"
#include "timer_function.h"
#include "7seg_function.h"
#include "button_function.h"

Seg_TypeDef seg_Gpio= {
		.gpio = GPIOC,
		.DIN = 5,
		.CS = 6,
		.CLK = 8
};

int servo_Direction = 1;
int servo_State = 0;
int degree[5] = {0, 45, 90, 135, 180};
int degree_Pulse_Width[5] = {500, 975, 1450, 1925, 2400};


void  TIM4_IRQHandler();

int main(){

	system_Clock_Config(&sysclk_40Mhz);

	if(init_7seg(&seg_Gpio) != 0){
		return -1;
	}

	Timer_PWM_Init_Data timer_PWM_Data ={
			.channel = 1,
			.PSC = 40 - 1,
			.ARR = 5000 - 1,
			.CCRx = 500
	};

	timer_Enable(TIM2);
	timer_PWM_Init(TIM2, &timer_PWM_Data);
	timer_Start(TIM2);
	timer_Channel_Start(TIM2, 1);

	NVIC_SetPriorityGrouping(0b101);
	NVIC_SetPriority(TIM4_IRQn,0b0000);
	NVIC_EnableIRQ(TIM4_IRQn);

	Timer_Init_Data tim_Data ={
			.PSC = (40*1000) - 1,
			.ARR = 1000 - 1
	};

	timer_Enable(TIM4);
	timer_Init(TIM4, &tim_Data);
	timer_Start_IT(TIM4);

	while(1){

	}

	return 0;
}

void  TIM4_IRQHandler(){
	TIM4->SR &= ~TIM_SR_UIF;

	if(servo_State == 0){
		servo_Direction = 1;
	}
	else if(servo_State == 4){
		servo_Direction = 0;
	}

	if(servo_Direction){
		servo_State++;
	}
	else{
		servo_State--;
	}

	TIM2->CCR1 = degree_Pulse_Width[servo_State];

	display_Number_Float2(&seg_Gpio, degree[servo_State]*100);

	return;
}
