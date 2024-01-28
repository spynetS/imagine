##
# Project Title
#
# @file
# @version 0.1


cc = gcc -Wall

files = main.c -lm -lprinter

all: $(files)
	$(cc) -o main $(files)

run: all
	./main

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --read-inline-info=yes -s ./main

clean:
	rm -rf ./main

# end
