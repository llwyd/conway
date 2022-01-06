conway.bin: conway.elf
	arm-none-eabi-objcopy -Obinary conway.elf conway.bin
conway.elf: i2c.o life.o display.o conway.o vectors.o startup.o conway.ld
	arm-none-eabi-ld -Map conway.map -Tconway.ld -o conway.elf startup.o i2c.o life.o display.o conway.o vectors.o -print-memory-usage -nostdlib
	arm-none-eabi-objdump -t conway.elf
startup.o: startup.s
	arm-none-eabi-as -g -mcpu=cortex-m4 -mthumb  startup.s -o startup.o
vectors.o: vectors.s
	arm-none-eabi-as -g -mcpu=cortex-m4 -mthumb  vectors.s -o vectors.o
conway.o: conway.c
	arm-none-eabi-gcc -O3  -Wall  -g -nostdlib -nostartfiles -mcpu=cortex-m4 -I../life -mthumb -c conway.c -o conway.o
display.o: display.c
	arm-none-eabi-gcc -O3  -Wall  -g -nostdlib -nostartfiles -mcpu=cortex-m4 -mthumb -c display.c -o display.o
life.o: ../life/life.c
	arm-none-eabi-gcc -O3  -Wall  -g -nostdlib -nostartfiles -lgcc -mcpu=cortex-m4 -mthumb -c ../life/life.c -o life.o
i2c.o: i2c.c
	arm-none-eabi-gcc -Wall  -g -nostartfiles -mcpu=cortex-m4 -mthumb -c i2c.c -o i2c.o
clean:
	rm startup.o conway.elf conway.bin conway.o conway.map vectors.o i2c.o display.o life.o
debug:
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2-1.cfg -f /usr/share/openocd/scripts/target/stm32l4x.cfg
flash:
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2-1.cfg -f /usr/share/openocd/scripts/target/stm32l4x.cfg -c "program conway.bin exit 0x08000000 verify reset"
gdb:
	arm-none-eabi-gdb -ex "target remote localhost:3333" conway.elf
ezflash:
	st-flash --reset write conway.bin 0x08000000
dump:
	hexdump -C conway.bin
