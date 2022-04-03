.syntax unified
.cpu cortex-m4
.thumb

.text
	.global main
    .include "../src/pin.s"


 // R0 address of target register
 // R1 value of register


main:
	// enable GPIOC
	LDR R0, =RCC_AHB2ENR;
	LDR R1, [R0]
	AND R1, #0xFB
	ORR R1, #0x04
	STR R1, [R0]

	// setting GPIOC_MODER
	LDR R0, =GPIOC_MODER
	LDR R1, [R0]
	AND R1, #0xFFFFF00F
	ORR R1, #0x00000550
	STR R1, [R0]

	// setting GPIOC_OTYPER to push-pull
	LDR R0, =GPIOC_OTYPER
	LDR R1, [R0]
	AND R1, #0xFFFFFFC3
	ORR R1, #0x00000000
	STR R1, [R0]

	// setting GPIOC_ODR to open pc2~pc5
	LDR R0, =GPIOC_ODR
	LDR R1, [R0]
	AND R1, #0xFFFFFFC3
	ORR R1, #0x0000002C
	STR R1, [R0]



