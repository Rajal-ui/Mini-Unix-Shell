# myshell — A Unix Shell Built from Scratch in C

A POSIX-inspired Unix shell written in C using Linux system calls directly.  
Built from scratch as a hands-on deep dive into how a real shell works under the hood —
process creation, I/O redirection, pipes, signal handling, and more.

> ~800 lines of C · Built and tested on Ubuntu Linux · No external libraries

---

## Why I Built This

Most CS courses teach you *about* `fork()` and `exec()` — this project forced me to actually use them.  
Every feature here required understanding what the OS is doing at the system call level,
not just calling a library function and moving on.

---

## Features

| Feature | Details |
|---------|---------|
| Command execution | `fork()` + `execvp()` + `waitpid()` — the real Unix process model |
| Built-in commands | `cd`, `pwd`, `exit`, `echo` — implemented in the shell process itself |
| I/O Redirection | `>` (overwrite), `>>` (append), `<` (input) via `dup2()` |
| Pipes | Single and chained — `ls \| grep .c \| wc -l` |
| Signal handling | `Ctrl+C` kills the child process, shell survives |
| Environment variables | `$HOME`, `$PATH`, `$USER`, `$?` expansion |
| Background jobs | `sleep 10 &` — non-blocking execution |
| Dynamic prompt | Shows current working directory at all times |

---

## Build and Run

**Requirements:** `gcc`, Linux or WSL (Ubuntu recommended)

```bash
git clone https://github.com/Rajal-ui/myshell.git
cd myshell
make
./myshell
```

You should see:
```
rajal@ubuntu:~/myshell$ _
```

---

## Usage Examples

```bash
# Basic command
rajal@ubuntu:~$ ls -la

# I/O Redirection
rajal@ubuntu:~$ echo "hello" > out.txt
rajal@ubuntu:~$ cat < out.txt
rajal@ubuntu:~$ ls >> out.txt

# Pipes (single and chained)
rajal@ubuntu:~$ ls | grep .c
rajal@ubuntu:~$ ls | grep .c | wc -l

# Background job
rajal@ubuntu:~$ sleep 10 &

# Environment variable expansion
rajal@ubuntu:~$ echo $HOME
rajal@ubuntu:~$ echo $?

# Built-ins
rajal@ubuntu:~$ cd /tmp
rajal@ubuntu:/tmp$ pwd
rajal@ubuntu:/tmp$ exit
```

---

## Design Decisions

These are the real questions that came up while building this — and what I learned solving them.

---

**Why is `cd` a built-in?**

`fork()` creates a copy of the shell process. `chdir()` inside a child changes only
that child's working directory — when the child exits, the change is lost and the
shell's directory is unchanged. `cd` has to call `chdir()` directly inside the
shell process itself to make the change persist. This is true for `exit` too —
calling it in a child would only kill the child, not the shell.

---

**How pipes work internally**

`pipe()` creates two file descriptors — a read end and a write end.
After forking each child, `dup2()` rewires the child's `stdout` to the pipe's
write end (or `stdin` to the read end). The parent closes all pipe fds after
forking so the last command (e.g. `wc -l`) receives `EOF` when the previous
command finishes writing. For N commands chained together, N−1 pipes are created.

---

**`sigaction()` over `signal()`**

`sigaction()` is more portable and has predictable behaviour across Linux versions.
`SA_RESTART` ensures that system calls interrupted mid-execution (like `fgets` waiting
for input) resume automatically instead of returning an error. The shell registers a
custom `SIGINT` handler so `Ctrl+C` is caught cleanly. Each child process restores
`SIG_DFL` before calling `exec` — so `Ctrl+C` terminates the child but the shell
keeps running.

---

**`strtok_r` instead of `strtok`**

The outer loop splits the command line on `|` (pipe), and the inner loop splits
each segment on spaces to get arguments. Both run at the same time in a nested
structure. `strtok` uses a hidden global pointer — two nested calls corrupt each
other silently. `strtok_r` takes an explicit `saveptr` per call, keeping both
tokenizers independent and safe.

---

## Project Structure

```
myshell/
│
├── myshell.c       # Main shell loop, input parsing, built-ins
├── execute.c       # fork/exec/waitpid logic
├── pipes.c         # Pipe setup and chained command handling
├── redirect.c      # I/O redirection via dup2
├── signals.c       # sigaction handlers
├── Makefile
└── README.md
```

---

## What I Learned

- How the Unix process model actually works (`fork` + `exec` + `wait`)
- Why certain commands *must* be built-ins vs external processes
- How the kernel wires file descriptors using `dup2()`
- Why signal handling in shells is more subtle than it looks
- How to manage multiple processes, pipe chains, and resource cleanup without leaking fds

---

## Known Limitations

- No job control (`fg`, `bg`, `jobs` not implemented)
- No tab completion or command history
- Quoting (e.g. `echo "hello world"`) partially supported

---

## Author

**Rajal Mistry** — 2nd year CS undergraduate, Pune  
[GitHub](https://github.com/Rajal-ui) · [LinkedIn](https://linkedin.com/in/rajal-mistry) · [g.dev/rajalmistry](https://g.dev/rajalmistry)
