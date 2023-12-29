# Write a make file for this directory

BINARY_DIR=src
OBJECT_DIR=object_files

build: create_folder shell

create_folder:
	mkdir -p $(OBJECT_DIR)

shell: $(OBJECT_DIR)/main.o $(OBJECT_DIR)/neonate.o $(OBJECT_DIR)/iman.o $(OBJECT_DIR)/raw_mode.o $(OBJECT_DIR)/signal.o $(OBJECT_DIR)/piping.o $(OBJECT_DIR)/pastevents.o $(OBJECT_DIR)/activities.o $(OBJECT_DIR)/helper_functions.o $(OBJECT_DIR)/proclore.o $(OBJECT_DIR)/seek.o $(OBJECT_DIR)/color.o $(OBJECT_DIR)/peek.o $(OBJECT_DIR)/input_handling.o $(OBJECT_DIR)/path_handling.o $(OBJECT_DIR)/bg_process.o $(OBJECT_DIR)/input_redirection.o
	gcc -o shell $(OBJECT_DIR)/piping.o $(OBJECT_DIR)/neonate.o $(OBJECT_DIR)/activities.o $(OBJECT_DIR)/signal.o $(OBJECT_DIR)/raw_mode.o $(OBJECT_DIR)/iman.o $(OBJECT_DIR)/proclore.o $(OBJECT_DIR)/helper_functions.o $(OBJECT_DIR)/input_redirection.o $(OBJECT_DIR)/main.o $(OBJECT_DIR)/bg_process.o $(OBJECT_DIR)/color.o $(OBJECT_DIR)/seek.o $(OBJECT_DIR)/pastevents.o $(OBJECT_DIR)/peek.o $(OBJECT_DIR)/input_handling.o $(OBJECT_DIR)/path_handling.o 
	rm -rf $(OBJECT_DIR)

$(OBJECT_DIR)/color.o: $(BINARY_DIR)/color.c
	gcc -O -c -Wall $(BINARY_DIR)/color.c -o $(OBJECT_DIR)/color.o

$(OBJECT_DIR)/iman.o: $(BINARY_DIR)/iman.c
	gcc -O -c -Wall $(BINARY_DIR)/iman.c  -o $(OBJECT_DIR)/iman.o

$(OBJECT_DIR)/raw_mode.o: $(BINARY_DIR)/raw_mode.c
	gcc -O -c -Wall $(BINARY_DIR)/raw_mode.c  -o $(OBJECT_DIR)/raw_mode.o

$(OBJECT_DIR)/neonate.o: $(BINARY_DIR)/neonate.c
	gcc -O -c -Wall $(BINARY_DIR)/neonate.c  -o $(OBJECT_DIR)/neonate.o

$(OBJECT_DIR)/seek.o: $(BINARY_DIR)/seek.c
	gcc -O -c -Wall $(BINARY_DIR)/seek.c  -o $(OBJECT_DIR)/seek.o

$(OBJECT_DIR)/pastevents.o: $(BINARY_DIR)/pastevents.c
	gcc -O -c -Wall $(BINARY_DIR)/pastevents.c  -o $(OBJECT_DIR)/pastevents.o

$(OBJECT_DIR)/peek.o: $(BINARY_DIR)/peek.c
	gcc -O -c -Wall $(BINARY_DIR)/peek.c  -o $(OBJECT_DIR)/peek.o

$(OBJECT_DIR)/input_handling.o: $(BINARY_DIR)/input_handling.c
	gcc -O -c -Wall $(BINARY_DIR)/input_handling.c  -o $(OBJECT_DIR)/input_handling.o

$(OBJECT_DIR)/helper_functions.o: $(BINARY_DIR)/helper_functions.c
	gcc -O -c -Wall $(BINARY_DIR)/helper_functions.c  -o $(OBJECT_DIR)/helper_functions.o

$(OBJECT_DIR)/path_handling.o: $(BINARY_DIR)/path_handling.c
	gcc -O -c -Wall $(BINARY_DIR)/path_handling.c  -o $(OBJECT_DIR)/path_handling.o

$(OBJECT_DIR)/proclore.o: $(BINARY_DIR)/proclore.c
	gcc -O -c -Wall $(BINARY_DIR)/proclore.c  -o $(OBJECT_DIR)/proclore.o

$(OBJECT_DIR)/activities.o: $(BINARY_DIR)/activities.c
	gcc -O -c -Wall $(BINARY_DIR)/activities.c  -o $(OBJECT_DIR)/activities.o

$(OBJECT_DIR)/bg_process.o: $(BINARY_DIR)/bg_process.c
	gcc -O -c -Wall $(BINARY_DIR)/bg_process.c  -o $(OBJECT_DIR)/bg_process.o

$(OBJECT_DIR)/signal.o: $(BINARY_DIR)/signal.c
	gcc -O -c -Wall $(BINARY_DIR)/signal.c  -o $(OBJECT_DIR)/signal.o

$(OBJECT_DIR)/input_redirection.o: $(BINARY_DIR)/input_redirection.c
	gcc -O -c -Wall $(BINARY_DIR)/input_redirection.c  -o $(OBJECT_DIR)/input_redirection.o

$(OBJECT_DIR)/piping.o: $(BINARY_DIR)/piping.c
	gcc -O -c -Wall $(BINARY_DIR)/piping.c  -o $(OBJECT_DIR)/piping.o

$(OBJECT_DIR)/main.o: $(BINARY_DIR)/main.c
	gcc -O -c -Wall $(BINARY_DIR)/main.c -o $(OBJECT_DIR)/main.o

clean:
	rm -rf *.o shell a.out
	rm -rf $(OBJECT_DIR)