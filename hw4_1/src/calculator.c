#include "calculator.h"

int calculator_Init(Keypad_TypeDef* keypad_Data, Seg_TypeDef* seg_Data){
	if(init_Keypad(keypad_Data) != 0 || init_7seg(seg_Data) != 0){
		return -1;
	}

	calculator_Clean(seg_Data);

	return 0;
}

int calculator_Interrupt(Keypad_TypeDef* keypad_Data){
	uint16_t keypad_Input = read_Keypad(keypad_Data);

	if((keypad_Input >> 14) & 0x01){
		return 1;
	}
	else{
		return 0;
	}
}

void calculator_Clean(Seg_TypeDef* seg_Data){
	for(int i = 0; i < 3; i++){
		input_Buffer[i] = -1;
	}

	for(int i = 0; i < 8; i++){
		display_Data[i] = -1;
	}

	for(int i = 0; i < 50; i++){
		command_Buffer[i] = -1;
	}

	display_Data[0] = 0;
	command_Pointer = 0;
	error_Flag = 0;
	clean_Flag = 0;
	display_Push_Flag = 0;

	calculator_Display(seg_Data);
	return;
}

void calculator_Run(Keypad_TypeDef* keypad_Data, Seg_TypeDef* seg_Data){
	uint16_t keypad_Input;
	uint8_t count;

	while(!((keypad_Input >> 15) & 0x01)){
		keypad_Input = read_Keypad(keypad_Data);
		for(int i = 0; i < 16; i ++){
			if((keypad_Input >> i) & 0x01){
				if(i < 10){
					for(int j = 2; j > 0; j--){
						input_Buffer[j] = input_Buffer[j - 1];
					}

					input_Buffer[0] = i;

					calculator_Display(seg_Data);

					break;
				}
				else if(i > 9 && i < 14){
					count = 0;
					for(int j = 0; j < 3; j++){
						if(input_Buffer[j] < 0){
							input_Buffer[j] = 0;
							count++;
						}
					}

					if(count == 3){
						error_Flag = 1;
						return;
					}

					display_Push_Flag = 4;
					display_Operator = i;
					command_Buffer[command_Pointer + 1] = (-1 * i);
					command_Buffer[command_Pointer] = (input_Buffer[2]*100) + (input_Buffer[1]*10) + (input_Buffer[0]);
					command_Pointer = command_Pointer + 2;
					if(command_Pointer == 52){
						error_Flag = 1;
						return;
					}
					calculator_Display(seg_Data);

					for(int j = 0; j < 3; j++){
						input_Buffer[j] = -1;
					}

					break;
				}
				else if(i == 14){
					clean_Flag = 1;
					return;
				}
				else if(i == 15){
					for(int j = 0; j < 3; j++){
						if(input_Buffer[j] < 0){
							input_Buffer[j] = 0;
						}
					}

					command_Buffer[command_Pointer + 1] = (-1 * i);
					command_Buffer[command_Pointer] = (input_Buffer[2]*100) + (input_Buffer[1]*10) + (input_Buffer[0]);
					command_Pointer = command_Pointer + 2;
					if(command_Pointer == 52){
						error_Flag = 1;
						return;
					}
				}
			}
		}

		delay(50);
	}
	return;
}

void calculator_Display(Seg_TypeDef* seg_Data){
	if(display_Push_Flag != 0){
		for(int i = 7; i > 0; i--){
			display_Data[i] = display_Data[i - 1];
		}

		if(display_Operator != 0){
			display_Data[0] = display_Operator;
			display_Operator = 0;
		}
		else{
			display_Data[0] = input_Buffer[0];
		}

		display_Push_Flag--;
	}
	else{
		for(int i = 0; i < 3; i++){
			display_Data[i] = input_Buffer[i];
		}
	}

	for(int i = 0; i < 8; i++){
		if(display_Data[i] > 9){
			send_7seg_Int(seg_Data, i, display_Data[i], 1);
		}
		else{
			send_7seg_Int(seg_Data, i, display_Data[i], 0);
		}
	}
	return;
}

int calculator_Cal(int* command){
	int ans = 0;
	int scanner[3];
	uint8_t read_Pointer;
	scanner[0] = -1;
	scanner[1] = -1;
	scanner[2] = -1;

	for(int i = 0; i < 50; i++){
		command[i] = command[i]*1000;
	}

	for(int i = 0; i < 50; i++){
		if(command[i] == (-12*1000)){
			scanner[1] = command[i];
			command[i] = -1;

			for(int j = i -1; j > -1; j--){
				if(command[j] < -1){
					error_Flag = 1;
					return -1;
				}
				else if(command[j] > -1){
					scanner[0] = command[j];
					command[j] = -1;
					read_Pointer = j;
					break;
				}
			}

			for(int j = i + 1; j < 50; j++){
				if(command[j] < -1){
					error_Flag = 1;
					return -1;
				}
				else if(command[j] > -1){
					scanner[2] = command[j];
					command[j] = -1;
					break;
				}
			}

			scanner[0] = (scanner[0]/1000)*(scanner[2]/1000)*1000;
			command[read_Pointer] = scanner[0];
			scanner[0] = -1;
			scanner[1] = -1;
			scanner[2] = -1;
		}
		else if(command[i] == (-13*1000)){
			scanner[1] = command[i];
			command[i] = -1;

			for(int j = i -1; j > -1; j--){
				if(command[j] < -1){
					error_Flag = 1;
					return -1;
				}
				else if(command[j] > -1){
					scanner[0] = command[j];
					command[j] = -1;
					read_Pointer = j;
					break;
				}
			}

			for(int j = i + 1; j < 50; j++){
				if(command[j] < -1){
					error_Flag = 1;
					return -1;
				}
				else if(command[j] > -1){
					scanner[2] = command[j];
					command[j] = -1;

					if(scanner[2] == 0){
						error_Flag = 1;
						return -1;
					}

					break;
				}
			}

			scanner[0] = scanner[0]/(scanner[2]/1000);
			command[read_Pointer] = scanner[0];
			scanner[0] = -1;
			scanner[1] = -1;
			scanner[2] = -1;

		}
		else if(command[i] == (-15*1000)){
			break;
		}
	}

	scanner[0] = command[0];
	read_Pointer = 1;

	for(int i = 1; i < 50; i++){
		if(command[i] != -1){
			scanner[read_Pointer] = command[i];
			command[i] = -1;
			read_Pointer++;
		}

		if(scanner[1] == -15){
			break;
		}

		if(read_Pointer == 3){
			read_Pointer = 1;

			if(scanner[1] == (-10*1000)){
				scanner[0] = scanner[0] + scanner[2];
			}
			else if(scanner[1] == (-11*1000)){
				scanner[0] = scanner[0] - scanner[2];
			}

			scanner[1] = 0;
			scanner[2] = 0;
		}
	}

	ans = scanner[0];

	return ans;
}
