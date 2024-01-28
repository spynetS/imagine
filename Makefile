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
	valgrind ./main

clean:
	rm -rf ./main

# end
