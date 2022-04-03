#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"

// Define pins for 7seg
#define SEG_gpio GPIOC
#define DIN_pin 3
#define CS_pin 4
#define CLK_pin 5

// Define pins for keypad
#define COL_gpio GPIOA
#define COL_pin 5       // 5 6 7 8
#define ROW_gpio GPIOB
#define ROW_pin 3       // 3 4 5 6

// Use to decide which part of the code will run
// Use define & ifdef to control
#define lab_keypad_single_key
//#define lab_keypad_multiple_key
//#define lab_keypad_calculator

int main(){

#ifdef lab_keypad_single_key

	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
		// Fail to init 7seg
		return -1;
	}

	// Set Decode Mode to Code B decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, 0xFF);
	// Set Scan Limit to all digits
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0x07);
	// Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, 0x01);

	if(init_keypad(ROW_gpio, COL_gpio, ROW_pin, COL_pin) != 0){
		// Fail to init keypad
		return -1;
	}

	while(1){
		int input = 0;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				if(check_keypad_input_one(ROW_gpio, COL_gpio, ROW_pin, COL_pin, i, j)){
					input = 1;
					display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, keypad[i][j], num_digits(keypad[i][j]));
				}
			}
		}
		if(input == 0){
			display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 0, 0);
		}
	}

#endif

	while(1){}

	return 0;
}
