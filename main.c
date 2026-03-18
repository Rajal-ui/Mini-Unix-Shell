#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"

#define MAX_INPUT 1024

int main(void) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("mysh$ ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n"); break;
        }
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0') continue;

        int count = tokenize(input, args);
        if (count == 0) continue;

        execute(args);  /* run the command! */
    }
    return 0;
}