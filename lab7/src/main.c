#include "stm32l476xx.h"
#include "helper_functions.h"
#include "usart.h"
#include "adc.h"
#include "led_button.h"
#include "7seg.h"
#include <stdio.h>

#define lab10_2
#define SEG_gpio GPIOC
#define DIN_pin 3
#define CS_pin 4
#define CLK_pin 5
double resistor;
int adcVal = 0;

void GPIO_init(){
	// Setup FPU
	SCB->CPACR |= (0xF << 20);
	__DSB();
	__ISB();

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   // PA5 LED
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;   // PC13 Button
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;     // ADC

	// Setup Button
	GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
	GPIOC->MODER |= (0x0 << GPIO_MODER_MODE13_Pos);

	// Setup PA5 LED
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOA->MODER |= (0x1 << GPIO_MODER_MODE5_Pos);


	// PC0 ADC
	GPIOC->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOC->MODER |= (0x3 << GPIO_MODER_MODE0_Pos);
	GPIOC->ASCR |= GPIO_ASCR_ASC0;

}

void EXTI_Setup(){
	// Enable SYSCFG CLK
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Select output bits
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk;
	SYSCFG->EXTICR[3] |= (2 << SYSCFG_EXTICR4_EXTI13_Pos);
	// Enable interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	// Enable Falling Edge
	EXTI->RTSR1 |= EXTI_RTSR1_RT13;
	// Enable NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void ADCInit(){
	ADCResolution(ADC1, 0);              // 12 bits
	ADCContinuousConversion(ADC1, 0);    // enable continuous conversion
	ADCDataAlign(ADC1, 0);               // set right align
	ADCCommonDualMode(0);                // independent mode
	ADCCommonClockMode(1);               // hclk / 1
	ADCCommonPrescaler(0);               // div 1
	ADCCommonDMAMode(0);                 // disable dma
	ADCCommonDelayTwoSampling(0b0100);   // 5 adc clk cycle
	ADCChannel(ADC1, 1, 1, 2);           // channel 1, rank 1, 12.5 adc clock cycle
	ADCWakeup(ADC1);
	ADCInterrupt(ADC1, ADC_IER_EOCIE, 1);
	NVIC_EnableIRQ(ADC1_2_IRQn);
	ADCEnable(ADC1);
}

void EXTI15_10_IRQHandler(){
	if(EXTI->PR1 & EXTI_PR1_PIF13_Msk){

	#ifdef lab10_1
		USARTPrintString(USART1, "Hello World!");
		USARTPrintString(USART1, "\r\n");
	#endif

	#ifdef lab10_2
		char s[20];
		sprintf(s, "%d", resistor);
		USARTPrintString(USART1, s);
		USARTPrintString(USART1, "\r\n");
	#endif

		delay_ms(100);
		EXTI->PR1 = EXTI_PR1_PIF13_Msk;
	}
}

void SysTick_Handler() {
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		ADCStartConversion(ADC1);
	}
	return;
}

void ADC1_2_IRQHandler(){
	if(ADC1->ISR & ADC_ISR_EOC){
		ADC1->ISR &= ADC_ISR_EOC;
		resistor = ADCGetValue(ADC1);
		resistor = (3.3 / 4095) * resistor;
		resistor = (3.3 - resistor) * 2000 / resistor;
	}
}

void TIM2_IRQHandler(){
	if(TIM2->SR & TIM_SR_UIF){

		char s[20];
		USARTPrintString(USART1, "\33[2K\r");
		sprintf(s, "%d", resistor);
		USARTPrintString(USART1, s);

		TIM2->SR &= ~(TIM_SR_UIF);
	}
}
void startADC() {
	while (!(ADC1->ISR & ADC_ISR_ADRDY)) ADC1->CR |= ADC_CR_ADEN; // TURN ON
	delay_us(5000);
	ADC1->ISR = ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR; // Clear flags
	ADC1->CR |= ADC_CR_ADSTART; // START CONV

	}

void light() {
	startADC();
	while (!(ADC1->ISR & ADC_ISR_EOC));
	adcVal = ADC1->DR;
}


void lab_10_2(){
	EXTI_Setup();
//	USARTInit();
	ADCInit();
	delay_ms(100);
	SystemClock_Config_Interrupt(4, 1000000);
	delay_ms(500);
	init_led(GPIOA,5);
	while(1){
		light();
		ADC1_2_IRQHandler();
		EXTI15_10_IRQHandler();
		TIM2_IRQHandler();
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT_0, adcVal%10);
		adcVal/=10;
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT_1, adcVal%10);
		adcVal/=10;
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT_2, adcVal%10);
		adcVal/=10;
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT_3, adcVal%10);
		adcVal/=10;
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT_4, adcVal%10);
		adcVal/=10;
	}




}


int main(){

	GPIO_init();
	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
					return -1;
				}
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, 0xFF);
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0x04);
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, 0x01);

	lab_10_2();
	while(1){}
	return 0;
}

