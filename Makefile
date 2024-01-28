##
# Project Title
#
# @file
# @version 0.1


cc = gcc -Wall -lm -lprinter

files = imagine.c

all: $(files)
	$(cc) -o main main.c $(files)

img:
	$(cc) -o main single.c $(files)
	./main

run: all
	./main

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --read-inline-info=yes -s ./main

clean:
	rm -rf ./main

# end
