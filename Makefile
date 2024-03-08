

# choose your compiler
CC=gcc
#CC=gcc -Wall

mysh: shell.o builtins.o jobs.o 
	$(CC) -g shell.o builtins.o jobs.o -o mysh
#	$(CC) -g main.c sh.o get_path.o bash_getcwd.o -o mysh

shell.o: shell.c 
	$(CC) -g -c shell.c

#csapp.o: csapp.c csapp.h
#	$(CC) -g -c csapp.c

builtins.o: builtins.c builtins.h
	$(CC) -g -c builtins.c

jobs.o: jobs.c jobs.h
	$(CC) -g -c jobs.c

clean:
	rm -rf shell.o builtins.o jobs.o mysh
