#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int isDirectory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);

    // Check if the path corresponds to a directory
    return S_ISDIR(path_stat.st_mode);
}

int main(int argv, char **argc) {

    char *path = argc[1];
    if (isDirectory(path)) {
        print_folder(path,2,1);
    }
    else{
        print_image(path, 0,0);
    }

    return 0;
}
