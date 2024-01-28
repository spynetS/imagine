#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"

int main(int argv, char **argc) {

    char *path = argc[1];
    Frame *prev_frame = NULL;
    Frame *new_frame = NULL;
    system("clear");

    puts(HIDE_CURSOR);

    for(int i = 1500; i < 3500; i +=1){
        char str[25];
        if(i > 999)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 99)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 9)
            sprintf(str,"./%s/0%d.png",path,i);
        else
            sprintf(str,"./%s/00%d.png",path,i);

        if(new_frame != NULL){
            if(prev_frame != NULL){
                free_frame(prev_frame);
            }
            prev_frame = new_frame;
        }

        new_frame = newFrame(str);
        load_frame(new_frame,0,2);

        if(prev_frame != NULL)
            draw_frame(prev_frame,new_frame);
        else
            print_frame(new_frame);

        msleep(33);
    }
    free_frame(new_frame);
    free_frame(prev_frame);

    puts(SHOW_CURSOR);
    return 0;
}
