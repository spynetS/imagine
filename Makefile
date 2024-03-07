#
# Project Title
#
# @file
# @version 0.1


cc = gcc -Wall -lm

files = ./src/imagine.c ./src/flagcer.c ../printer/build/libprinter.a -lm
out = imager

all: $(files)
	$(cc) -o $(out) ./src/main.c $(files)


video: ./src/video.c $(files)
	$(cc) -o video ./src/video.c $(files)

run: all
	./$(out)

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --read-inline-info=yes -s ./$(out)

clean:
	rm -rf ./$(out)

# end
