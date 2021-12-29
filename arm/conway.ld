MEMORY {
    ROM  (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 48K
    RAM2 (rwx) : ORIGIN = 0x10000000, LENGTH = 16K
}

STACK_SIZE = 0x1000;

SECTIONS
{
    . = ORIGIN(ROM);
    .text :
    {
        KEEP(*(.isr_vectors))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > ROM

    . = ORIGIN(RAM2);
    .fastdata :
    {
        _sfastdata = .;
        *(.fastdata*)
        _efastdata = .;
    } > RAM2 AT > ROM

    . = ORIGIN(RAM);
    .data :
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT > ROM
    
    . = ALIGN(4);
    .bss :
    { 
        _sbss = .;
        *(.bss*)
        _ebss = .;
    } > RAM AT > ROM  

    . = ALIGN(4);
    .stack :
    {
        . += STACK_SIZE;
        . = ALIGN(4);
        _estack = .;
    } > RAM
}
