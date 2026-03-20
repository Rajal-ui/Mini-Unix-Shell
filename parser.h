#ifndef PARSER_H  
#define PARSER_H

#define MAX_ARGS 64 
#define MAX_PIPES 16

typedef struct {
    char *infile;   
    char *outfile;  
    int  append;  
} Redirect;

typedef struct {
    char  *args[MAX_ARGS]; 
    Redirect redir;        
} Command;

int parse_pipeline(char *input, Command cmds[]);
int tokenize(char *input, char **args, Redirect *redir);

#endif