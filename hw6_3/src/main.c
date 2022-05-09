#include "stm32l476xx.h"
#include "clock.h"

int main(){

	if(clock_Init() != 0){
		return -1;
	}

	while(1){
		clock_Config();
		clock_Counting();
		clock_Alarming();
	}

	return 0;
}

