#ifndef PARSER_H
#define PARSER_H
#include <stddef.h>

#define MAX_ARGS  64
#define MAX_PIPES 16

typedef struct {
    char *infile;
    char *outfile;
    int   append;
    int   background;
} Redirect;

typedef struct {
    char    *args[MAX_ARGS];
    Redirect redir;
} Command;

extern int last_exit_status;

int  tokenize(char *input, char **args, Redirect *redir);
int  parse_pipeline(char *input, Command cmds[]);
void expand_variables(char **args);

#endif