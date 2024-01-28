#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"
#include "string.h"



int main(int argv, char **argc) {

    char *path = argc[1];
    Frame *prev_frame = NULL;
    Frame *new_frame = NULL;
    system("clear");

    puts(HIDE_CURSOR);

    for(int i = 2; i < 10; i +=1){
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

        if(prev_frame != NULL){
            draw_frame(prev_frame,new_frame,2,1);
        }
        else{
            print_frame(new_frame,2,1);
        }
        msleep(0);
        /* system("clear"); */
    }
    free_frame(new_frame);
    free_frame(prev_frame);

    puts(SHOW_CURSOR);
    printf("\\n\n\n\n\n\n\n\n\n\nn");
    return 0;
}
