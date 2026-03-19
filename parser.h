#ifndef PARSER_H  
#define PARSER_H

#define MAX_ARGS 64 

typedef struct {
    char *infile;   
    char *outfile;  
    int  append;  
} Redirect;


int tokenize(char *input, char **args, Redirect *redir);

#endif