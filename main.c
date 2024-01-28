#include <stdio.h>
#include "printer.h"
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const size_t NUM_PIXELS_TO_PRINT = 1000U;

typedef struct frame{
    char **frame;
    int strings;
    unsigned char* pixel_data;
    int width, height, comp;

} Frame;


void print_frame(Frame* frame);

Frame *newFrame(char *path){
    Frame *new_frame  = malloc(sizeof(Frame));
    new_frame->width  = 0;
    new_frame->height = 0;
    new_frame->comp   = 0;

    unsigned char *data = stbi_load(path, &(new_frame->width), &(new_frame->height), &(new_frame->comp), 0);
    new_frame->pixel_data = data;
    new_frame->strings = 0;

    printf("%d \n", new_frame->width);

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
    if(prev_frame == NULL || new_frame == NULL ) {
        print_frame(new_frame);
    }

    int count = 0;
    for(int i = 0; i < new_frame->strings; i ++){
        /* printf("%s",prev_frame->frame[i]); */
        /* msleep(0.5); */
        if(strcmp(new_frame->frame[i], prev_frame->frame[i]) != 0){
            int x = i % new_frame->width;
            int y = i / new_frame->width;
            /* printf("%dx%d\n",x,y); */
            setCharAt(x, y, new_frame->frame[i]);
        }
        /* if(i % new_frame->width == 0) */
        /*     puts(""); */
    }
    /* printf("changed %d pixels of %d\n",count, new_frame->strings); */
}

void print_frame(Frame* frame){
    int count = 0;
    puts("FIRST FRAME");
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

int drawImage(Frame *frame, int size){
    char *mychars = " .:-=+*#%@";
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
            sprintf(pixel,"%s%c"RESET,color,mychars[(data[i]*(strlen(mychars)-1)/255)]);

            frame->frame[frame->strings] = pixel;
            frame->strings++;
        }
    return 1;
}

int main() {

    char *path = "oppenheimer";
    Frame *prev_frame = NULL;
    Frame *new_frame = NULL;

    for(int i = 1; i < 500; i +=1){
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
        drawImage(new_frame,1);
        if(prev_frame != NULL)
            draw_frame(prev_frame,new_frame);
        else
            puts("NULL");

        printf("\nframe %s\n", str);
        msleep(33);
        //system("clear");
    }
    free_frame(new_frame);
    free_frame(prev_frame);

    return 0;
}
