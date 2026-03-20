#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int execute_pipeline(Command cmds[], int num_cmds) {
   
    if (num_cmds == 1) {
        pid_t pid = fork();
        if (pid == 0) {
            signal(SIGINT, SIG_DFL);

            if (cmds[0].redir.outfile) {
                int fl = O_WRONLY|O_CREAT|(cmds[0].redir.append?O_APPEND:O_TRUNC);
                int fd = open(cmds[0].redir.outfile, fl, 0644);
                if (fd < 0) { perror("mysh: open"); exit(1); }
                dup2(fd, STDOUT_FILENO); close(fd);
            }
            if (cmds[0].redir.infile) {
                int fd = open(cmds[0].redir.infile, O_RDONLY);
                if (fd < 0) { perror("mysh: open"); exit(1); }
                dup2(fd, STDIN_FILENO); close(fd);
            }
            execvp(cmds[0].args[0], cmds[0].args);
            fprintf(stderr,"mysh: %s: command not found\n",cmds[0].args[0]);
            exit(127);
        }
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }

    int pipes[MAX_PIPES - 1][2];
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("mysh: pipe"); return -1;
        }
    }

    pid_t pids[MAX_PIPES];

    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();

        if (pids[i] == 0) {
            signal(SIGINT, SIG_DFL);

            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            if (cmds[i].redir.outfile) {
                int fl = O_WRONLY|O_CREAT|(cmds[i].redir.append?O_APPEND:O_TRUNC);
                int fd = open(cmds[i].redir.outfile, fl, 0644);
                if (fd < 0) { perror("mysh: open"); exit(1); }
                dup2(fd, STDOUT_FILENO); close(fd);
            }
            if (cmds[i].redir.infile) {
                int fd = open(cmds[i].redir.infile, O_RDONLY);
                if (fd < 0) { perror("mysh: open"); exit(1); }
                dup2(fd, STDIN_FILENO); close(fd);
            }

            execvp(cmds[i].args[0], cmds[i].args);
            fprintf(stderr,"mysh: %s: command not found\n",cmds[i].args[0]);
            exit(127);
        }
    }

    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    int status = 0;
    for (int i = 0; i < num_cmds; i++) {
        int s;
        waitpid(pids[i], &s, 0);
        if (i == num_cmds - 1)
            status = WEXITSTATUS(s); 
    }
    return status;
}