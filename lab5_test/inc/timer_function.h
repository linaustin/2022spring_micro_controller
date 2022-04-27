#ifndef __TIMER_FUNCTION_H__
#define __TIMER_FUNCTION_H_

#include "stm32l476xx.h"
#include "gpio_function.h"

typedef struct tim_Init_Data_Struct{
	int PSC;
	int ARR;
}Timer_Init_Data;

typedef struct tim_PWM_Init_Data_Struct{
	int PSC;
	int ARR;
	uint8_t channel;
	uint8_t OCxM;
	uint8_t CCxP;
	uint8_t CCxE;
}Timer_PWM_Init_Data;

void timer_Enable(TIM_TypeDef *tim);
void timer_Disable(TIM_TypeDef *tim);
void timer_Reload(TIM_TypeDef *tim);
void timer_Start(TIM_TypeDef *tim);
void timer_Stop(TIM_TypeDef *tim);
void timer_Init(TIM_TypeDef *tim, Timer_Init_Data *timer_Data);
void timer_PWM_Init(TIM_TypeDef *tim, Timer_PWM_Init_Data *timer_Data);

#endif
