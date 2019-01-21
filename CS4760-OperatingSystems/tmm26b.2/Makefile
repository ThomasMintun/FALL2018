CC = gcc

SOURCE_MASTER = master.c
OBJECT_MASTER = master.o
EXE_MASTER = master

SOURCE_WORKER = worker.c
OBJECT_WORKER = worker.o
EXE_WORKER = worker

all: $(EXE_MASTER) $(EXE_WORKER)

$(EXE_MASTER): $(OBJECT_MASTER)
	gcc $(OBJECT_MASTER) -o $(EXE_MASTER)

$(EXE_WORKER): $(OBJECT_WORKER)
	gcc $(OBJECT_WORKER) -o $(EXE_WORKER)

%.o: %.c
	gcc -c -g -Wall $*.c -o $*.o

.PHONY: clean

clean:
	rm *.o master worker