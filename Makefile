CC = gcc
CFLAGS = -Wall -Wextra -g

myshell: main.c parser.c executor.c builtins.c pipes.c signals.c
	$(CC) $(CFLAGS) -o myshell main.c parser.c executor.c builtins.c pipes.c signals.c

clean:
	rm -f myshell