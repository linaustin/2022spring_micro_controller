#include "adc.h"
#include "helper_functions.h"

void ADCResolution(ADC_TypeDef *adc, int resolution){
	// 0 -> 12
	// 1 -> 11
	// 2 -> 10
	// 3 -> 11
	adc->CFGR &= ~ADC_CFGR_RES;
	adc->CFGR |= resolution << ADC_CFGR_RES_Pos;
}

void ADCContinuousConversion(ADC_TypeDef *adc, int enable){
	if(enable){
		adc->CFGR |= ADC_CFGR_CONT;
	}else{
		adc->CFGR &= ~ADC_CFGR_CONT;
	}
}

void ADCDataAlign(ADC_TypeDef *adc, int dir){
	// 0 -> right
	// 1 -> left
	if(dir == 0){
		adc->CFGR &= ~ADC_CFGR_ALIGN;
	}else{
		adc->CFGR |= ADC_CFGR_ALIGN;
	}
}

void ADCWakeup(ADC_TypeDef *adc){
	adc->CR &= ~ADC_CR_DEEPPWD;
	adc->CR |= ADC_CR_ADVREGEN;
	delay_us(20);
}

void ADCEnable(ADC_TypeDef *adc){
	adc->CR |= ADC_CR_ADEN;
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
}

int ADCGetValue(ADC_TypeDef *adc){
	return adc->DR;
}

void ADCStartConversion(ADC_TypeDef *adc){
	adc->CR |= ADC_CR_ADSTART;
}

void ADCInterrupt(ADC_TypeDef *adc, int it, int enable){
	if(enable){
		adc->IER |= it;
	}else{
		adc->IER &= ~it;
	}
}

void ADCCommonDualMode(int mode){
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	ADC123_COMMON->CCR |= mode << ADC_CCR_DUAL_Pos;
}

void ADCCommonClockMode(int clkMode){
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= clkMode << ADC_CCR_CKMODE_Pos;
}

void ADCCommonPrescaler(int psc){
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	ADC123_COMMON->CCR |= psc << ADC_CCR_PRESC_Pos;
}

void ADCCommonDMAMode(int mode){
	ADC123_COMMON->CCR &= ~ADC_CCR_MDMA;
	ADC123_COMMON->CCR |= mode << ADC_CCR_MDMA_Pos;
}

void ADCCommonDelayTwoSampling(int clk){
	ADC123_COMMON->CCR &= ~ADC_CCR_DELAY;
	ADC123_COMMON->CCR |= clk << ADC_CCR_DELAY_Pos;
}

void ADCChannel(ADC_TypeDef *adc, int channel, int rank, int sampleTime){
	if(rank <= 4){
		adc->SQR1 &= ~(ADC_SQR1_SQ1 << (rank * 6));
		adc->SQR1 |= (channel << (rank * 6));
	}
	if(channel <= 9){
		adc->SMPR1 &= ~(ADC_SMPR1_SMP0 << (channel * 3));
		adc->SMPR1 |= (sampleTime << (channel * 3));
	}
}
