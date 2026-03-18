CC = gcc
CFLAGS = -Wall -Wextra -g

myshell: main.c
	$(CC) $(CFLAGS) -o myshell main.c

clean:
	rm -f myshell