<h1 align="center">
  C-Shell
</h1>

<h4 align="center">A Custom Shell written in C based on bash semantics </h4>

### Building and running

Run `make` in the root directory to build. Run `./shell` to run the shell.

### Directory structure

- `src` folder contains all the source code of the project which are mainly `.c` files. Each `.c` file implements a functionality/command e.g. `proclore` command
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

1. [Basic System Calls](#1-basic-system-calls)
   1. [Display](#11-display)
   2. [Input](#12-input)
   3. [Warp Command](#13-warp-command)
   4. [Peek Command](#14-peek-command)
   5. [Pastevents Commands](#15-pastevents-commands)
   6. [Processes](#16-processes)
   7. [Proclore Command](#17-proclore-command)
   8. [Seek Command](#18-seek-command)
2. [Processes, Files, and Misc.](#2-processes-files-and-misc)
   1. [I/O Redirection](#21-io-redirection)
   2. [Pipes](#22-pipes)
   3. [Redirection along with Pipes](#23-redirection-along-with-pipes)
   4. [Activities](#24-activities)
   5. [Signals](#25-signals)
   6. [fg and bg](#25-fg-and-bg)
   7. [Neonate](#26-neonate)
3. [Networking](#3-networking)
   1. [iMan](#31-iman)
4. [Commands Implementation overview](#4-command-implementation-overview)

## 1. Basic System Calls

### 1.1 Display

```
<mayankgoel@MayankGoel-2.local:~>
```

- The shell displays a prompt with the format `<Username@SystemName:~>`, providing information about the current directory. The prompt dynamically updates when changing directories.
- Note that `~` refers to the directory in which the shell was started. Any directory/file outside the home directory is shown as an absolute path.

### 1.2 Input

- The shell supports all the bash commands and some custom commands listed below. Multiple commands chaining is supported using `;` or `&`, with robust input handling for random spaces and tabs.
- In case of an invalid command or error. `Invalid command: error` is printed.
- Raw mode is supported in addition to &#8593; and &#8595; arrow keys for command history navigation.

### 1.3 Warp Command

Syntax:  `warp <directory>`

- The `warp` command changes the current working directory to the specified directory.
- Reference symbols `.`  `..`  `~` and `-` are supported and handles both absolute and relative paths.
- In case no directory is specified, it won't change the current working directory.

### 1.4 Peek Command

Syntax:  `peek -flags(optional) <directory>`

- The `peek` command lists files and directories with color-coding: files in white, directories in blue and executable in green.
- If no directory is specified then, it shows the content of the current directory.
- Concatenable flag and reference symbols such as `.`  `..`  `~` and `-` are supported.
- `-a` flag lists all the hidden files/directories (similar to `ls -a` in bash).
- `-l` flag displays the complete information regarding a file/directory (similar to `ls -l` in bash).

### 1.5 Pastevents Commands

```
<mayankgoel@MayankGoel-2.local:~> pastevents   # lists the recently used commands
ls
cat Makefile
clear
<mayankgoel@MayankGoel-2.local:~> pastevents purge  # clears all the pastevents stored
<mayankgoel@MayankGoel-2.local:~> pastevents
<mayankgoel@MayankGoel-2.local:~> 
<mayankgoel@MayankGoel-2.local:~> echo Hello
Hello
<mayankgoel@MayankGoel-2.local:~> pastevents
clear
echo Hello
<mayankgoel@MayankGoel-2.local:~> pastevents execute 1
Hello
```

- The `pastevents` command lists the last 15 used command statements, excluding duplicates and the `pastevents` command.
- The limit of 15 can be changed in `include/params.h`
- `pastevents execute <index>` executes the command at position in pastevents (ordered from most recent to oldest) with base 1 indexing.
- `pastevents purge` clears all the pastevents currently stored. This command is not stored in the pastevents.

### 1.6 Processes

```
<mayankgoel@MayankGoel-2.local:~> sleep 5
# sleeps for 5 seconds
<mayankgoel@MayankGoel-2.local:~ sleep : 5s>
<mayankgoel@MayankGoel-2.local:~>
<mayankgoel@MayankGoel-2.local:~> sleep 10 &
11821
<mayankgoel@MayankGoel-2.local:~> sleep 20 &
11823
<mayankgoel@MayankGoel-2.local:~> echo Hi
Hi
sleep exited normally (11821)
```

#### Foreground Process:

- The shell can execute system commands in both foreground and background.
- Foreground processes which take > 2 seconds to run are printed in the next prompt (rounded down to integer).

#### Background Process.

- Any command invoked with “&” is treated as a background command. This implies that your shell will spawn that process but doesn’t hand the control of terminal to it. Hence, shell will keep taking other user commands.
- Whenever a new background process is started, the PID (Process ID) of the newly created background process is printed and same goes background process ends with the exit status.
- Background processes are not supported for custom commands such as warp, peek, pastevents etc.

### 1.7 Proclore Command

Syntax:  `proclore <PID>`

```
<mayankgoel@MayankGoel-2.local:~> proclore
pid : 210
process status : R+
Process Group : 210
Virtual memory : 167142
executable path : ~/a.out
```

- The `proclore` command provides information about a process, including PID, status, process group, virtual memory, and executable path. Proclore does't work on Mac.
- Process states :
  * R/R+ : Running
  * S/S+ : Sleeping in an interruptible wait
  * Z : Zombie
- `+` signifies a foreground process

### 1.8 Seek Command

Syntax:  `seek -flags <search> <target_directory>`

#### Basic Usage:

```

<mayankgoel@MayankGoel-2.local:~> seek newfolder # searches for newfolder in current dir
./newfolder
./doe/newfolder
./doe/newfolder/newfolder.txt
<mayankgoel@MayankGoel-2.local:~> seek newfolder ./doe
./newfolder                   # This is relative to ./doe
./newfolder/newfolder.txt
```

- The `seek` command looks for a file/directory in the specified target directory (or current if no directory is specified).
- It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue).
- In case of no matching files/directories "No match found!" is printed.

#### Flags:

```
<mayankgoel@MayankGoel-2.local:~> seek -d newfolder ./doe
./newfolder
<mayankgoel@MayankGoel-2.local:~> seek -f newfolder ./doe
./newfolder/newfolder.txt
<mayankgoel@MayankGoel-2.local:~>
<mayankgoel@MayankGoel-2.local:~> seek -e -f newfolder ./doe
./newfolder/newfolder.txt
This is a new folder!    # Content of newfolder.txt
<mayankgoel@MayankGoel-2.local:~> seek -e -d newfolder ./doe
./newfolder/
<JohnDoe@SYS:~/doe/newfolder>
```

- -d : Only look for directories (ignore files even if name matches)
- -f : Only look for files (ignore directories even if name matches)
- -e :
  * This flag is effective only when a single file or a single directory with the name is found.
  * If only one file (and no directories) is found, then it's output is printed.
  * If only one directory (and no files) is found, then change current working directory to it.
  * This flag can be concatenated with -d and -f flags.
- Note that -d and -f flag can’t be used at the same time, if used “Invalid flags!” error message is printed.

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
