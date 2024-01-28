#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"
#include "string.h"


void draw_frame(Frame *prev_frame, Frame *new_frame){

    if(prev_frame == NULL){
        print_frame(new_frame);
        return;
    }

    int offsetX = 0;
    int offsetY = 0;
    char *mychars = " .isk@";

    int x = 0;
    int y = 0;

    for(int i = 0; i < new_frame->width*new_frame->height*new_frame->comp; i +=new_frame->comp){
            if(new_frame->pixel_data[i] != prev_frame->pixel_data[i]){
                char str[50];
                char color_str[28] = "";
                //sprintf(color,"\x1b[38;5;%dm",data[i]);
                sprintf(color_str,"\033[38;2;%d;%d;%dm",new_frame->pixel_data[i],new_frame->pixel_data[i+1],new_frame->pixel_data[i+2]);
                sprintf(str,"%s██"RESET, color_str);
                /* sprintf(str,"%s%c "RESET,color_str,mychars[(new_frame->pixel_data[i]*(strlen(mychars)-1)/255)]); */
                setCharAt(x+offsetX,y+offsetY,str);
            }
        x += 2;
        if(x == new_frame->width*2){
            x = 0;
            y++;
        }
    }
    /* for(int y = 0; y < new_frame->height;y++){ */
    /*     for(int x = 0; x < new_frame->width*2;x+=2){ */
    /*         if(new_frame->pixel_data[i] != prev_frame->pixel_data[i]){ */
    /*             char str[50]; */
    /*             char color_str[28] = ""; */
    /*             //sprintf(color,"\x1b[38;5;%dm",data[i]); */
    /*             switch(2){ */
    /*                 case 1: */
    /*                     sprintf(color_str,"\033[38;2;%d;%d;%dm",new_frame->pixel_data[i],new_frame->pixel_data[i+1],new_frame->pixel_data[i+2]); */
    /*                     break; */
    /*                 case 2: */
    /*                     sprintf(color_str,"\033[48;2;%d;%d;%dm",new_frame->pixel_data[i],new_frame->pixel_data[i+1],new_frame->pixel_data[i+2]); */
    /*                     break; */
    /*             } */

    /*             sprintf(str,"%s█", color_str); */
    /*             setCharAt(x+offsetX,y+offsetY,str); */
    /*         } */
    /*         i++; */
    /*     } */
    /* } */
}
void print_frame(Frame* frame){
    int count = 0;
    char *mychars = " .isk@";
    printf("FIRST FRAME %d", frame->comp);
    for(int i = 0; i < frame->width*frame->height*frame->comp; i +=frame->comp){
        char str[50];
        char color_str[28] = "";
        //sprintf(color,"\x1b[38;5;%dm",data[i]);
        sprintf(color_str,"\033[38;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]);
        /* switch(2){ */
        /*     case 1: */
        /*         sprintf(color_str,"\033[38;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]); */
        /*         break; */
        /*     case 2: */
        /*         sprintf(color_str,"\033[48;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]); */
        /*         break; */
        /* } */

        printf("%s██"RESET, color_str);
        /* printf("%s%c"RESET,color_str,mychars[(frame->pixel_data[i]*(strlen(mychars)-1)/255)]); */
        count ++;
        if(count == frame->width){
            count = 0;
            printf("\n");
        }
    }
}

int main(int argv, char **argc) {

    char *path = argc[1];
    Frame *prev_frame = NULL;
    Frame *new_frame = NULL;
    system("clear");

    puts(HIDE_CURSOR);

    for(int i = 100; i < 3500; i +=1){
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

        if(prev_frame != NULL)
            draw_frame(prev_frame,new_frame);
        else{
            print_frame(new_frame);
        }
        msleep(30);
    }
    free_frame(new_frame);
    free_frame(prev_frame);

    puts(SHOW_CURSOR);
    return 0;
}
