<h1 align="center">
  C-Shell
</h1>

<h4 align="center">A Custom Shell written in C based on bash semantics </h4>

### Building and running

Run `make` in the root directory to build. Run `./shell` to run the shell.

### Directory structure

- `src` folder contains all the source code of the project which are mainly `.c` files. Each `.c` file implements a functionality/command e.g. `bg` command
- `include` folder contains all the header files associated with the format `.h`.
- `include/libs.h` is the main header file which imports all the other custom header files.

<pre>
.
├── shell
├── include
│   ├── activities.h
│   ├── bg_process.h
│   ├── color.h
│   ├── helper_functions.h
│   ├── iman.h
│   ├── input_handling.h
│   ├── input_redirection.h
│   ├── neonate.h
│   ├── params.h
│   ├── pastevents.h
│   ├── path_handling.h
│   ├── peek.h
│   ├── piping.h
│   ├── proclore.h
│   ├── raw_mode.h
│   ├── seek.h
│   ├── shell.h
│   ├── signal.h
├── makefile
├── README.md
└── src
    ├── activities.c
    ├── bg_processes.c
    ├── color.c
    ├── helper_functions.c
    ├── iman.c
    ├── input_handling.c
    ├── input_redirection.c
    ├── main.c
    ├── neonate.c
    ├── pastevents.c
    ├── path_handling.c
    ├── peek.c
    ├── piping.c
    ├── proclore.c
    ├── raw_mode.c
    ├── seek.c
    ├── signal.c
</pre>

## Usage:

- Precedence order followed is `\` , `"` , `'` , `;` , `&` , `|` , `<<` , `<` , `>` in the decreasing order. Any symbol after `\` is ignore similar to bash.

- `;` and `&` can be used to chain commands. `;` runs the previous command in foreground. `&` symbol runs the previous command in background.

- Pipe symbol `|` can be used for piping commands similar to bash.

- Input-output redirection can be performed using `>`, `>>` and `<` similar to bash.

- `.. . ~ -` can be used for referencing directories similar to bash.

- &#8593; and &#8595; arrows can be used to scroll through the command history similar to bash.


## Assumptions:

- Multiple spaces and tab spaces are ignored.

- Commands, files, folders are case-insensitive.

- The history is stored in a hidden file `.CShell_history`.

- `cd` command won't change directories due to running a command in a fork. Instead use `warp` command to change directories.

- If the shell has just started, `warp -` won't change any directory.

- Reasonable assumptions have been made for the maximum length of commands, filenames, history etc. The lengths for the same can be found out in the `include/params.h` file.


## Table of Contents
1. [Basic System Calls](#basic-system-calls)
    1. [Display](#display)
    2. [Input Requirements](#input-requirements)
    3. [Warp](#warp)
    4. [Peek](#peek)
    5. [Pastevents Commands](#pastevents-commands)
    6. [System Commands](#system-commands)
    7. [Proclore](#proclore)
    8. [Seek](#seek)
2. [Processes, Files, and Misc.](#processes-files-and-misc)
    1. [I/O Redirection](#io-redirection)
    2. [Pipes](#pipes)
    3. [Redirection along with Pipes](#redirection-along-with-pipes)
    4. [Activities](#activities)
    5. [Signals](#signals)
    6. [fg and bg](#fg-and-bg)
    7. [Neonate](#neonate)
3. [Networking](#networking)
    1. [iMan](#iman)
4. [Commands Implementation overview](#)


## 1. Basic System Calls

### 1.1 Display
The shell displays a prompt with the format `<Username@SystemName:~>`, providing information about the current directory. The prompt dynamically updates when changing directories.

### 1.2 Input Requirements
The shell supports a list of commands separated by `;` or `&`, with robust input handling for random spaces and tabs. It can execute commands in both foreground and background.

### 1.3 Warp
The `warp` command changes the current working directory and supports various flags like `.` , `..` , `~` and `-`. It handles both absolute and relative paths.

### 1.4 Peek
The `peek` command lists files and directories with color coding based on flags like `-a`, and `-l`. It supports symbols like `.` , `..` , `~` and `-`.

### 1.5 Pastevents Commands
The `pastevents` command stores and retrieves the 15 (this limit can be changed in `include/params.h`) most recent command statements, excluding duplicates and specific commands. It supports execution of past events and purging.

### 1.6 System Commands
The shell can execute system commands in both foreground and background. It provides information about foreground processes' execution time and name. Background processes are managed, and their completion is reported.

### 1.7 Proclore
The `proclore` command provides information about a process, including PID, status, process group, virtual memory, and executable path. Proclore won't work on mac.

### 1.8 Seek
The `seek` command searches for files or directories in the specified target directory, supporting flags like `-d` , `-f`, and `-e`.


## 2. Processes, Files, and Misc.

### 2.1 I/O Redirection
The shell supports I/O redirection with `>` , `>>` and `<`. It handles errors gracefully, creating or overwriting files based on the redirection type.

### 2.2 Pipes
Pipes are supported for passing information between commands. The shell handles multiple pipes and executes commands sequentially.

### 2.3 Redirection along with Pipes
The shell seamlessly integrates I/O redirection with pipes.

### 2.4 Activities
The `activities` command displays a list of running or stopped processes spawned by the shell, including command name, PID, and state.

### 2.5 Signals
The `ping` command sends signals to processes, allowing termination or modification of their behavior. Keyboard shortcuts like `Ctrl-C`, `Ctrl-D` and `Ctrl-Z` are supported.

### 2.6 fg and bg
The `fg` command brings a background process to the foreground, while `bg` changes the state of a stopped background process to running.

### 2.7 Neonate
The `neonate` command prints the PID of the most recently created process at intervals until the user presses `x`.


## 3. Networking

### 3.1 iMan
The `iMan` command fetches man pages from http://man.he.net/ using sockets. It displays information about the specified command or outputs an error if the page does not exist.



## 4. Commands Implementation overview:

| Commands             | Forked or not | Input Redirection | Output Redirection              | Piping status         |
| -------------------- | ------------- | ----------------- | ------------------------------- | --------------------- |
| `activities`       | yes           | no                | yes                             | cannot read from pipe |
| `proclore`         | yes           | no                | yes                             | cannot read from pipe |
| `bg`               | no            | no                | no                              | not piped             |
| `fg`               | no            | no                | no                              | not piped             |
| `iMan`             | yes           | no                | yes                             | cannot read from pipe |
| `neonate`          | yes           | no                | yes                             | cannot read from pipe |
| `ping`             | yes           | no                | yes                             | cannot read from pipe |
| `warp`             | no            | no                | yes                             | cannot read from pipe |
| `peek`             | yes           | no                | yes                             | cannot read from pipe |
| `seek`             | no            | no                | yes                             | cannot read from pipe |
| `pastevents`       | yes           | no                | yes                             | cannot read from pipe |
| `pastevents purge` | yes           | no                | yes (but no output to redirect) | cannot read from pipe |
| builtins             | yes           | yes               | yes                             | piped                 |