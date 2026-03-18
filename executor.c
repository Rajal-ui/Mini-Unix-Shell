#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int execute(char **args) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        // only reached if execvp failed
        fprintf(stderr, "mysh: %s: command not found\n", args[0]);
        exit(127);
    }
    else if (pid > 0) {
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    else {
        perror("mysh: fork");
        return -1;
    }
}