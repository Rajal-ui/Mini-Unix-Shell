#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parser.h"
#include <fcntl.h> 

int execute(char **args, Redirect *redir) {
    pid_t pid = fork();

    if (pid == 0) {
        if (redir->outfile != NULL) {
            int flags = O_WRONLY | O_CREAT;
            flags |= redir->append ? O_APPEND : O_TRUNC;

            int fd = open(redir->outfile, flags, 0644);
            if (fd < 0) {
                perror("mysh: open");
                exit(1);
            }
         
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        if (redir->infile != NULL) {
            int fd = open(redir->infile, O_RDONLY);
            if (fd < 0) {
                perror("mysh: open");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        execvp(args[0], args);
        fprintf(stderr, "mysh: %s: command not found\n", args[0]);
        exit(127);

    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        perror("mysh: fork");
        return -1;
    }
}