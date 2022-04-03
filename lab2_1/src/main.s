.syntax unified
.cpu cortex-m4
.thumb

.data
    Leds: .byte 0

.text
    .global main
    .equ mini_sec,  0x96     // 150
    .include "../src/pin.s"  // import GPIO constants

Delay:
    PUSH {r0, lr}
    ldr r0, =mini_sec     // Use to control total delay time
Delay_loop:
    subs r0, r0, #1       // 1 cycle
    bne Delay_loop        // 1~3 cycle
    POP {r0, pc}

// r0 = value
DisplayLED:
    PUSH {r1, r2, lr}
    ldr r1, =GPIOB_ODR
    ldr r2, [r1]          // Get Current Output
    and r2, #0xFFFFFF87   // Clear PB3 to PB6
    eor r0, #0xFFFFFFFF   // Reverse r0 (Active Low)
    and r0, #0x78         // Select the needed bits
    orr r2, r0            // Set the output bits
    str r2, [r1]          // Write back to Output Reg
    POP {r1, r2, pc}

GPIO_init:
    // Enable AHB2 clock
    movs r0, #0x6
    ldr r1, =RCC_AHB2ENR
    str r0, [r1]
    // Set PB3 ~ PB6 to output mode
    ldr r1, =GPIOB_MODER
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]
    // Set PB3 ~ PB6 to high speed
    ldr r1, =GPIOB_OSPEEDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x2A80
    str r0, [r1]
    // Set PB3 ~ PB6 to pullup
    ldr r1, =GPIOB_PUPDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]
    // Set PC13 to input mode
    ldr r1, =GPIOC_MODER
    ldr r0, [r1]
    and r0, #0xF3FFFFFF
    orr r0, #0x0
    str r0, [r1]
    bx lr

// r3 = last button status
// r4 = shift direction (0:left, 1:right)
// r5 = Leds Address
// r6 = lock shifting (0=lock)
Loop:
    ldrb r0, [r5]        // Load Leds into r0
    and r0, r0, #0x1E    // Get the center 4 bits
    lsl r0, #2           //     which are the next led pattern
    bl DisplayLED        // Branch to change led display
    movs r2, #100        // Check For 100 times of Button pressing
loop_check:
    bl check_button      // Branch to Ckeck button press
    subs r2, r2, #1      // Decrement counter
    bne loop_check       // Check if loop is finished
    ldrb r0, [r5]        // Load old Leds into r0
    and r1, r0, #0x20    // Check if need to change shifting direction
    cbnz r1, Loop_right  // Hit left border
    and r1, r0, #0x01
    cbnz r1, Loop_left   // Hit right border
    b Loop_end
Loop_left:
    mov r4, #0           // Change shift direction to left
    b Loop_end
Loop_right:
    mov r4, #1           // Change shift direction to right
    b Loop_end
Loop_end:
    cbz r4, mov_left     // Choose which way to shift Leds
mov_right:
    lsr r0, #1           // Shift Leds right
    b mov_done
mov_left:
    lsl r0, #1           // Shift Leds left
    b mov_done
mov_done:
    cbz r6, mov_end      // If locked, don't move
    strb r0, [r5]        // Else, write new Leds back to Leds
mov_end:
    b Loop

// r3 = last button status
// r4 = shift direction (0:left, 1:right)
// r5 = Leds Address
// r6 = lock shifting (0=lock)
check_button:
    PUSH {r0, r1, r2, lr}
    movs r0, #100        // Check for 100 times for debounce
    movs r1, #0          // r1 = The amount of times button equals 1
check_button_loop:
    bl Delay             // Delay for a short period of time
    ldr r2, =GPIOC_IDR   // Check if button is pressed
    ldr r2, [r2]
    ands r2, #0x2000
    bne check_button_end // If button = 0, branch to check_button_end
    add r1, r1, #1       // Else, increment button press counter
check_button_end:
    subs r0, r0, #1      // Decrement counter
    bne check_button_loop
    cmp r1, #50          // Check if there are more than 50 ones
    blt check_button_change   // Negative edge trigger
    movs r3, #1          // Last button status is 1
    POP {r0, r1, r2, pc}
check_button_change:
    cbz r3, check_button_nochange  // Check if switch from 1 to 0
    movs r3, #0          // Last button status is 0
    cbz r6, check_button_change_1
    movs r6, #0x0        // Unlock Leds
    POP {r0, r1, r2, pc}
check_button_change_1:
    movs r6, #0x1        // Lock up Leds
    POP {r0, r1, r2, pc}
check_button_nochange:
    POP {r0, r1, r2, pc}

main:
    bl GPIO_init    // Setup GPIO
    movs r0, #0x3   // write 0b'000011 to Leds
    ldr r5, =Leds   // r5 = Leds addr
    strb r0, [r5]
    movs r6, #0x1   // r6 = lock (0=lock)
    movs r3, #0     // r3 = last button stat
    b Loop

L: b L

