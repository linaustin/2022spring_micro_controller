#include "timer_function.h"

void timer_Enable(TIM_TypeDef *tim){
	if(tim == TIM2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	}
	else if(tim == TIM3){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	}

	return;
}

void timer_Disable(TIM_TypeDef *tim){
	if(tim == TIM2){
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;
	}
	else if(tim == TIM3){
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM3EN;
	}

	return;
}

void timer_Reload(TIM_TypeDef *tim){
	tim->EGR |= TIM_EGR_UG;
	return;
}

void timer_Start(TIM_TypeDef *tim){
	tim->CR1 |= TIM_CR1_CEN;
	return;
}

void timer_Stop(TIM_TypeDef *tim){
	tim->CR1 &= ~TIM_CR1_CEN;
	return;
}

void timer_Init(TIM_TypeDef *tim, Timer_Init_Data *timer_Data){
	tim->ARR = (uint32_t)timer_Data->ARR;
	tim->PSC = (uint32_t)timer_Data->PSC;
	tim->EGR |= TIM_EGR_UG;
	return;
}

void timer_PWM_Init(TIM_TypeDef *tim, Timer_PWM_Init_Data *timer_Data){
	if((tim == TIM2) && (timer_Data->channel == 1)){
		tim->ARR = (uint32_t)timer_Data->ARR;
		tim->PSC = (uint32_t)timer_Data->PSC;

		gpio_AF_Init(GPIOA, 0, 0b0001); // TIM2_CH1¡@= PA0(AF1)  init PA0 AF1 function


	}
	return;
}

