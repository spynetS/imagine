#
# Project Title
#
# @file
# @version 0.1


cc = gcc -Wall -lm -lprinter

files = ./src/imagine.c ./src/flagcer.c
out = imagine

all: $(files)
	$(cc) -o $(out) ./src/main.c $(files)

img:
	$(cc) -o $(out) single.c $(files) flagcer.c

run: all
	./$(out)

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --read-inline-info=yes -s ./$(out)

clean:
	rm -rf ./$(out)

# end
