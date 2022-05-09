#ifndef __TIMER_FUNCTION_H__
#define __TIMER_FUNCTION_H__

#include "stm32l476xx.h"
#include "gpio_function.h"

typedef struct tim_Init_Data_Struct{
	int PSC;
	int ARR;
}Timer_Init_Data;

typedef struct tim_PWM_Init_Data_Struct{
	uint8_t channel;
	int PSC;
	int ARR;
	int CCRx;
	//uint8_t CCxP 0 = active high 1 = active low
}Timer_PWM_Init_Data;

void timer_Init(TIM_TypeDef *tim, Timer_Init_Data *timer_Data);
void timer_PWM_Init(TIM_TypeDef *tim, Timer_PWM_Init_Data *timer_Data);
void timer_Enable(TIM_TypeDef *tim);
void timer_Disable(TIM_TypeDef *tim);
void timer_Reload(TIM_TypeDef *tim);
void timer_Start(TIM_TypeDef *tim);
void timer_Start_IT(TIM_TypeDef *tim);
void timer_Stop(TIM_TypeDef *tim);
void timer_Stop_IT(TIM_TypeDef *tim);
void timer_Channel_Start(TIM_TypeDef *tim, int channel);
void timer_Channel_Stop(TIM_TypeDef *tim, int channel);

void __attribute__((weak)) TIM2_IRQHandler();
void __attribute__((weak)) TIM3_IRQHandler();


#endif
