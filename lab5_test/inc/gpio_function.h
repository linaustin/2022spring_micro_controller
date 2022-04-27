#ifndef __GPIO_FUNCTION__H__
#define __GPIO_FUNCTION__H__

#include "stm32l476xx.h"

int gpio_Init(GPIO_TypeDef *gpio, int pin);
int gpio_AF_Init(GPIO_TypeDef *gpio, int pin, uint8_t AF);
int gpio_Read(GPIO_TypeDef *gpio, int pin);
void gpio_Set(GPIO_TypeDef *gpio, int pin);
void gpio_Reset(GPIO_TypeDef *gpio, int pin);
void gpio_Toggle(GPIO_TypeDef *gpio, int pin);

#endif
