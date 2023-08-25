# Write a make file for this directory

shell: shell.o history.o find.o color.o ls.o input_handling.o path_handling.o
	gcc -o shell shell.o color.o find.o history.o ls.o input_handling.o path_handling.o 

color.o: color.c
	gcc -O -c -Wall color.c

find.o: find.c
	gcc -O -c -Wall find.c

history.o: history.c
	gcc -O -c -Wall history.c

ls.o: ls.c
	gcc -O -c -Wall ls.c

input_handling.o: input_handling.c
	gcc -O -c -Wall input_handling.c

path_handling.o: path_handling.c
	gcc -O -c -Wall path_handling.c

shell.o: shell.c
	gcc -O -c -Wall shell.c

clean:
	rm -f *.o shell