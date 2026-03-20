#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char exit_buf[16];

void expand_variables(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] != '$') continue;
        char *name = args[i] + 1;
        if (strcmp(name, "?") == 0) {
            snprintf(exit_buf, sizeof(exit_buf), "%d", last_exit_status);
            args[i] = exit_buf;
        } else {
            char *val = getenv(name);
            args[i] = val ? val : "";
        }
    }
}

int tokenize(char *input, char **args, Redirect *redir) {
    redir->infile     = NULL;
    redir->outfile    = NULL;
    redir->append     = 0;
    redir->background = 0;

    int count = 0;
    char *saveptr;
    char *token = strtok_r(input, " \t", &saveptr);

    while (token != NULL && count < MAX_ARGS - 1) {
        if (strcmp(token, ">>") == 0) {
            redir->outfile = strtok_r(NULL, " \t", &saveptr);
            redir->append  = 1;
        } else if (strcmp(token, ">") == 0) {
            redir->outfile = strtok_r(NULL, " \t", &saveptr);
        } else if (strcmp(token, "<") == 0) {
            redir->infile = strtok_r(NULL, " \t", &saveptr);
        } else {
            args[count++] = token;
        }
        token = strtok_r(NULL, " \t", &saveptr);
    }
    if (count > 0 && strcmp(args[count-1], "&") == 0) {
        redir->background = 1;
        args[--count] = NULL;
    }
    args[count] = NULL;
    return count;
}

int parse_pipeline(char *input, Command cmds[]) {
    int num_cmds = 0;
    char *saveptr;
    char *segment = strtok_r(input, "|", &saveptr);
    while (segment != NULL && num_cmds < MAX_PIPES) {
        tokenize(segment, cmds[num_cmds].args, &cmds[num_cmds].redir);
        num_cmds++;
        segment = strtok_r(NULL, "|", &saveptr);
    }
    return num_cmds;
}