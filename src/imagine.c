//Alfred Roos 2023
#include <stdio.h>
#include "printer.h"
#include "imagine.h"
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

/*
**         #TODO
**  - scale image
**  - different options
**  - change frame on gpu
**  - play video without self extracting frames
**  - music
*/

Frame *new_frame(char *path){
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

int brightness(int red, int green, int blue){
    //0.299 r + 0.587 g + 0.114 b
    return (0.299*red+green*0.587+blue*0.114);
}

void draw_frame(Frame *prev_frame, Frame *new_frame, int characters, int color){

    if(prev_frame == NULL){
        print_frame(new_frame,2,1);
        return;
    }

    int offsetX = 0;
    int offsetY = 0;
    char *mychars = " .isk@";
    char *mychars_detailed = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

    int x = 0;
    int y = 0;

    if(characters == 2) color = 1;

    for(int i = 0; i < new_frame->width*new_frame->height*new_frame->comp; i +=new_frame->comp){
            if(new_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
                new_frame->pixel_data[i+1] != prev_frame->pixel_data[i+1] ||
                new_frame->pixel_data[i+2] != prev_frame->pixel_data[i+2]){

                int contrast = 0;

                char str[50];
                char color_str[28] = "";
                switch(color){
                    case 1:
                        sprintf(color_str,"\033[38;2;%d;%d;%dm",MIN(new_frame->pixel_data[i]+contrast,255),MIN(new_frame->pixel_data[i+1]+contrast,255),MIN(new_frame->pixel_data[i+2]+contrast,255));
                        break;
                    case 2:
                        sprintf(color_str,"\033[48;2;%d;%d;%dm",MIN(new_frame->pixel_data[i]+contrast,255),MIN(new_frame->pixel_data[i+1]+contrast,255),MIN(new_frame->pixel_data[i+2]+contrast,255));
                        break;

                }
                switch(characters){
                    case 0:
                        sprintf(str,"%s%c ",color_str,mychars[(brightness(new_frame->pixel_data[i],new_frame->pixel_data[i+1],new_frame->pixel_data[i+2])*(strlen(mychars)-1)/255)]);
                        break;
                    case 1:
                        sprintf(str,"%s%c ",color_str,mychars_detailed[(brightness(new_frame->pixel_data[i],new_frame->pixel_data[i+1],new_frame->pixel_data[i+2])*(strlen(mychars_detailed)-1)/255)]);
                        break;
                    case 2:
                        sprintf(str,"%s██", color_str);
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
    char *mychars_detailed = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

    if(characters == 2) color = 1;

    for(int i = 0; i < frame->width*frame->height*frame->comp; i +=frame->comp){
        char color_str[28] = "";
        switch(color){
            case 1:
                sprintf(color_str,"\033[38;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]);
                break;
            case 2:
                sprintf(color_str,"\033[48;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]);
                break;
        }
        switch(characters){
            case 0:
                printf("%s%c ",color_str,mychars[(brightness(frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2])*(strlen(mychars)-1)/255)]);
                break;
            case 1:
                printf("%s%c ",color_str,mychars_detailed[(brightness(frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2])*(strlen(mychars_detailed)-1)/255)]);
                break;
            case 2:
                printf("%s██", color_str);
                break;
        }
        count ++;
        if(count == frame->width){
            count = 0;
            printf("\n");
        }
    }
}

int __compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

char **__get_frame_names(char *path, int *file_count){
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    if (d) {
        // Collect file names in a dynamic array
        char **file_names = NULL;
        (*file_count) = 0;

        while ((dir = readdir(d)) != NULL) {
            const char *ext = strrchr (dir->d_name, '.');
            if (dir->d_type == DT_REG && (!strcmp (ext+1, "png"))){
                printf("%d\n",(*file_count));
                file_names = realloc(file_names, ((*file_count) + 1) * sizeof(char *));
                file_names[*file_count] = strdup(dir->d_name);
                (*file_count)++;
            }
        }

        // Sort the array of file names
        qsort(file_names, (*file_count), sizeof(char *), __compare);

        // Print the sorted file names
        closedir(d);

        return file_names;
    }
    return NULL;
}

int get_file_count(char* path){
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    int file_count = 0;

    if (d) {

        while ((dir = readdir(d)) != NULL) {
            const char *ext = strrchr (dir->d_name, '.');
            if (dir->d_type == DT_REG && (!strcmp (ext+1, "png"))){
                (file_count)++;
            }
        }
        closedir(d);
    }
    return file_count;
}

void print_folder(char *path,double max_width, int characters, int color){
    //int file_count = 0;
    int file_count = get_file_count(path);

    Frame *prev_frame = NULL;
    Frame *curr_frame = NULL;

    system("clear");
    puts(HIDE_CURSOR);
    for (int i = 1; i < file_count; i++) {

        char file_path[100]; // change to dynamic
        sprintf(file_path, "%s/%d.png",path,i);

        if(curr_frame != NULL){
            if(prev_frame != NULL){
                free_frame(prev_frame);
            }
            prev_frame = curr_frame;
        }

        curr_frame = new_frame(file_path);

        double scaler = max_width / curr_frame->width;// == 0? 1: max_width/curr_frame->width;
        scale_frame(curr_frame,curr_frame->width*scaler/2, curr_frame->height*scaler/2);
        /* printf("%lf\n",scaler); */
        if(prev_frame != NULL){
            /* puts(file_path); */

            draw_frame(prev_frame,curr_frame,characters,color);
        }
        else{
            /* print_frame(curr_frame,characters,color); */
        }
        msleep(33);

    }
    free_frame(curr_frame);
    free_frame(prev_frame);
    puts(SHOW_CURSOR);
}

void scale_frame(Frame *frame, int width, int height){

    int nw = width;
    int nh = height;

    unsigned char *resizedPixels = malloc(nh * nw * 3);
    stbir_resize_uint8_linear(frame->pixel_data, frame->width, frame->height, 0, resizedPixels, nw, nh, 0, frame->comp);
    free(frame->pixel_data);

    frame->pixel_data = resizedPixels;
    frame->width = nw;
    frame->height = nh;

}

void print_image(char *path, double max_width,int characters, int color){
    puts(path);

    Frame *frame = new_frame(path);
    printf("Channels %d\n", frame->comp);
    double scaler = max_width / frame->width;// == 0? 1: max_width/curr_frame->width;
    scale_frame(frame,frame->width*scaler/2, frame->height*scaler/2);

    print_frame(frame,  characters, color);
    free_frame(frame);
}
