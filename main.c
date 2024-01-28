//#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "printer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const size_t NUM_PIXELS_TO_PRINT = 1000U;

void free_frame(char **frame,int size){
    printf("size %d\n",size);
    if(frame!=NULL){
        for(int i = 0;i <= size; i ++){
            if(frame[i] != NULL){
                free(frame[i]);
            }
        }
        free(frame);
    }
}

void print_frame(char ** frame, int w,int h){
    if(frame != NULL){
        for(int i = 0;i < w*h; i ++){
            if(i % w==0)
                printf("\n");
            printf("%s",frame[i]);
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

char** getImage(char *path, int size, int* width, int* height){
    char *mychars = " .:-=+*#%@";
    int comp = 0;
    int w,h;
    unsigned char *data = stbi_load(path, &w,&h, &comp, 0);

    *width = w;
    *height = h;


    char **frame = malloc(sizeof(char*)*(w*h)+1);

    if (data) {
        int index = 0;
        int count = 0;
        for (size_t i = 0; i < (w*h)*comp; i+=comp*size) {

            char* pixel = malloc(sizeof(char)*26);
            sprintf(pixel, "\033[38;2;%d;%d;%dm%c"RESET, data[i], data[i + 1], data[i + 2],
                    mychars[(data[i] * (strlen(mychars) - 1) / 255)]);

            frame[index] = pixel;
            index++;
            //printf("%s",pixel);
            /* free(pixel); */

            count ++;
            if(count == w){
                count = 0;
             // puts("");
            }
        }
    }

    return frame;
}

int main() {

    char *path = "sergen";
    char **prev_frame = NULL;
    char **currframe = NULL;

    int width, height;

    for(int i = 0; i < 50; i +=1){
        printf("i %d\n",i);
        char str[20];
        if(i > 999)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 99)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 9)
            sprintf(str,"./%s/0%d.png",path,i);
        else
            sprintf(str,"./%s/00%d.png",path,i);


        currframe = getImage(str, 1,&width,&height);

        if(currframe != NULL){
            if(prev_frame != NULL){
                free_frame(prev_frame,(width)*(height));
            }
            prev_frame = currframe;
        }
        /* print_frame(currframe, width,height); */

        msleep(30);

        /* system("clear"); */
    }
    /* free_frame(currframe,(width)*(height)); */
    //free_frame(prev_frame,(width)*(height));
    return 0;
}
