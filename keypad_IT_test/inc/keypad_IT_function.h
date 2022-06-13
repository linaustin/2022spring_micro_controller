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
int last_Key;
int debounce_Cnt;
int keypad_EXTI_Key;

int keypad_IT_Init();
void keypad_EXTI_Config(GPIO_TypeDef *gpio, int pin);
void keypad_EXTI_Handle(int key);
int keypad_Read();

void TIM3_IRQHandler();
void EXTI3_IRQHandler();
void EXTI4_IRQHandler();
void EXTI9_5_IRQHandler();

#endif
