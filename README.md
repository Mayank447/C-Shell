<h1 align="center">
  C-Shell
</h1>

<h4 align="center">A Custom Shell written in C based on bash semantics </h4>

### Building and running

Run `make` in the root directory to build. Run `./shell` to run the shell.

### Navigating the directory structure

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

- Precedence order followed `\ " ' ;  & | << < >` in decreasing order. Any symbol after \ is ignore similar to bash.

- `;` and `&` can be used to chain commands. `;` runs the previous command in foreground. `&` runs the previous command in background.

- Pipe symbol `|` can be used for piping commands similar to bash.

- Input-output redirection can be performed using `>`, `>>` and `<` similar to Bash.

- `.. . ~ -` can be used for referencing directories similar to bash.

- &#8593; and &#8595; arrows can be used to scroll through the command history similar to bash.


## Assumptions:

- Multiple spaces and tab spaces are ignored.

- Commands, files, folders are case-insensitive.

- The history is stored in a hidden file `.CShell_history`.

- `cd` command won't chnage directories because of forking. Instead use `warp` command to change directories.

- If the shell has just started, `warp` - won't change any directory.

- Reasonable assumptions have been made for the maximum length of commands, filenames, history etc. The lengths for the same can be found out in the `include/params.h` file.


### Compatibility

- `sigabbrev_np` and `sigdescr_np` are used which may not be available on all systems

### Commands overview

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

### Pipe and Redirection

- prioritizing piping over redirection [bash has a proper grammar and handles things differently]
- `warp()`, `fg()` and `bg()` wont work with piping

### Inbuilt functions

- `sed` will work with piping given you don't include the replace logic in quotes (this probably happens because)

```bash
echo "Hello there" | sed 's/hello/hi/' | sed 's/there/OSN/'
```

The above command will not work and output an error message but the below command will give a valid output.

```bash
echo "hello there" | sed s/hello/hi/ | sed s/there/OSN/
"hi OSN" # output 
```

### Custom functions

#### **`pastevents`**

- commands stored in history are properly formatted with spaces. The raw string is not stored.
- regardless of a commands' validity, it will be stored in history
- history will never store the command line in history if there is even a single `pastevents` or `pastevents purge`, irrespective of the remaining command.
  Example:

```bash
pastevents | pastevents execute 7 | wc > output.txt
```

The above command won't be stored in history.

```bash
pastevents execute 7 | wc > output.txt
```

The above command will be stored in history after replacing `pastevents execute 7` with the 7th command stored in history

#### **`proclore`**

- Earlier, I was forking `proclore` to run it but it never showed the parent shell as $R+$ but as $S/S+$ since at the moment of execution, the shell is not in foreground but the child process is. This does not mean that the implementation is wrong. But due to my implementation of piping and all, I decided to keep this so I could do piping with `proclore`.

#### **`warp`**

- will work with output redirection

#### **`iMan`**

- in `iMan` , grep is very large so can't be malloc that much space. This is my output of `realloc()` again and again

```bash
iMan grep
# output: cur_size is the realloc size() called
cur_size = 2048
cur_size = 4096
cur_size = 6144
cur_size = 8192
cur_size = 10240
cur_size = 12288
cur_size = 14336
cur_size = 16384
cur_size = 18432
realloc(): invalid next size
```

what I am doing is reading until a max size and displaying whatever information I fetched. This is to keep everything simple.

### Some assumptions

- `getenv()` and `setenv()` have been used to set `HOME` , `PWD` and `OLDPWD` environment variables. None others have been utilized.
- to keep everything atomic, the changes are not made immediately but in the next iteration of while loop

### Things to keep in mind while giving input

- `;` and `&` can be chained in similar ways to bash

### Below are a few bash specific things

- ctrl-d does not work while sleep is in foreground
- ctrl-d , ctrl-z are disabled while taking input
- ctrl-c works just fine while taking input. It moves to a newline

### Some Resources

## Compiling

```
make
./a.out
```
