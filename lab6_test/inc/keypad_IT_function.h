#ifndef __KEYPAD_IT_FUNCTION_H__
#define __KEYPAD_IT_FUNCTION_H__

#include "stm32l476xx.h"
#include "timer_function.h"
#include "gpio_function.h"
#include "system_function.h"

typedef struct keypad_Struct{
	GPIO_TypeDef *out_Gpio[4];
	int out_Pin[4];

	GPIO_TypeDef *in_Gpio[4];
	int in_Pin[4];
}Keypad_TypeDef;

Keypad_TypeDef keypad_Gpio;

const int  keypad_Map[4][4];
int out_Pin_Index;

uint16_t keypad_Input;

int keypad_IT_Init();
void keypad_EXTI_config(GPIO_TypeDef *gpio, int pin);

void TIM3_IRQHandler();
void EXTI9_5_IRQHandler();
void EXTI15_10_IRQHandler();

#endif
