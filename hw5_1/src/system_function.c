#include "system_function.h"

void FPU_init(){
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
