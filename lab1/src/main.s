	.syntax unified
	.cpu cortex-m4
	.thumb

.data

	str: .asciz "Hello World!"

.text
	X: .word 100
	.global main
	.equ AA, 0x55

main:

	ldr r1, =X
	ldr r0, [r1]
