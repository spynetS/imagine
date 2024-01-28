##
# Project Title
#
# @file
# @version 0.1


cc = gcc -Wall -lm -lprinter

files = ./src/imagine.c

all: $(files)
	$(cc) -o main ./src/main.c $(files)

img:
	$(cc) -o main single.c $(files) flagcer.c

run: all
	./main

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --read-inline-info=yes -s ./main

clean:
	rm -rf ./main

# end
