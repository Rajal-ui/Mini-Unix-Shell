#include "signals.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sigint_handler(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\nmysh$ ", 7);
}

void setup_signals(void) {
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTSTP, &sa, NULL);
}