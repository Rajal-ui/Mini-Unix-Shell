CC = gcc
CFLAGS = -Wall -Wextra -g

myshell: main.c parser.c executor.c
	$(CC) $(CFLAGS) -o myshell main.c parser.c executor.c

clean:
	rm -f myshell