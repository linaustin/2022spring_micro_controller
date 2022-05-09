#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "button_function.h"
#include "7seg_function.h"
#include "keypad_IT_function.h"
#include "system_function.h"
#include "timer_function.h"

Seg_TypeDef seg_Gpio;
int count_Time[3];

int clock_Init();
void clock_Config();
void clock_Counting();
void clock_Alarming();

void clock_Display(int *time_Data);


void TIM4_IRQHandler();
#endif
