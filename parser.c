#include "parser.h"
#include <string.h>  
#include <stdio.h>

int tokenize(char *input, char **args) {
    int count = 0;
    char *token = strtok(input, " \t");

    while (token != NULL && count < MAX_ARGS - 1) {
        args[count] = token; 
        count++;

        token = strtok(NULL, " \t");
    }

    args[count] = NULL;  
    return count;
}