# Mini-Unix-Shell

# myshell — a Unix shell built from scratch in C

A POSIX-inspired Unix shell implementing core OS concepts:
process creation, I/O redirection, pipes, and signal handling.
Built in C using Linux system calls directly.

## Features

- Command execution via fork/execvp/waitpid
- Built-in commands: cd, pwd, exit, echo
- I/O redirection: > (output), >> (append), < (input)
- Pipes: single and chained (ls | grep .c | wc -l)
- Signal handling: Ctrl+C kills child, shell survives
- Environment variable expansion: $HOME, $PATH, $USER, $?
- Background jobs: sleep 10 &
- Dynamic prompt showing current directory

## Build and run

```
git clone https://github.com/YOURNAME/myshell.git
cd myshell
make
./myshell
```

Requires: gcc, Linux or WSL

## Design decisions

**Why is cd a built-in?**
fork() creates a copy of the shell process. chdir() inside
a child changes only that child's directory. When the child
exits, the change is lost. cd must call chdir() directly in
the shell process to persist the directory change.

**How pipes work**
pipe() creates two file descriptors — a read end and write
end. After forking each child, dup2() rewires the child's
stdout to the pipe write end (or stdin to the read end).
The parent closes all pipe fds after forking so wc -l
receives EOF when ls finishes. N commands use N-1 pipes.

**Why sigaction() over signal()**
sigaction() is more portable and predictable. SA_RESTART
ensures that system calls interrupted by signals (like
fgets) resume automatically rather than returning errors.
The shell sets SIGINT to a custom handler; each child
restores SIG_DFL before exec so Ctrl+C kills the child
but not the shell.

**strtok_r instead of strtok**
The outer pipe-split loop and inner space-split tokenizer
run simultaneously. strtok uses a hidden global pointer —
two nested calls corrupt each other. strtok_r takes an
explicit saveptr per call, making both splits independent.

## Resume line

"Built a POSIX-compliant Unix shell in C — supports pipes,
I/O redirection, signal handling, and environment variable
expansion via fork/exec/wait model (~800 lines of C)"
