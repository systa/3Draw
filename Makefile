CC=gcc

grprimitives.o: grprimitives.h grprimitives.c
	$(CC) -c grprimitives.c

stlutils.o: grprimitives.h stlutils.h stlutils.c
	$(CC) -c stlutils.c

makestl: makestl.c grprimitives.o stlutils.o
	$(CC) -o makestl makestl.c grprimitives.o stlutils.o -lm

trig: trig.c grprimitives.o stlutils.o
	$(CC) -o trig trig.c grprimitives.o stlutils.o -lm

ladders: ladders.c grprimitives.o stlutils.o
	$(CC) -o ladders ladders.c grprimitives.o stlutils.o -lm

clean:
	rm *.o makestl trig ladders *~

