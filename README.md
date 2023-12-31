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
│   ├── libs.h
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
- `..`, `.`, `~`, and `-` can be used for path referencing similar to bash.
- &#8593; and &#8595; arrows can be used to scroll through the command history similar to bash.

## Assumptions:

- Multiple spaces and tab spaces are ignored.
- Commands, file and folder names are case-insensitive.
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
2. [Piping, Redirection, and Misc.](#2-piping-redirection-and-misc)
   1. [I/O Redirection](#21-io-redirection)
   2. [Pipes](#22-pipes)
   3. [Redirection along with Pipes](#23-redirection-along-with-pipes)
   4. [Activities](#24-activities)
   5. [Signals](#25-signals)
   6. [fg command](#26-fg-command)
   7. [bg command](#27-bg-command)
   8. [Neonate Command](#28-neonate-command)
3. [Networking](#3-networking)
   1. [iMan](#31-iman)
4. [Commands Implementation overview](#4-commands-implementation-overview)

## 1. Basic System Calls

### 1.1 Display

```
<mayankgoel@MayankGoel-2.local:~>
```

- The shell displays a prompt with the format `<Username@SystemName:~>`, providing information about the current directory. The prompt dynamically updates when changing directories.
- Note that `~` refers to the directory in which the shell was started. Any directory/file outside the home directory is shown as an absolute path.

### 1.2 Input

- The shell supports all the bash commands and some custom commands listed below. Multiple commands chaining is supported using `;` or `&`, with robust input handling for random spaces and tabs.
- In case of an invalid command. `Invalid command: <error>` is printed.
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

- Any command invoked with `&` is treated as a background command. This implies that your shell will spawn that process but doesn’t hand the control of terminal to it. Hence, shell will keep taking other user commands.
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

- The `proclore` command provides information about a process, including PID, status, process group, virtual memory, and executable path. `Proclore does't work on Mac.`
- Process states :
  * R/R+ : Running
  * S/S+ : Sleeping in an interruptible wait
  * Z : Zombie
- "+" signifies a foreground process

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

## 2. Piping, Redirection, and Misc.

### 2.1 I/O Redirection

```
<mayankgoel@MayankGoel-2.local:~> echo "Hello world" > newfile.txt
<mayankgoel@MayankGoel-2.local:~> cat newfile.txt
Hello world
<mayankgoel@MayankGoel-2.local:~> wc < a.txt
1 2 12   
<mayankgoel@MayankGoel-2.local:~> echo "Lorem ipsum" > newfile.txt
<mayankgoel@MayankGoel-2.local:~> cat newfile.txt
Lorem ipsum
<mayankgoel@MayankGoel-2.local:~> echo "dolor sit amet" >> newfile.txt
Lorem ipsum
dolor sit amet
```

- The shell supports I/O redirection with `>` , `>>` and `<`.
  * `>` : Outputs to the filename following ">"
  * `>>` : Similar to ">" but appends instead of overwriting if the file already exists.
  * `<` : Reads input from the filename following "<"


### 2.2 Pipes
```
<mayankgoel@MayankGoel-2.local:~>  echo "Lorem Ipsum" | wc
1 2 12      # extra spaces can be present
<mayankgoel@MayankGoel-2.local:~>  echo "Lorem Ipsum" | wc | sed 's/ //g'
1212
```
- Pipes are supported for passing information between commands. The shell handles multiple pipes and executes commands sequentially.
- Note there must be commands to the left and right of the pipe symbol. Otherwise an error "Invalid use of pipe" will be printed.

### 2.3 Redirection along with Pipes
```
<mayankgoel@MayankGoel-2.local:~> cat a.txt
Lorem Ipsum
<mayankgoel@MayankGoel-2.local:~> cat < a.txt | wc | sed 's/ //g' | cat > b.txt
<mayankgoel@MayankGoel-2.local:~> cat b.txt
1212
```
The shell seamlessly integrates I/O redirection with pipes.


### 2.4 Activities
Syntax: `activities`
```
<mayankgoel@MayankGoel-2.local:~> activities
221 : emacs new.txt - Running
430 : vim - Stopped
620 : gedit - Stopped
```
- The `activities` command displays a list of running or stopped background processes spawned by the shell whcih includes the command name, PID, and the process state.


### 2.5 Signals
Syntax: `ping <pid> <signal>`
```
<mayankgoel@MayankGoel-2.local:~> activities
221 : emacs new.txt - Running
430 : vim - Stopped
620 : gedit - Stopped
<mayankgoel@MayankGoel-2.local:~> ping 221 9   # 9 is for SIGKILL
Sent signal 9 to process with pid 221
<mayankgoel@MayankGoel-2.local:~> activities
430 : vim - Stopped
620 : gedit - Stopped
```
- The `ping` command sends signals to specified processes given the PID. If the PID is not found, the error “No such process found” is printed.

- Keyboard shortcuts like `Ctrl-C`, `Ctrl-D` and `Ctrl-Z` are also supported.
  * `Ctrl - C`: Interrupt any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.
  * `Ctrl - D`: Logs out of the shell (after killing all processes) while having no effect on the actual terminal.
  * `Ctrl - Z`: Pushes the running foreground process (if any) to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.


### 2.6 fg command
Syntax: `fg <pid>`
```
<mayankgoel@MayankGoel-2.local:~> activities
620 : gedit - Stopped
<mayankgoel@MayankGoel-2.local:~> fg 620
# brings gedit [620] to foreground and change it's state to Running
```
- The `fg` command brings a background process (running or stopped) to the foreground.
- If no process with the given pid exists, the error “No such process found” is printed.


### 2.7 bg command
Syntax: `bg <pid>`
```
<mayankgoel@MayankGoel-2.local:~> activities
729 : htop - Stopped
<mayankgoel@MayankGoel-2.local:~> bg 729
# Changes [729] htop's state to Running (in the background).
```
- `bg` command changes the state of a background process to running (in the background). 
- If no process with the given pid exists, the error “No such process found” is printed.


### 2.8 Neonate Command
Syntax: `neonate -n <time_arg>`
```
<mayankgoel@MayankGoel-2.local:~> neonate -n 4
# A line containing the pid should be printed
# every 4 seconds until the user 
# presses the key: 'x'. 
11810
11811
11811
11812
11813 # key 'x' is pressed at this moment terminating the printing
```
- The `neonate` command prints the PID of the most recently created process at intervals until the user presses `x`.
- The command is intended mostly for debugging the shell.


## 3. Networking
### 3.1 iMan
Syntax:  `iMan <command_name>`
```
<mayankgoel@MayankGoel-2.local:~> iMan sleep
NAME
       sleep - delay for a specified amount of time

SYNOPSIS
       sleep NUMBER[SUFFIX]...
       sleep OPTION

DESCRIPTION
       Pause for NUMBER seconds.  SUFFIX may be 's' for seconds (the default),
       'm' for minutes, 'h' for hours or 'd' for days.  Unlike most  implemen-
       tations  that require NUMBER be an integer, here NUMBER may be an arbi-
       trary floating point number.  Given two or more  arguments,  pause  for
       the amount of time specified by the sum of their values.

       --help display this help and exit

       --version
              output version information and exit
```
- The `iMan` command fetches man pages from [http://man.he.net/](http://man.he.net/) using sockets. It displays information about the specified command or outputs an error if the page does not exist.


## 4. Commands Implementation overview:

| Commands             | Forked or not | Input Redirection | Output Redirection              |
| -------------------- | ------------- | ----------------- | ------------------------------- |
| `activities`       | yes           | no                | yes                             | 
| `proclore`         | yes           | no                | yes                             | 
| `bg`               | no            | no                | no                              |
| `fg`               | no            | no                | no                              |
| `iMan`             | yes           | no                | yes                             | 
| `neonate`          | yes           | no                | yes                             | 
| `ping`             | yes           | no                | yes                             | 
| `warp`             | no            | no                | yes                             | 
| `peek`             | yes           | no                | yes                             | 
| `seek`             | no            | no                | yes                             | 
| `pastevents`       | yes           | no                | yes                             | 
| `pastevents purge` | yes           | no                | yes (but no self-output to redirect) | 
| `builtins`             | yes           | yes               | yes                             |
