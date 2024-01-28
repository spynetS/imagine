#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imagine.h"
#include "flagser.h"
int option = 0;
int color = 0;

void setOption(int argv, char** argc){
    option = atoi(argc[1]);
}
void setColor(int argv, char** argc){
    color = atoi(argc[1]);
}

int main(int argv,char ** argc){
    if(argv > 1){

        addFlag("-o","--option","Sets the charecter to print",setOption);
        addFlag("-c","--color","Sets the color",setColor);

        addHelp();
        parse(argv,argc);

        char path[40];
        strcpy(path,argc[argv-1]);
        printf("%d %s\n",option,path);

        Frame *frame = newFrame(path);
        load_frame(frame,option,color);
        draw_frame(NULL,frame);
        free_frame(frame);
    }
    else{
        puts("No image was provided");
    }
}
