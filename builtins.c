#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void builtin_cd(char **args) {
    char *path;                       

    if (args[1] == NULL) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "mysh: cd: HOME not set\n");
            return;
        }
    } else {
        path = args[1];
    }

    if (chdir(path) != 0)
        fprintf(stderr, "mysh: cd: %s: No such file or directory\n", path);
}

static void builtin_pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("mysh: pwd");
    }
}

static void builtin_exit(char **args) {
    int code = 0;
    if (args[1] != NULL) {
        code = atoi(args[1]); 
    }
    exit(code);
}

static void builtin_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        if (i > 1) printf(" ");
        printf("%s", args[i]);
    }
    printf("\n");
}

int run_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0)   { builtin_cd(args);  return 1; }
    if (strcmp(args[0], "pwd") == 0)  { builtin_pwd();     return 1; }
    if (strcmp(args[0], "exit") == 0) { builtin_exit(args);return 1; }
    if (strcmp(args[0], "echo") == 0) { builtin_echo(args);return 1; }
    
    return 0; 
}