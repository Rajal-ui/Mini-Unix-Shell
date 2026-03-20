#include "parser.h"
#include <string.h>

int tokenize(char *input, char **args, Redirect *redir) {
    redir->infile  = NULL;
    redir->outfile = NULL;
    redir->append  = 0;

    int count = 0;
    char *saveptr;                           
    char *token = strtok_r(input, " \t", &saveptr);

    while (token != NULL && count < MAX_ARGS - 1) {
        if (strcmp(token, ">>") == 0) {
            redir->outfile = strtok_r(NULL, " \t", &saveptr);
            redir->append  = 1;
        } else if (strcmp(token, ">") == 0) {
            redir->outfile = strtok_r(NULL, " \t", &saveptr);
            redir->append  = 0;
        } else if (strcmp(token, "<") == 0) {
            redir->infile = strtok_r(NULL, " \t", &saveptr);
        } else {
            args[count++] = token;
        }
        token = strtok_r(NULL, " \t", &saveptr);
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