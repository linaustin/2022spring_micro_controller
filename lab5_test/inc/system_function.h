#ifndef __SYSTEM_FUNCTION_H__
#define __SYSTEM_FUNCTION_H__

#include "stm32l476xx.h"

typedef struct sysclk_Data_Struct{
	// sysclk = source * plln/2* (pllr + 1) * (pllm + 1)
	uint8_t pllm;
	uint8_t plln;
	uint8_t pllr;
}Sysclk_Data;

void FPU_Init();
void delay_Loop(int msec);

void system_Clock_Config(Sysclk_Data *sysclk_Data);

#endif
