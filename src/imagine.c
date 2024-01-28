#include <stdio.h>
#include "printer.h"
#include "imagine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
**         #TODO
**  - scale image
**  - different options
**  - change frame on gpu
**  - play video without self extracting frames
**  - music
*/


Frame *newFrame(char *path){
    Frame *new_frame  = malloc(sizeof(Frame));
    new_frame->width  = 0;
    new_frame->height = 0;
    new_frame->comp   = 0;

    unsigned char *data = stbi_load(path, &(new_frame->width), &(new_frame->height), &(new_frame->comp), 0);

    new_frame->pixel_data = data;

    return new_frame;
}

void free_frame(Frame* frame){
    free(frame->pixel_data);
    free(frame);
}

void draw_frame(Frame *prev_frame, Frame *new_frame, int characters, int color){

    if(prev_frame == NULL){
        print_frame(new_frame,2,1);
        return;
    }

    int offsetX = 0;
    int offsetY = 0;
    char *mychars = " .isk@";

    int x = 0;
    int y = 0;

    for(int i = 0; i < new_frame->width*new_frame->height*new_frame->comp; i +=new_frame->comp){
            if(new_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
                new_frame->pixel_data[i+1] != prev_frame->pixel_data[i+1] ||
                new_frame->pixel_data[i+2] != prev_frame->pixel_data[i+2]){

                char str[50];
                char color_str[28] = "";
                switch(color){
                    case 1:
                        sprintf(color_str,"\033[38;2;%d;%d;%dm",new_frame->pixel_data[i],new_frame->pixel_data[i+1],new_frame->pixel_data[i+2]);
                        break;

                }
                switch(characters){
                    case 0:
                        sprintf(str,"%s%c "RESET,color_str,mychars[(new_frame->pixel_data[i]*(strlen(mychars)-1)/255)]);
                        break;
                    case 2:
                        sprintf(str,"%s██"RESET, color_str);
                        break;
                }
                setCharAt(x+offsetX,y+offsetY,str);
            }
        x += 2;
        if(x == new_frame->width*2){
            x = 0;
            y++;
        }
    }
}

void print_frame(Frame* frame, int characters, int color){
    int count = 0;
    char *mychars = " .isk@";
    for(int i = 0; i < frame->width*frame->height*frame->comp; i +=frame->comp){
        char str[50];
        char color_str[28] = "";
        switch(color){
            case 1:
                sprintf(color_str,"\033[38;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]);
                break;
        }
        switch(characters){
            case 0:
                printf("%s%c "RESET,color_str,mychars[(frame->pixel_data[i]*(strlen(mychars)-1)/255)]);
                break;
            case 2:
                printf("%s██"RESET, color_str);
                break;
        }
        count ++;
        if(count == frame->width){
            count = 0;
            printf("\n");
        }
    }
}
/* void draw_frame(Frame* prev_frame, Frame *new_frame){ */
/*     if(prev_frame == NULL){ */
/*         print_frame(new_frame); */
/*         return; */
/*     } */
/*     if(new_frame == NULL){ */
/*         print_frame(prev_frame); */
/*         return; */
/*     } */
/*     int i = 0; */
/*     int offsetX = 0; */
/*     int offsetY = 0; */

/*     for(int y = 0; y < new_frame->height;y++){ */
/*         for(int x = 0; x < new_frame->width*2;x+=2){ */
/*             if(strcmp(new_frame->frame[i], prev_frame->frame[i]) ){ */
/*                 setCharAt(x+offsetX,y+offsetY,new_frame->frame[i]); */
/*             } */
/*             i++; */
/*         } */
/*     } */
/* } */

/* void print_frame(Frame* frame){ */
/*     int count = 0; */
/*     printf("FIRST FRAME %d", frame->comp); */
/*     for(int i = 0; i < frame->strings; i ++){ */
/*         printf("%s",frame->frame[i]); */
/*         count ++; */
/*         if(count == frame->width){ */
/*             count = 0; */
/*             printf("\n"); */
/*         } */
/*     } */
/* } */

/* int draw_frame(Frame *frame, int option, int color){ */
/*     char *mychars_detail = " `.-':_,^=;><+!rc*\/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@"; */
/*     char *mychars = " .isk@"; */
/*     int width = frame->width; */
/*     int height = frame->height; */
/*     int comp = frame->comp; */
/*     unsigned char * data = frame->pixel_data; */

/*         for (size_t i = 0; i < (width*height)*comp; i+=comp) { */

/*             char *pixel = malloc(sizeof(char)*50); */
/*             char color_str[28] = ""; */
/*             //sprintf(color,"\x1b[38;5;%dm",data[i]); */
/*             switch(color){ */
/*                 case 1: */
/*                     sprintf(color_str,"\033[38;2;%d;%d;%dm",data[i],data[i+1],data[i+2]); */
/*                     break; */
/*                 case 2: */
/*                     sprintf(color_str,"\033[48;2;%d;%d;%dm",data[i],data[i+1],data[i+2]); */
/*                     break; */
/*             } */

/*             switch(option){ */
/*                 case 0: */
/*                     sprintf(pixel,"%s%c "RESET,color_str,mychars[(data[i]*(strlen(mychars)-1)/255)]); */
/*                     break; */
/*                 case 1: */
/*                     sprintf(pixel,"%s%c "RESET,color_str,mychars_detail[(data[i]*(strlen(mychars)-1)/255)]); */
/*                     break; */
/*                 case 2: */
/*                     sprintf(pixel,"%s██"RESET,color_str); */
/*                     break; */
/*             } */

/*             frame->frame[frame->strings] = pixel; */
/*             frame->strings++; */
/*         } */
/*     return 1; */
/* } */
