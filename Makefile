CC = gcc
CFLAGS = -Wall -Wextra -g

myshell: main.c parser.c executor.c builtins.c
	$(CC) $(CFLAGS) -o myshell main.c parser.c executor.c builtins.c

clean:
	rm -f myshell