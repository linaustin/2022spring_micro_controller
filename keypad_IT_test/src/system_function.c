#include "system_function.h"

void FPU_Init(){
	// Setup FPU
	SCB->CPACR |= (0xF << 20);
	__DSB();
	__ISB();
}

void delay_Loop(int msec){
	int loop_cnt = 500*msec;

	while(loop_cnt){
		loop_cnt--;
	}

	return;
}

void system_Clock_Config(Sysclk_Data *sysclk_Data){

	RCC->CR |= (0b01 << 0); //enable MSI;

	while(!((RCC->CR >> 1) & 0b01)); //check MSI enable ready

	RCC->CFGR &= ~(0b11 << 0); //clear CFGR SW
	RCC->CFGR |= (0b00 << 0); //set CFGR SW to MSI

	while(!(((RCC->CFGR >> 2) & 0b11) == 0b00)); //check CFGR SWS MSI ready

	RCC->CR &= ~(0b01 << 24); //disable PLL;

	while(((RCC->CR >> 25) & 0b01)); //check PLL disable ready

	RCC->PLLCFGR &= ~(0b11 << 0); //clear PLLCFGR�@SRC
	RCC->PLLCFGR |= (0b01 << 0); //set PLLSRC MSI

	RCC->PLLCFGR &= ~(0b111 << 4); //clear PLLCFGR PLLM
	RCC->PLLCFGR |= ((sysclk_Data->pllm) << 4); //set PLLCFGR PLLM

	RCC->PLLCFGR &= ~(0b1111111 << 8); //clear PLLCFGR PLLN;
	RCC->PLLCFGR |= ((sysclk_Data->plln) << 8); //set PLLCFGR PLLN;

	RCC->PLLCFGR &= ~(0b11 << 25); //clear PLLCFGR PLLR
	RCC->PLLCFGR |= ((sysclk_Data->pllr) << 25); //set PLLCFGR PLLR

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN; // enable PLLR

	RCC->CR &= ~(0b01 << 24); //clear RCCCR PLL
	RCC->CR |= (0b01 << 24); //enable PLL

	while(!((RCC->CR >> 25) & 0b01)); //check PLL enable

	RCC->CFGR &= ~(0b11 << 0); //clear CFGR SW
	RCC->CFGR |= (0b11 << 0); //set CFGR SW to PLL

	while(!(((RCC->CFGR >> 2) & 0b11) == 0b11)); //check CFGR SWS PLL ready

	return;
}


void system_Tick_Start_IT(uint32_t reload){
	SysTick->LOAD = reload;			//only 24 bits valid
	SysTick->CTRL |= (0b01 << 2);	//set systick clock source as FCLK
	SysTick->CTRL |= (0b01 << 1);	//enable systick interrupt
	SysTick->CTRL |= (0b01 << 0);	//enable systick

	NVIC_SetPriorityGrouping(0b101);
	NVIC_SetPriority(SysTick_IRQn, 0b0000);
	NVIC_EnableIRQ(SysTick_IRQn);

	return;
}

void system_Tick_Stop_IT(){
	SysTick->CTRL &= ~(0b01<< 1);	//enable systick interrupt

	return;
}

void tim_Delay_us(int usec){
	system_Clock_Config(&sysclk_40Mhz);
	timer_Enable(TIM15);

	Timer_Init_Data tim_Data = {
			.PSC = 40 - 1,
			.ARR = usec - 1
	};

	timer_Init(TIM15, &tim_Data);
	timer_Start(TIM15);

	while(!(TIM15->SR & TIM_SR_UIF));

	timer_Stop(TIM15);

	return;
}

void EXTI_Config(GPIO_TypeDef *gpio, int pin){
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

void __attribute__((weak))SysTick_Handler(){
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
	}

	return;
}

void __attribute__((weak))EXTI9_5_IRQHandler(){
	if((EXTI->PR1 >> 7) & 0b01){
		EXTI->PR1 |= (0b01 << 7);
	}
	else if((EXTI->PR1 >> 8) & 0b01){
		EXTI->PR1 |= (0b01 << 8);
	}
	else if((EXTI->PR1 >> 9) & 0b01){
		EXTI->PR1 |= (0b01 << 9);
	}


	return;
}

void __attribute__((weak))EXTI15_10_IRQHandler(){
	if((EXTI->PR1 >> 10) & 0b01){
		EXTI->PR1 |= (0b01 << 10);
	}

	return;
}

// sysclk = source * plln/2* (pllr + 1) * (pllm + 1)   source MSI = 4Mhz

Sysclk_Data sysclk_1Mhz = {
	.plln = 8,
	.pllr = 3,
	.pllm = 3
};

Sysclk_Data sysclk_6Mhz = {
	.plln = 12,
	.pllr = 0,
	.pllm = 3
};

Sysclk_Data sysclk_10Mhz = {
	.plln = 10,
	.pllr = 1,
	.pllm = 0
};

Sysclk_Data sysclk_16Mhz = {
	.plln = 8,
	.pllr = 0,
	.pllm = 0
};

Sysclk_Data sysclk_20Mhz = {
	.plln = 20,
	.pllr = 1,
	.pllm = 0
};

Sysclk_Data sysclk_40Mhz = {
	.plln = 40,
	.pllr = 1,
	.pllm = 0
};
