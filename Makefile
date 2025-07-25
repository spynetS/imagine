#
# Project Title
#
# @file
# @version 0.1


cc = gcc -Wall -lm
CFLAGS=-march=x86-64 -mtune=generic -O2 -pipe -fno-plt -D_FORTIFY_SOURCE=2

out_folder = ./bin
out = $(out_folder)/imagine

src = $(wildcard src/*.c)

objects = $(prefix bin ) $(notdir $(src:.c=.o))
bin_objects = $(addprefix bin/, $(objects))


bin/%.o: src/%.c
	if [ -d "bin" ]; then echo "Dir exists"; else mkdir bin; fi
	$(cc) $(CFLAGS) -c $< -o $@

$(out): $(bin_objects)
	$(cc) $(bin_objects) ./lib/libprinter.a -o $(out)

run: $(out)
	./$(out)

clean:
	rm -rf $(out_folder)/*

# end
