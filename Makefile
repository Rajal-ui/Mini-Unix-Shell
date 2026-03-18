CC = gcc
CFLAGS = -Wall -Wextra -g

myshell: main.c
	$(CC) $(CFLAGS) -o myshell main.c parser.c

clean:
	rm -f myshell