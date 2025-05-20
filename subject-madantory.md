# Chapter II: Common Instructions

## General Requirements

- Your project must be written in **C**.
- Your project must adhere to the **Norm(en.norm.tex)**. If there are any norm errors, including in bonus files/functions, the project will receive a **0**.
- Functions must not quit unexpectedly (e.g., segmentation fault, bus error, double free), except for undefined behavior. If such errors occur, the project will be considered **non-functional** and receive a **0**.
- All heap-allocated memory must be properly freed when necessary. **Memory leaks will not be tolerated**.

## Makefile Requirements

- The Makefile must:
  - Compile source files with the flags `-Wall`, `-Wextra`, and `-Werror` using `cc`.
  - Avoid unnecessary relinking.
  - Contain at least the following rules:
    - `$(NAME)`
    - `all`
    - `clean`
    - `fclean`
    - `re`

# Chapter III: Mandatory Part

## Program Details

- **Program name**: `minishell`
- **Turn in files**: `Makefile`, `*.h`, `*.c`
- **Makefile rules**: `NAME`, `all`, `clean`, `fclean`, `re`
- **Libft authorized**: Yes

## External Functions Allowed

- `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`
- `printf`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`
- `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`, `exit`
- `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `execve`, `dup`, `dup2`, `pipe`
- `opendir`, `readdir`, `closedir`, `strerror`, `perror`, `isatty`, `ttyname`, `ttyslot`, `ioctl`
- `getenv`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

## Requirements

Your shell must:

1. **Display a prompt** when waiting for a new command.
2. **Maintain a working history** of commands.
3. **Search and launch executables**:
   - Based on the `PATH` variable.
   - Using relative or absolute paths.
4. **Handle signals**:
   - Use at most **one global variable** to indicate a received signal.
   - This variable must only store the signal number and must not provide additional information or access to data.
5. **Handle quotes**:
   - Single quotes (`'`) prevent interpretation of metacharacters.
   - Double quotes (`"`) prevent interpretation except for `$` (dollar sign).
6. **Implement redirections**:
   - `<` redirects input.
   - `>` redirects output.
   - `<<` reads input until a delimiter is found (does not update history).
   - `>>` appends output to a file.
7. **Implement pipes** (`|`):
   - Connect the output of one command to the input of the next.
8. **Expand environment variables**:
   - `$` followed by a sequence of characters expands to their values.
   - `$?` expands to the exit status of the most recently executed foreground pipeline.
9. **Handle interactive mode**:
   - `ctrl-C`: Displays a new prompt on a new line.
   - `ctrl-D`: Exits the shell.
   - `ctrl-\`: Does nothing.

## Built-in Commands

Your shell must implement the following:

- `echo` with `-n` option.
- `cd` with relative or absolute paths.
- `pwd` with no options.
- `export` with no options.
- `unset` with no options.
- `env` with no options or arguments.
- `exit` with no options.

## Memory Management

- The `readline()` function may cause memory leaks, but you are not required to fix them.
- Your own code must not have memory leaks.
- Limit yourself to the subject description. Anything not asked is not required.
- If in doubt, take **bash** as a reference.
