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


const size_t NUM_PIXELS_TO_PRINT = 1000U;

void print_frame(Frame* frame);

Frame *newFrame(char *path){
    Frame *new_frame  = malloc(sizeof(Frame));
    new_frame->width  = 0;
    new_frame->height = 0;
    new_frame->comp   = 0;

    unsigned char *data = stbi_load(path, &(new_frame->width), &(new_frame->height), &(new_frame->comp), 0);
    new_frame->pixel_data = data;
    new_frame->strings = 0;


    new_frame->frame = malloc(sizeof(char*)*(new_frame->width*new_frame->height)+1);

    return new_frame;
}

void free_frame(Frame* frame){
    for(int i = 0; i < frame->strings; i ++){
        free(frame->frame[i]);
    }
    free(frame->frame);
    free(frame->pixel_data);
    free(frame);
}

void draw_frame(Frame* prev_frame, Frame *new_frame){
    if(prev_frame == NULL){
        print_frame(new_frame);
        return;
    }
    if(new_frame == NULL){
        print_frame(prev_frame);
        return;
    }
    int i = 0;
    int offsetX = 0;
    int offsetY = 0;

    for(int i = 0; i < new_frame->width*new_frame->height; i ++){
        int x = i % new_frame->width;
        int y = i / new_frame->width;
        if(strcmp(new_frame->frame[i], prev_frame->frame[i]) != 0){
            setCharAt(x+offsetX,y+offsetY,new_frame->frame[i]);
        }
    }

    /* for(int y = 0; y < new_frame->height;y++){ */
    /*     for(int x = 0; x < new_frame->width;x+=2){ */
    /*         if(strcmp(new_frame->frame[i], prev_frame->frame[i]) != 0){ */
    /*             setCharAt(x+offsetX,y+offsetY,new_frame->frame[i]); */
    /*         } */
    /*         i++; */
    /*     } */
    /* } */
}

void print_frame(Frame* frame){
    int count = 0;
    printf("FIRST FRAME %d", frame->comp);
    for(int i = 0; i < frame->strings; i ++){
        printf("%s",frame->frame[i]);
        count ++;
        if(count == frame->width){
            count = 0;
            printf("\n");
        }
    }
}

int getBrightness(int red, int green, int blue){
    //0.2126*R + 0.7152*G + 0.0722*B
    if(red < 0) red = 0;
    if(green < 0) green = 0;
    if(blue < 0) blue = 0;
    /* return (int)(0.299 * red + 0.587 * green + 0.114 * blue); */
    return (int)(red +green + blue);
}

int load_frame(Frame *frame, int size){
    char *mychars = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
    /* char *mychars = " .isk@"; */
    int width = frame->width;
    int height = frame->height;
    int comp = frame->comp;
    unsigned char * data = frame->pixel_data;

    int count = 0;
        for (size_t i = 0; i < (width*height)*comp; i+=comp*size) {

            char *pixel = malloc(sizeof(char)*50);
            char color[28];
            //sprintf(color,"\x1b[38;5;%dm",data[i]);
            sprintf(color,"\033[38;2;%d;%d;%dm",data[i],data[i+1],data[i+2]);
            /* sprintf(pixel,"%s%c "RESET,"",mychars[(data[i]*(strlen(mychars)-1)/255)]); */
            sprintf(pixel,"%s█"RESET,color);

            frame->frame[frame->strings] = pixel;
            frame->strings++;
        }
    return 1;
}
