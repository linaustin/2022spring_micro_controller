#include "clock.h"

Seg_TypeDef seg_Gpio = {
	.gpio = GPIOC,
	.DIN = 1,
	.CS = 2,
	.CLK = 3
};

int count_Time[3] = {0, 0, 0};

int clock_Init(){
	system_Clock_Config(&sysclk_40Mhz);

	if(keypad_IT_Init() != 0){
		return -1;
	}

	if(init_7seg(&seg_Gpio) != 0){
		return -1;
	}

	if(button_Init(GPIOC, 13) != 0){
		return -1;
	}

	NVIC_SetPriority(TIM4_IRQn, 0b1000);
	NVIC_EnableIRQ(TIM4_IRQn);

	Timer_Init_Data timer_Display = {
			.PSC = 40*1000 - 1,
			.ARR = 100 - 1
	};

	timer_Enable(TIM4);
	timer_Init(TIM4, &timer_Display);
	timer_Start_IT(TIM4);

	return 0;
}

void clock_Config(){
	int input_Key;
	uint8_t button_Flag = 0;
	uint8_t count_Time_Index = 0;

	while(!button_Flag){
		input_Key = keypad_Read();

		switch(input_Key){

		case 0:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10;
			break;

		case 1:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 1;
			break;

		case 2:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 2;
			break;

		case 3:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 3;
			break;

		case 4:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 4;
			break;

		case 5:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 5;
			break;

		case 6:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 6;
			break;

		case 7:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 7;
			break;

		case 8:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 8;
			break;

		case 9:
			count_Time[count_Time_Index] = count_Time[count_Time_Index]%10;
			count_Time[count_Time_Index] = count_Time[count_Time_Index]*10 + 9;
			break;

		case 10:
			count_Time_Index++;
			if(count_Time_Index == 3){
				count_Time_Index = 2;
			}
			break;

		case 11:
			count_Time_Index--;
			if(count_Time_Index == -1){
				count_Time_Index = 0;
			}
			break;

		default:

			break;
		}

		button_Flag = button_Read_RisingEdge(GPIOC, 13, 0);
	}

	return;
}

void clock_Counting(){

	Timer_Init_Data timer_Counting = {
			.PSC = 40*1000 -1,
			.ARR = 1000 - 1
	};

	NVIC_SetPriority(TIM5_IRQn, 0b00000);
	NVIC_EnableIRQ(TIM5_IRQn);

	timer_Enable(TIM5);
	timer_Init(TIM5, &timer_Counting);
	timer_Start_IT(TIM5);

	while((count_Time[2] != 0) || (count_Time[1] != 0) || (count_Time[0] != 0));

	return;
}

void clock_Alarming(){

	Timer_PWM_Init_Data timer_PWM = {
			.channel = 1,
			.PSC = (40 -1),
			.ARR = 1912,
			.CCRx = ((1912/4)*3)                                                                                                                                                                                                                                                                                                                                                                                                 ,
	};

	timer_Enable(TIM2);
	timer_PWM_Init(TIM2, &timer_PWM);
	timer_Reload(TIM2);
	timer_Start(TIM2);
	timer_Channel_Start(TIM2, 1);

	uint8_t button_Flag = 0;

	while(!button_Flag){
		button_Flag = button_Read_RisingEdge(GPIOC, 13, 0);
	}

	timer_Channel_Stop(TIM2, 1);
	timer_Stop(TIM2);

	return;
}

void clock_Display(int *time_Data){
	send_7seg_Int(&seg_Gpio, 0, time_Data[0]%10, 0);
	send_7seg_Int(&seg_Gpio, 1, time_Data[0]/10, 0);
	send_7seg_Char(&seg_Gpio, 2, '-');

	send_7seg_Int(&seg_Gpio, 3, time_Data[1]%10, 0);
	send_7seg_Int(&seg_Gpio, 4, time_Data[1]/10, 0);
	send_7seg_Char(&seg_Gpio, 5, '-');

	send_7seg_Int(&seg_Gpio, 6, time_Data[2]%10, 0);
	send_7seg_Int(&seg_Gpio, 7, time_Data[2]/10, 0);

	return;
}

void TIM4_IRQHandler(){
	TIM4->SR &= ~TIM_SR_UIF;
	clock_Display(count_Time);

	return;
}

void TIM5_IRQHandler(){
	TIM5->SR &= ~TIM_SR_UIF;

	if(count_Time[0]  != 0){
		count_Time[0]-= 1;
		return;
	}
	else{
		if(count_Time[1] != 0){
			count_Time[1]-= 1;
			count_Time[0] = 59;
			return;
		}
		else{
			if(count_Time[2] != 0){
				count_Time[2]-= 1;
				count_Time[1] = 59;
				count_Time[0] = 59;
				return;
			}
			else{
				timer_Stop_IT(TIM5);
				return;
			}
		}
	}
}
