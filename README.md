

## List of assumptions-

* Reasonable assumptions have been made for the maximum length of commands, filenames etc. The lengths for the same can be found out in the main.c file
* Precedence order followed - \\"' ;  & | << < > other characters, strings. Any symbol after \ is ignore similar to bash.
* Multiple random spaces and tab spaces have been handled.
* The input string is converted into lowercase beforehand.
* Up and down arrows can be used to scroll through the command history similar to bash.
* The history is stored in .CShell_history file.
* cd command won't execute. Instead use warp to change directories. If the shell has just started, warp - won't change any directory.
* .. . ~ - can be used for referencing directories similar to bash.

## Compiling

```
make
./a.out
```
