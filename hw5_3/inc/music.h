#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "stm32l476xx.h"
#include "timer_function.h"
#include "7seg_function.h"

int music_Duty_Cycle;

void music_Init();
void music_Play(int note);
void music_Read_Keypad(uint16_t keypad_Input, Seg_TypeDef *seg_Data);

uint32_t note_Arr[8];

#endif
