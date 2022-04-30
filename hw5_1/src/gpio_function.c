#include <gpio_function.h>

int gpio_Init(GPIO_TypeDef* gpio, int pin){

	if(gpio == GPIOA){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	}
	else if(gpio == GPIOB){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}
	else if(gpio == GPIOC){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}
	else{
		return -1;
	}

	gpio->MODER &= ~(0b11 << (2 * pin));
	gpio->MODER |= (0b01 << (2 * pin));

	return 0;
}

int gpio_AF_Init(GPIO_TypeDef *gpio, int pin, uint8_t AF){
	if(gpio == GPIOA){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	}
	else if(gpio == GPIOB){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}
	else if(gpio == GPIOC){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}
	else{
		return -1;
	}

	gpio->MODER &= ~(0b11 << pin);
	gpio->MODER |= (0b10 << pin);

	if(pin < 8){
		gpio->AFR[0] &= ~(0b1111 << (4*pin));
		gpio->AFR[0] |= (AF << (4*pin));
	}
	else{
		pin = pin -8;
		gpio->AFR[1] &= ~(0b1111 << (4*pin));
		gpio->AFR[1] |= (AF << (4*pin));
	}

	return 0;
}

int gpio_Read(GPIO_TypeDef* gpio, int pin){

	return (gpio->IDR >> (pin)) & 0b01;
}


void gpio_Set(GPIO_TypeDef* gpio, int pin){

	gpio->BSRR |= (0b01 << pin);

	return;
}

void gpio_Reset(GPIO_TypeDef* gpio, int pin){

	gpio->BRR |= (0b01 << pin);

	return;
}

void gpio_Toggle(GPIO_TypeDef* gpio, int pin){

	if(gpio_Read(gpio,pin) == 0){
		gpio_Set(gpio, pin);
	}
	else{
		gpio_Reset(gpio, pin);
	}

	return;
}

