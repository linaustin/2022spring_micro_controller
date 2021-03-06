#include "music.h"

int music_Duty_Cycle = 50;

void music_Init(){

	Timer_PWM_Init_Data timer_PWM_Data = {
			.channel = 1,
			.PSC = (40 - 1),
			.ARR = 100,
			.CCRx = 50,
	};

	timer_Enable(TIM2);
	timer_PWM_Init(TIM2, &timer_PWM_Data);
	timer_Reload(TIM2);
	timer_Start(TIM2);

	return;
}

void music_Play(int note){
	TIM2->ARR = note_Arr[note - 1];
	TIM2->CCR1 = (note_Arr[note - 1]*music_Duty_Cycle)/100;

	timer_Channel_Start(TIM2, 1);

	return;
}

void music_Read_Keypad(uint16_t keypad_Input, Seg_TypeDef *seg_Data){
	if(keypad_Input == 0){
		timer_Channel_Stop(TIM2, 1);
		display_Number_Float2(seg_Data, music_Duty_Cycle*100);
		send_7seg_Int(seg_Data, 7, -1, 0);
	}
	else{
		for(int i = 0; i < 16; i++){
			if ((keypad_Input >> i) & 0b01){
				switch(i){
					case 1:
						music_Play(1);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 2:
						music_Play(2);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 3:
						music_Play(3);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 4:
						music_Play(4);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 5:
						music_Play(5);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 6:
						music_Play(6);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 7:
						music_Play(7);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 8:
						music_Play(8);
						display_Number_Float2(seg_Data, music_Duty_Cycle*100);
						send_7seg_Int(seg_Data, 7, i, 0);
						break;

					case 10:
						music_Duty_Cycle = music_Duty_Cycle + 5;

						if(music_Duty_Cycle == 100){
							music_Duty_Cycle = 95;
						}

						display_Number_Float2(seg_Data, music_Duty_Cycle*100);

						break;

					case 11:
						music_Duty_Cycle = music_Duty_Cycle - 5;

						if(music_Duty_Cycle == 0){
							music_Duty_Cycle = 5;
						}

						display_Number_Float2(seg_Data, music_Duty_Cycle*100);

						break;

					default:
						break;
				}
				break;
			}
		}
	}

	return;
}

uint32_t note_Arr[8] = {1912, 1704, 1517, 1433, 1276, 1136, 1012, 956};
