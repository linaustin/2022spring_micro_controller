#include "keypad_IT_function.h"

Keypad_TypeDef keypad_Gpio ={
		.out_Gpio[0] = GPIOB,
		.out_Pin[0] = 6,

		.out_Gpio[1] = GPIOA,
		.out_Pin[1] = 7,

		.out_Gpio[2] = GPIOA,
		.out_Pin[2] = 6,

		.out_Gpio[3] = GPIOA,
		.out_Pin[3] = 5,

		.in_Gpio[0] = GPIOB,
		.in_Pin[0] = 10,

		.in_Gpio[1] = GPIOA,
		.in_Pin[1] = 8,

		.in_Gpio[2] = GPIOA,
		.in_Pin[2] = 9,

		.in_Gpio[3] = GPIOC,
		.in_Pin[3] = 7
};

const int  keypad_Map[4][4] = {
		{1,2,3,10},
		{4,5,6,11},
		{7,8,9,12},
		{15,0,14,13}
};

int out_Pin_Index = 0;

int keypad_IT_Init(){
	system_Clock_Config(&sysclk_40Mhz);
	//output pin enable
	for(int i = 0; i < 4; i++){
		if(keypad_Gpio.out_Gpio[i] == GPIOA){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		}
		else if(keypad_Gpio.out_Gpio[i] == GPIOB){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		}
		else if(keypad_Gpio.out_Gpio[i] == GPIOC){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		}
		else{
			return -1;
		}
	}
	//output pin configure
	for(int i = 0; i < 4; i++){
		keypad_Gpio.out_Gpio[i]->MODER &= ~(0b11 << (2 * keypad_Gpio.out_Pin[i]));
		keypad_Gpio.out_Gpio[i]->MODER |= (0b01 << (2 * keypad_Gpio.out_Pin[i]));

		keypad_Gpio.out_Gpio[i]->OSPEEDR &= ~(0b11 << (2 * keypad_Gpio.out_Pin[i]));
		keypad_Gpio.out_Gpio[i]->OSPEEDR |= (0b11 << (2 * keypad_Gpio.out_Pin[i]));

		keypad_Gpio.out_Gpio[i]->OTYPER &= ~(0b01 << (keypad_Gpio.out_Pin[i]));
		keypad_Gpio.out_Gpio[i]->OTYPER |= (0b01 << (keypad_Gpio.out_Pin[i]));

		gpio_Set(keypad_Gpio.out_Gpio[i], keypad_Gpio.out_Pin[i]);
	}
	//input pin enable
	for(int i = 0; i < 4; i++){
		if(keypad_Gpio.in_Gpio[i] == GPIOA){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		}
		else if(keypad_Gpio.in_Gpio[i] == GPIOB){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		}
		else if(keypad_Gpio.in_Gpio[i] == GPIOC){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		}
		else{
			return -1;
		}
	}
	//input pin configure
	for(int i = 0; i < 4; i++){
		keypad_Gpio.in_Gpio[i]->MODER &= ~(0b11 << (2 * keypad_Gpio.in_Pin[i]));
		keypad_Gpio.in_Gpio[i]->MODER |= (0b00 << (2 * keypad_Gpio.in_Pin[i]));

		keypad_Gpio.in_Gpio[i]->PUPDR &= ~(0b11 << (2 * keypad_Gpio.in_Pin[i]));
		keypad_Gpio.in_Gpio[i]->PUPDR |= (0b10 << (2 * keypad_Gpio.in_Pin[i]));

		gpio_Reset(keypad_Gpio.in_Gpio[i], keypad_Gpio.in_Pin[i]);
	}

	for(int i = 0; i < 4; i++){
		keypad_EXTI_config(keypad_Gpio.in_Gpio[i], keypad_Gpio.in_Pin[i]);
	}

	NVIC_SetPriorityGrouping(0b101);	//set 2 group priorities 2 sub priorities

	NVIC_SetPriority(EXTI9_5_IRQn, 0b0000);
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	NVIC_SetPriority(EXTI15_10_IRQn, 0b0000);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	Timer_Init_Data keypad_Timer ={
			.PSC = (40*1000) - 1,
			.ARR = 100 - 1
	};

	timer_Init(TIM3, &keypad_Timer);
	timer_Start_IT(TIM3);

	NVIC_SetPriority(TIM3_IRQn, 0b0100);
	NVIC_EnableIRQ(TIM3_IRQn);

	return 0;
}

void keypad_EXTI_config(GPIO_TypeDef *gpio, int pin){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	//enable syscfgen
	//config EXTIx 's gpio
	if(gpio == GPIOA){
		SYSCFG->EXTICR[pin/4] &= ~(0b111 << ((pin%4)*4));
		SYSCFG->EXTICR[pin/4] |= (0b000 << ((pin%4)*4));
	}
	else if(gpio == GPIOB){
		SYSCFG->EXTICR[pin/4] &= ~(0b111 << ((pin%4)*4));
		SYSCFG->EXTICR[pin/4] |= (0b001 << ((pin%4)*4));
	}
	else if(gpio == GPIOC){
		SYSCFG->EXTICR[pin/4] &= ~(0b111 << ((pin%4)*4));
		SYSCFG->EXTICR[pin/4] |= (0b010 << ((pin%4)*4));
	}

	EXTI->IMR1 |= (0b01 << pin);	//disable interrupt request mask
	EXTI->RTSR1 |= (0b01 << pin);	//trigger by rising edge

	return;
}

uint16_t keypad_Read(Keypad_TypeDef* keypad_Data){
	uint16_t input = 0;
	int cnt[4][4];
	int scan_Cycle = 10;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			cnt[i][j] = 0;
		}
	}

	keypad_Data->out_Gpio[out_Pin_Index]->OTYPER |= (0b01 << (keypad_Data->out_Pin[out_Pin_Index]));

	for(int count = 0; count < scan_Cycle; count++){
		for(int j = 0; j < 4; j++){
			keypad_Data->out_Gpio[j]->OTYPER &= ~(0b01 << (keypad_Data->out_Pin[j]));

			for(int i = 0; i < 4; i++){
					cnt[i][j] += gpio_Read(keypad_Data->in_Gpio[i], keypad_Data->in_Pin[i]);
				}

			keypad_Data->out_Gpio[j]->OTYPER |= (0b01 << (keypad_Data->out_Pin[j]));
		}
	}

	for(int i = 0; i < 4; i ++){
		for(int j = 0; j < 4; j++){
			if(cnt[i][j] > 5){
				input |= (0b01 << (keypad_Map[i][j]));
			}
		}
	}

	return input;
}

void TIM3_IRQHandler(){
	TIM3->SR &= ~TIM_SR_UIF;

	keypad_Gpio.out_Gpio[out_Pin_Index]->OTYPER |= (0b01 << (keypad_Gpio.out_Pin[out_Pin_Index]));

	out_Pin_Index++;

	if(out_Pin_Index == 4){
		out_Pin_Index = 0;
	}

	keypad_Gpio.out_Gpio[out_Pin_Index]->OTYPER &= ~(0b01 << (keypad_Gpio.out_Pin[out_Pin_Index]));

	return;
}

void EXTI9_5_IRQHandler(){
	if((EXTI->PR1 >> 7) & 0b01){
		EXTI->PR1 |= (0b01 << 7);

		keypad_Input = keypad_Read(&keypad_Gpio);
	}
	else if((EXTI->PR1 >> 8) & 0b01){
		EXTI->PR1 |= (0b01 << 8);

		keypad_Input = keypad_Read(&keypad_Gpio);
	}
	else if((EXTI->PR1 >> 9) & 0b01){
		EXTI->PR1 |= (0b01 << 9);

		keypad_Input = keypad_Read(&keypad_Gpio);
	}

	return;
}

void EXTI15_10_IRQHandler(){
	if((EXTI->PR1 >> 10) & 0b01){
		EXTI->PR1 |= (0b01 << 10);

		keypad_Input = keypad_Read(&keypad_Gpio);
	}

	return;
}
