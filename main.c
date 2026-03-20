#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "executor.h"
#include "builtins.h"
#include "pipes.h"
#include "signals.h"

#define MAX_INPUT 1024

int last_exit_status = 0;

int main(void) {
    char    input[MAX_INPUT];
    Command cmds[MAX_PIPES];
    char    cwd[1024];

    setup_signals();

    while (1) {
        if (getcwd(cwd, sizeof(cwd))) {
            char *dir = strrchr(cwd, '/');
            printf("%s$ ", dir ? dir + 1 : cwd);
        } else {
            printf("mysh$ ");
        }
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n"); break;
        }
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0') continue;

        int num_cmds = parse_pipeline(input, cmds);
        if (num_cmds == 0) continue;

        for (int i = 0; i < num_cmds; i++)
            expand_variables(cmds[i].args);

        int has_redirect = (cmds[0].redir.outfile || cmds[0].redir.infile);
        if (num_cmds == 1 && !has_redirect && run_builtin(cmds[0].args) == 1)
            continue;

        last_exit_status = execute_pipeline(cmds, num_cmds);
    }
    return 0;
}