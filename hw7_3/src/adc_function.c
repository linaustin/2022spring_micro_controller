#include "adc_function.h"

void adc_Init(ADC_TypeDef *adc, ADC_Init_Data *adc_Data){
	//enable adc common clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	//configure adc gpio channel
	if((adc == ADC1) && (adc_Data->channel == 1)){
		gpio_Analog_Init(GPIOC, 0);
	}

	//configure adc resolution
	adc->CFGR &= ~ADC_CFGR_RES;
	adc->CFGR |= adc_Data->resolution << ADC_CFGR_RES_Pos;

	//congigure adc continues
	if(adc_Data->continus){
		adc->CFGR |= ADC_CFGR_CONT;
	}
	else{
		adc->CFGR &= ~ADC_CFGR_CONT;
	}

	//configure adc align
	adc->CFGR &= ~ADC_CFGR_ALIGN;
	adc->CFGR |= adc_Data->align << ADC_CFGR_ALIGN_Pos;

	//configure adc common dual mode
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	ADC123_COMMON->CCR |= adc_Data->common_Dual_Mode << ADC_CCR_DUAL_Pos;

	//configure adc common clock mode
	ADC123_COMMON->CCR &= ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= adc_Data->common_Clock_Mode << ADC_CCR_CKMODE_Pos;

	//configure adc common dma mode
	ADC123_COMMON->CCR &= ~ADC_CCR_MDMA;
	ADC123_COMMON->CCR |= adc_Data->common_DMA_Mode << ADC_CCR_MDMA_Pos;

	//configure adc common delay
	ADC123_COMMON->CCR &= ~ADC_CCR_DELAY;
	ADC123_COMMON->CCR |= 0b0000 << ADC_CCR_DELAY_Pos; //set delay to 1 cycle

	//configure adc regular sequence to 1 conversion
	adc->SQR1 &= ~ADC_SQR1_L;
	adc->SQR1 |= 0b0000 << ADC_SQR1_L_Pos;

	//configure adc regular sequence 1 channel
	adc->SQR1 &= ~ADC_SQR1_SQ1;
	adc->SQR1 |= adc_Data->channel << ADC_SQR1_SQ1_Pos;

	//configure adc sampling time
	if(adc_Data->channel < 10){
		adc->SMPR1 &= ~(ADC_SMPR1_SMP0 << (adc_Data->channel*3));
		adc->SMPR1 |= (adc_Data->sample_Time << (adc_Data->channel*3));
	}
	else{
		int shift = adc_Data->channel - 10;
		adc->SMPR2 &= ~(ADC_SMPR2_SMP10 << (shift*3));
		adc->SMPR2 |= (adc_Data->sample_Time << (shift*3));
	}

	//wakeup adc
	adc->CR &= ~ADC_CR_DEEPPWD;
	adc->CR |= ADC_CR_ADVREGEN;
	tim_Delay_us(20);

	return;
}

void adc_Enable(ADC_TypeDef *adc){
	adc->CR |= ADC_CR_ADEN;

	while(!(ADC1->ISR & ADC_ISR_ADRDY));

	return;
}

void adc_Disable(ADC_TypeDef *adc){
	adc->CR &= ~ADC_CR_ADEN;
	return;
}

void adc_Start(ADC_TypeDef *adc){
	adc->CR |= ADC_CR_ADSTART;
	return;
}

void adc_Start_IT(ADC_TypeDef *adc){
	adc->IER |= ADC_IER_EOCIE;
	adc->CR |= ADC_CR_ADSTART;

	return;
}

void adc_Stop(ADC_TypeDef *adc){
	adc->CR &= ~ADC_CR_ADSTART;
	return;
}

void adc_Stop_IT(ADC_TypeDef *adc){
	adc->CR &= ~ADC_CR_ADSTART;
	adc->IER &= ~ADC_IER_EOCIE;

	return;
}

uint16_t adc_Get_Value(ADC_TypeDef *adc){
	adc->ISR &= ~ADC_ISR_EOC;
	return adc->DR;
}
