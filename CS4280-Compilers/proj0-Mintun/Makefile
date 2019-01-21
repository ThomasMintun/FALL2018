CC = g++
CFLAGS = -c -std=c++11 -Wall -g
TARGET = P0

P0: main.o tree.o
	$(CC) -o $(TARGET) main.o tree.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

tree.o: tree.cpp node.h tree.h
	$(CC) $(CFLAGS) tree.cpp

clean:
	rm *.o out.txt P0 *.Inorder *.Postorder *.Preorder 