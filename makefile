# Write a make file for this directory

shell: shell.o history.o proclore.o find.o color.o ls.o input_handling.o path_handling.o bg_process.o
	gcc -o shell proclore.o shell.o bg_process.o color.o find.o history.o ls.o input_handling.o path_handling.o 

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

proclore.o: proclore.c
	gcc -O -c -Wall proclore.c

bg_process.o: bg_process.c
	gcc -O -c -Wall bg_process.c

shell.o: shell.c
	gcc -O -c -Wall shell.c

clean:
	rm -f *.o shell