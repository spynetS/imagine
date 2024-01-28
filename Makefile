##
# Project Title
#
# @file
# @version 0.1


cc = gcc

files = main.c -lm -lprinter

all: $(files)
	$(cc) -o main $(files)

run: all
	./main

clean:
	rm -rf ./main

# end
