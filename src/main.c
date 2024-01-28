#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "flagser.h"

int isDirectory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);

    // Check if the path corresponds to a directory
    return S_ISDIR(path_stat.st_mode);
}

int character = 0;
int color = 1;

void setOption(int av,char **ac){
    character = atoi(ac[1]);
}
void setColor(int av,char **ac){
    color = atoi(ac[1]);
}


int main(int argv, char **argc) {


    addFlag("-t", "--type", "Sets the type to be outputed as\n 0 ascii, 1 assci more detail, 2 unicode block", setOption);
    addFlag("-c", "--color", "Sets the if we should output with color (escape code)\n 0 no color, 1 forground color", setColor);

    addHelp();
    parse(argv,argc);

    char *path = argc[argv-1];
    puts(path);
    if (isDirectory(path)) {
        print_folder(path,character,color);
    }
    else{
        print_image(path, character,color);
    }

    return 0;
}
