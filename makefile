# Write a make file for this directory

shell: main.o iman.o raw_mode.o signal_handling.o piping.o history.o activities.o helper_functions.o proclore.o find.o color.o ls.o input_handling.o path_handling.o bg_process.o input_redirection.o
	gcc -o shell piping.o activities.o signal_handling.o raw_mode.o iman.o proclore.o helper_functions.o input_redirection.o main.o bg_process.o color.o find.o history.o ls.o input_handling.o path_handling.o 

color.o: color.c
	gcc -O -c -Wall color.c

iman.o: iman.c
	gcc -O -c -Wall iman.c

raw_mode.o: raw_mode.c
	gcc -O -c -Wall raw_mode.c

find.o: find.c
	gcc -O -c -Wall find.c

history.o: history.c
	gcc -O -c -Wall history.c

ls.o: ls.c
	gcc -O -c -Wall ls.c

input_handling.o: input_handling.c
	gcc -O -c -Wall input_handling.c

helper_functions.o: helper_functions.c
	gcc -O -c -Wall helper_functions.c

path_handling.o: path_handling.c
	gcc -O -c -Wall path_handling.c

proclore.o: proclore.c
	gcc -O -c -Wall proclore.c

activities.o: activities.c
	gcc -O -c -Wall activities.c

bg_process.o: bg_process.c
	gcc -O -c -Wall bg_process.c

signal_handling.o: signal_handling.c
	gcc -O -c -Wall signal_handling.c

input_redirection.o: input_redirection.c
	gcc -O -c -Wall input_redirection.c

piping.o: piping.c
	gcc -O -c -Wall piping.c

main.o: main.c
	gcc -O -c -Wall main.c

clean:
	rm -f *.o shell