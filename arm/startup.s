    .text
    .syntax unified
    .cpu cortex-m4
    .thumb


.globl _reset
.thumb_func
_reset:
    /* Copy Data values from ROM -> SRAM2 */
    ldr r0, = _etext 

    ldr r1, = _sfastdata
    ldr r2, = _efastdata
    bl copytoram

    /* Copy Data values from ROM -> SRAM */
    ldr r1, = _sdata
    ldr r2, = _edata
    bl copytoram

    /* Clear BSS Region */
    ldr r1, = _sbss
clearbss:
    ldr r2, = _ebss
    mov r0, #0x00
    /* Store 0x0 in location held in r1 */
    str r0, [r1], #4
    cmp r1, r2
    bne clearbss

    /* Disable interrupts */
    CPSID IF

    /* Enable FPU (from M4 programming manual) */
    ldr r0, = 0xE000ED88
    ldr r1, [r0]
    orr r1, r1, #(0xF << 20)
    str r1, [r1]
    dsb
    isb

    bl main

/*  Function for copying from rom to ram:
*   r0 = current source location
*   r1 = start of target location
*   r2 = end of target location
*/
copytoram:
    push {r4-r11, lr}
copy:
    /* load from ROM and post increment */
    ldr r3, [r0], #4
    /* Store in RAM and post increment */
    str r3, [r1], #4

    /* Compare current data pointer to end
     * and if equal, then branch to main
     */
    cmp r1, r2
    bne copy

    /* Return */
    pop {r4-r11, pc}

dead:
	b dead
