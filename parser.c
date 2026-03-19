#include "parser.h"
#include <string.h>  
#include <stdio.h>

int tokenize(char *input, char **args, Redirect *redir) {
    
    redir->infile  = NULL;
    redir->outfile = NULL;
    redir->append  = 0;

    int count = 0;
    char *token = strtok(input, " \t");

    while (token != NULL && count < MAX_ARGS - 1) {

        if (strcmp(token, ">>") == 0) {            
            redir->outfile = strtok(NULL, " \t");
            redir->append  = 1;

        } else if (strcmp(token, ">") == 0) {           
            redir->outfile = strtok(NULL, " \t");
            redir->append  = 0;

        } else if (strcmp(token, "<") == 0) {            
            redir->infile = strtok(NULL, " \t");

        } else {           
            args[count++] = token;
        }

        token = strtok(NULL, " \t");
    }
    args[count] = NULL;
    return count;
}