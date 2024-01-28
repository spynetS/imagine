#include <stdio.h>
#include "printer.h"
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const size_t NUM_PIXELS_TO_PRINT = 1000U;

typedef struct frame{
    char **frame;
    unsigned char* pixel_data;
    int width, height, comp;

} Frame;


Frame *newFrame(char *path){
    Frame *new_frame = malloc(sizeof(Frame));
    unsigned char *data = stbi_load(path, &new_frame->width, &new_frame->height, &new_frame->comp, 0);
    new_frame->pixel_data = data;

    new_frame->frame = malloc(sizeof(char*)*new_frame->width*new_frame->height);


    return new_frame;
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

    if (data){

        for (size_t i = 0; i < (width*height)*comp; i+=comp*size) {

            char pixel[100];
            char color[58];
            //sprintf(color,"\x1b[38;5;%dm",data[i]);
            sprintf(color,"\033[38;2;%d;%d;%dm",data[i],data[i+1],data[i+2]);
            sprintf(pixel,"%s%c"RESET,color,mychars[(data[i]*(strlen(mychars)-1)/255)]);

            printf("%s",pixel);
            count++;
            if(count == width){
                printf("\n");
                count = 0;
            }
        }
    }
    return 1;
}

int main() {

    char *path = "oppenheimer";

    for(int i = 0; i < 2840; i +=1){
        char str[10];
        if(i > 999)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 99)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 9)
            sprintf(str,"./%s/0%d.png",path,i);
        else
            sprintf(str,"./%s/00%d.png",path,i);

        Frame *new_frame = newFrame(str);
        drawImage(new_frame,1);

        printf("\nframe %s\n", str);
        msleep(30);
        system("clear");
    }
    return 0;
}
