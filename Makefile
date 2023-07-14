CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11

all: main

main: main.cpp lib.h
	$(CC) $(CFLAGS) -o main main.cpp lib.h

clean:
	rm -f main