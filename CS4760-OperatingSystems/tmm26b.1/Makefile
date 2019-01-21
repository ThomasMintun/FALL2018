CC 	= gcc
CFLAGS  = -g -Wall
default: ass1

ass1: ass1.o
	$(CC) $(CLFAGS) -o ass1 ass1.o

ass1.o: ass1.c
	$(CC) $(CFLAGS) -c ass1.c

clean:
	$(RM) ass1 *.o *~
