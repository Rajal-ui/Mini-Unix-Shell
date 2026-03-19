#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"
#include "builtins.h"

#define MAX_INPUT 1024

int last_exit_status = 0;

int main(void) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    Redirect redir;

    while (1) {
        printf("mysh$ ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0') continue;

        int count = tokenize(input, args, &redir);
        if (count == 0) continue;

        int has_redirect = (redir.outfile != NULL || redir.infile != NULL);

        if (!has_redirect && run_builtin(args) == 1) {
            continue;
        }

        last_exit_status = execute(args, &redir);
    }

    return 0;
}