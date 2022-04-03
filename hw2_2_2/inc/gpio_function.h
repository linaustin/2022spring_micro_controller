#ifndef __GPIO_FUNCTION__H__
#define __GPIO_FUNCTION__H__

#include "stm32l476xx.h"

int init_Gpio(GPIO_TypeDef* gpio, int pin);
int read_Gpio(GPIO_TypeDef* gpio, int pin);

void set_Gpio(GPIO_TypeDef* gpio, int pin);
void reset_Gpio(GPIO_TypeDef* gpio, int pin);
void toggle_Gpio(GPIO_TypeDef* gpio, int pin);

void delay(int msec);

#endif
