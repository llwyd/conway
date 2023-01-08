conway: life.o conway.o
	gcc -lX11 life.o conway.o -o conway
conway.o: conway.c
	gcc -O0  -Wall -DTARGET_DESKTOP -g -lX11 -I../life -c conway.c -o conway.o
life.o: ../life/life.c
	gcc -O0  -Wall -DTARGET_DESKTOP -g -c ../life/life.c -o life.o
clean:
	rm conway.o life.o conway
