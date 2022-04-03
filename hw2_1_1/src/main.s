.syntax unified
.cpu cortex-m4
.thumb

.text
	.global main
    .include "../src/pin.s"


 // R0 address of target register
 // R1 value of register


main:
	// enable GPIOB
	LDR R0, =RCC_AHB2ENR;
	LDR R1, [R0]
	AND R1, #0xFD
	ORR R1, #0x02
	STR R1, [R0]

	// setting GPIOB_MODER
	LDR R0, =GPIOB_MODER
	LDR R1, [R0]
	AND R1, #0xFFFFC03F
	ORR R1, #0x00001540
	STR R1, [R0]

	// setting GPIOB_OTYPER to push-pull
	LDR R0, =GPIOB_OTYPER
	LDR R1, [R0]
	AND R1, #0xFFFFFF87
	ORR R1, #0x00000000
	STR R1, [R0]

	// setting GPIOB_ODR to open pb3~pb6
	LDR R0, =GPIOB_ODR
	LDR R1, [R0]
	AND R1, #0xFFFFFF87
	ORR R1, #0x00000058
	STR R1, [R0]



