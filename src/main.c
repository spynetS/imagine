
//Alfred Roos 2023

#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "flagser.h"

Settings settings;

int isDirectory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);

    // Check if the path corresponds to a directory
    return S_ISDIR(path_stat.st_mode);
}

void setOption(int av,char **ac){
    settings.character_mode = atoi(ac[1]);
}
void setColor(int av,char **ac){
    settings.color = atoi(ac[1]);
}

void setRes(int av,char **ac){
    settings.width = atoi(ac[1]);
    settings.height = atoi(ac[2]);
}

int main(int argv, char **argc) {

    settings.character_mode = 3;
    settings.color = 1;

    settings.max_width = termWidth();
    settings.max_height = termHeight();

    settings.path = argc[argv-1];

    addFlag("-t", "--type", "Sets the type to be outputed as\n 0 ascii, 1 assci more detail, 2 unicode block", setOption);
    addFlag("-c", "--color", "Sets the if we should output with color (escape code)\n 0 no color, 1 forground color", setColor);
    addFlag("-w", "--width", "Set the width ", setRes);

    addHelp();
    parse(argv,argc);


    if (isDirectory(settings.path)) {

        printf("width %d\n", termWidth());
        print_folder(&settings);
    }
    else{
        print_image(&settings);
    }

    return 0;
}
