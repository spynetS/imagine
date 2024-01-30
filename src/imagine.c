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


char *ascii0   = " .isk@";
char *ascii1   = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

// We have to use a string array because unicode is larger then a char
char *unicode1[] = {"░", "▒", "▓", "█"};
int unicode = 4;

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
    //return red;
    return (0.299*red+green*0.587+blue*0.114);
}

void draw_frame(Frame *prev_frame, Frame *curr_frame, int characters, int color){

    if(prev_frame == NULL){
        print_frame(curr_frame,characters,color);
        return;
    }

    for(int i = 0; i < curr_frame->width*curr_frame->height*curr_frame->comp; i +=curr_frame->comp){
            if(curr_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
                curr_frame->pixel_data[i+1] != prev_frame->pixel_data[i+1] ||
                curr_frame->pixel_data[i+2] != prev_frame->pixel_data[i+2]){

                print_frame(curr_frame, characters, color);
            }
    }
}

void print_frame(Frame* frame, int characters, int color){

    if(characters == 3) color = 1;

    int contrast = 0;

    int offsetX = 0;
    int offsetY = 0;

    int x = 0;
    int y = 0;

    for(int i = 0; i < frame->width*frame->height*frame->comp; i +=frame->comp){

        char str[50];
        char color_str[28] = "";
        switch(color){
            case 1:
                sprintf(color_str,"\033[38;2;%d;%d;%dm",MIN(frame->pixel_data[i]+contrast,255),MIN(frame->pixel_data[i+1]+contrast,255),MIN(frame->pixel_data[i+2]+contrast,255));
                break;
            case 2:
                sprintf(color_str,"\033[48;2;%d;%d;%dm",MIN(frame->pixel_data[i]+contrast,255),MIN(frame->pixel_data[i+1]+contrast,255),MIN(frame->pixel_data[i+2]+contrast,255));
                break;

        }
        int index = 0;
        switch(characters){
            case 0:
                sprintf(str,"%s%c ",color_str,ascii0[(brightness(frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2])*(strlen(ascii0)-1)/255)]);
                break;
            case 1:
                sprintf(str,"%s%c ",color_str,ascii1[(brightness(frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2])*(strlen(ascii1)-1)/255)]);
                break;
            case 2:
                index = (int)(brightness(frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2])*(unicode-1)/255);
                sprintf(str,"%s%s%s",color_str,unicode1[index],unicode1[index]);
                break;
            case 3:
                sprintf(str,"%s██", color_str);
                break;
        }
        setCharAt(x+offsetX,y+offsetY,str);

        x += 2; // we add 2 becuase we print 2 chars
        if(x == frame->width*2){
            x = 0;
            y++;
        }
    }
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

void print_folder(Settings *settings){
    //int file_count = 0;
    int file_count = get_file_count(settings->path);

    Frame *prev_frame = NULL;
    Frame *curr_frame = NULL;

    system("clear");
    puts(HIDE_CURSOR);
    for (int i = 1; i < file_count; i++) {

        char file_path[100]; // change to dynamic
        sprintf(file_path, "%s/%d.png",settings->path,i);

        if(curr_frame != NULL){
            if(prev_frame != NULL){
                free_frame(prev_frame);
            }
            prev_frame = curr_frame;
        }

        curr_frame = new_frame(file_path);

        double scaler = get_scale_factor(curr_frame->width, curr_frame->height, settings->max_width,settings->max_height);
        scale_frame(curr_frame,curr_frame->width*scaler, curr_frame->height*scaler);
        /* printf("%lf\n",scaler); */
        if(prev_frame != NULL){
            /* puts(file_path); */

            draw_frame(prev_frame,curr_frame,settings->character_mode,settings->color);
        }
        else{
            print_frame(curr_frame,settings->character_mode,settings->color);
        }
        msleep(0);

    }
    free_frame(curr_frame);
    free_frame(prev_frame);
    puts(SHOW_CURSOR);
}

void scale_frame(Frame *frame, int width, int height){

    int nw = width;
    int nh = height;

    unsigned char *resizedPixels = malloc(nh * nw * frame->comp);
    stbir_resize_uint8_linear(frame->pixel_data, frame->width, frame->height, 0, resizedPixels, nw, nh, 0, frame->comp);
    free(frame->pixel_data);

    frame->pixel_data = resizedPixels;
    frame->width = nw;
    frame->height = nh;

}

double get_scale_factor(int w, int h,double wmax,double hmax){

    if(wmax == -1 || hmax == -1) return 1;

    double scaler = wmax / ((w == 0) ? 1: w)/2;
    if(h*scaler > hmax){
        scaler = hmax / ((h == 0) ? 1: h);
    }
    return scaler;
}


void print_image (Settings *settings){

    puts(settings->path);

    Frame *frame = new_frame(settings->path);
    printf("Channels %d\n", frame->comp);
    double scaler = get_scale_factor(frame->width, frame->height, settings->max_width,settings->max_height);

    scale_frame(frame,frame->width*scaler, frame->height*scaler);

    draw_frame(NULL,frame, settings->character_mode, settings->color);
    free_frame(frame);
}
