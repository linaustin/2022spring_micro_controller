#include "timer_function.h"

void timer_Init(TIM_TypeDef *tim, Timer_Init_Data *timer_Data){
	tim->CR1 |= TIM_CR1_ARPE;	//enable auto-reload preload
	tim->ARR = (uint32_t)timer_Data->ARR;
	tim->PSC = (uint32_t)timer_Data->PSC;
	tim->EGR |= TIM_EGR_UG;

	return;
}

void timer_PWM_Init(TIM_TypeDef *tim, Timer_PWM_Init_Data *timer_Data){
	if((tim == TIM2) && (timer_Data->channel == 1)){
		tim->CR1 |= TIM_CR1_ARPE;
		tim->ARR = (uint32_t)timer_Data->ARR;
		tim->PSC = (uint32_t)timer_Data->PSC;
		tim->CCR1 = (uint32_t)timer_Data->CCRx;

		gpio_AF_Init(GPIOA, 0, 0b0001); // TIM2_CH1 = PA0(AF1)  init PA0 AF1 function

		tim->CCMR1 &= ~TIM_CCMR1_OC1PE_Msk;
		tim->CCMR1 |= TIM_CCMR1_OC1PE_Msk; // enable TIM_CCR1 preload

		tim->CCMR1 &= ~(TIM_CCMR1_OC1M_Msk);
		tim->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); //set channel 1 to PWM mode 1    when TIM_CNT < TIM_CCR channel is active

		tim->CCER &= ~TIM_CCER_CC1P_Msk;
		tim->CCER |= (0 << TIM_CCER_CC1P_Pos);	//set active high for channel 1 polarity

		tim->CCER &= ~TIM_CCER_CC1E_Msk;
		tim->CCER |= (0 << TIM_CCER_CC1E_Pos); // disable channel 1 output
	}

	tim->EGR |= TIM_EGR_UG;

	return;
}

void timer_Enable(TIM_TypeDef *tim){
	if(tim == TIM2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	}
	else if(tim == TIM3){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	}
	else if(tim == TIM4){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	}
	else if(tim == TIM5){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	}
	else if(tim == TIM15){
		RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	}

	return;
}

void timer_Disable(TIM_TypeDef *tim){
	if(tim == TIM2){
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;
	}
	else if(tim == TIM3){
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM3EN;
	}
	else if(tim == TIM4){
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM4EN;
	}
	else if(tim == TIM5){
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM5EN;
	}

	return;
}

void timer_Reload(TIM_TypeDef *tim){
	tim->EGR |= TIM_EGR_UG;
	return;
}

void timer_Start(TIM_TypeDef *tim){
	tim->CR1 |= TIM_CR1_CEN;
	return;
}

void timer_Start_IT(TIM_TypeDef *tim){
	tim->DIER |= TIM_DIER_UIE;
	tim->CR1 |= TIM_CR1_CEN;

	return;
}

void timer_Stop(TIM_TypeDef *tim){
	tim->CR1 &= ~TIM_CR1_CEN;
	return;
}

void timer_Stop_IT(TIM_TypeDef *tim){
	tim->CR1 &= ~TIM_CR1_CEN;
	tim->DIER &= ~TIM_DIER_UIE;

	return;
}

void timer_Channel_Start(TIM_TypeDef *tim, int channel){
	if((tim == TIM2) && (channel == 1)){
		tim->CCER |= (1 << TIM_CCER_CC1E_Pos);
	}

	return;
}

void timer_Channel_Stop(TIM_TypeDef *tim, int channel){
	if((tim == TIM2) && (channel == 1)){
		tim->CCER &= ~TIM_CCER_CC1E_Msk;
	}

	return;
}





