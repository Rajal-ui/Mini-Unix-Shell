# MiniShell — a Minimal Unix Shell


MiniShell is a compact, POSIX-style command-line shell implemented in C. It reproduces core shell behaviors such as process creation, job control, piping, redirection, and a small set of built-in commands.

## Concept

MiniShell implements the core concepts of a Unix shell: read an input line, parse it into tokens, build an execution plan (commands, pipes, redirections), and run that plan while managing processes and I/O. The implementation follows common POSIX semantics for process creation and I/O handling.

## How it works

- Read-eval loop: the program reads lines from `stdin`, prompts for input, and dispatches parsing and execution for each line.
- Tokenization: the lexer splits input into tokens on unquoted whitespace and recognizes quoted strings and special operators (`|`, `>`, `>>`, `<`).
- Parsing: tokens are arranged into commands and I/O actions. Pipelines are represented as ordered command stages with associated redirections.
- Execution: for each command or pipeline, the shell creates child processes using **fork()**, configures I/O with `pipe()` and **dup2()**, then the child calls **execve()** to run external programs. The parent uses **waitpid()** to collect statuses.
- Built-ins: internal commands such as `cd`, `echo`, `exit`, `env`, `export`, and `unset` are executed without spawning external processes when appropriate.
- Signals: the shell handles interactive signals (SIGINT, SIGQUIT) to control foreground job behavior while keeping the shell process running.

## Quick Start

Build and run the shell:

```bash
make
./myshell
```

Common usage examples:

```sh
# Run an external command
ls -la

# Pipe commands
ls | grep txt

# Redirect output
echo hello > out.txt
cat < out.txt

# Use a built-in
cd /tmp
env | grep PATH
```

## Commands (built-ins)

| Command | Behavior |
|---|---|
| `cd` | Change current directory |
| `echo` | Print arguments to stdout |
| `exit` | Exit the shell with optional status |
| `env` | Print environment variables |
| `export` | Set environment variables |
| `unset` | Remove environment variables |

## Implementation Details

- Process creation: **fork()** is used to spawn children; children call **execve()** to run programs. The parent calls **waitpid()** to wait for child termination and to obtain exit statuses.
- Pipelines: implemented using `pipe()` pairs and careful `dup2()` usage to connect `STDIN_FILENO`/`STDOUT_FILENO` between processes.
- Redirections: handled by opening files with appropriate flags and `dup2()`ing file descriptors into the child process before `execve()`.
- Signals: the shell manages SIGINT and SIGQUIT, forwarding signals to child processes as appropriate while preserving the interactive prompt.

## Testing & Memory Checks

Manual test recommendations:

1. Build: `make`
2. Run: `./myshell`
3. Exercise built-ins, pipelines, and redirections as shown above.

Memory checks (Valgrind):

```bash
valgrind --leak-check=full ./myshell
```

## Files of interest

- `main.c` — entry and main loop
- `parser.c`, `parser.h` — tokenization and parsing
- `executor.c`, `executor.h` — execution, piping, redirection
- `builtins.c`, `builtins.h` — built-in command logic
- `signals.c`, `signals.h` — signal handling

## Contributing

Bug reports and pull requests are welcome. For clarity, include simple reproduction steps and expected behavior.


## Design Goals

- Correctness: follow POSIX semantics for process creation, exit statuses, and I/O redirection where practical.
- Simplicity: implement a clear, readable code structure separating lexing, parsing, and execution.
- Robustness: avoid crashes on malformed input and handle common edge cases in parsing and I/O.
- Resource safety: manage file descriptors and heap allocations carefully to prevent leaks and descriptor leaks.

## Examples and Use Cases

- Interactive command entry: run `./myshell`, then type `grep -i TODO README.md` to search project TODOs.
- Compound pipelines: `cat file.txt | tr ' ' '\n' | sort | uniq -c` demonstrates multi-stage pipeline handling.
- File operations: `sort input.txt > sorted.txt` shows output redirection; `wc -l < sorted.txt` shows input redirection.

## Limitations

- No advanced job control (no background job management with `&`, no `fg`/`bg` built-ins).
- Limited shell expansions: no globbing (`*`), no advanced parameter expansion, and no command substitution.
- Minimal error messages compared to a full shell; some failure modes report only standard `errno` text.

## Future Work

- Add background job control and a basic job table.
- Implement wildcard expansion (globbing) and simple variable expansion.
- Add an automated test suite with expected input/output cases.


