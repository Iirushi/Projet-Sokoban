CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = sokoban

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o

main.o: main.c sokoban.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o $(TARGET)
