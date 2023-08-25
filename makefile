# Write a make file for this directory

shell: shell.o history.o ls_command.o input_handling.o path_handling.o
	gcc -o shell shell.o history.o ls_command.o input_handling.o path_handling.o 

history.o: history.c
	gcc -O -c -Wall history.c

ls_command.o: ls_command.c
	gcc -O -c -Wall ls_command.c

input_handling.o: input_handling.c
	gcc -O -c -Wall input_handling.c

path_handling.o: path_handling.c
	gcc -O -c -Wall path_handling.c

shell.o: shell.c
	gcc -O -c -Wall shell.c

clean:
	rm -f *.o shell