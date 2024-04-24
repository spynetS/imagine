// Alfred Roos 2023
#include <stdio.h>
#include "../lib/printer.h"
#include "imagine.h"
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))


char *ascii0   = " .isk@";
char *ascii1   = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

// We have to use a string array because unicode is larger then a char
char *unicode1[] = {"░", "▒", "▓", "█"};
int unicode = 4;

Frame *new_frame_data(unsigned char *pixels, int width, int height,int comp){

    Frame *new_frame  = malloc(sizeof(Frame));
    new_frame->width  = width;
    new_frame->height = height;
    new_frame->comp   = comp;

    new_frame->pixel_data = pixels;

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

void set_pixel(char *str, int i, Frame* frame, int characters, int color){
    int contrast = 0;
    char color_str[28] = "";
    switch(color){
        case 1:
            sprintf(color_str,"\033[38;2;%d;%d;%dm",frame->pixel_data[i],frame->pixel_data[i+1],frame->pixel_data[i+2]);
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
}

void print_frame_as_string(Frame *prev_frame, Frame *curr_frame, int characters, int color){
    int offset = 0;
    char *output = malloc(sizeof(char)*50*curr_frame->width*curr_frame->height);
    if(characters == 3) color = 1;

    int x = 0;
    int y = 0;
    for(int i = 0; i < curr_frame->width*curr_frame->height*curr_frame->comp; i +=curr_frame->comp){
        set_pixel(output, i, curr_frame,characters,color);
        int o = strlen(output);
        output += o;
        offset += o;
        x += 2; // we add 2 becuase we print 2 chars
        if(x == curr_frame->width*2){
            // add a new line when a row is done
            sprintf(output,"\n");
            output++;
            offset++;
            x = 0;
            y++;
        }
    }
    printf("%s",output-offset);
    free(output-offset);
}

void draw_frame(Frame *prev_frame, Frame *curr_frame, int characters, int color){

    int offsetX = 0;
    int offsetY = 0;

    int x = 0;
    int y = 0;
    if(characters == 3) color = 1;
    for(int i = 0; i < curr_frame->width*curr_frame->height*curr_frame->comp; i +=curr_frame->comp){
        if(prev_frame == NULL || (curr_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
            curr_frame->pixel_data[i+1] != prev_frame->pixel_data[i+1] ||
            curr_frame->pixel_data[i+2] != prev_frame->pixel_data[i+2])){

            char str[50];
            set_pixel(str, i, curr_frame,characters,color);

            setCharAt(x+offsetX,y+offsetY,str);
        }
        x += 2; // we add 2 becuase we print 2 chars
        if(x == curr_frame->width*2){
            x = 0;
            y++;
        }
    }
}


int changes(Frame *prev_frame, Frame *curr_frame, int characters, int color){
    int changes = 0;
    for(int i = 0; i < curr_frame->width*curr_frame->height*curr_frame->comp; i +=curr_frame->comp){
        if(prev_frame == NULL || (curr_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
            curr_frame->pixel_data[i+1] != prev_frame->pixel_data[i+1] ||
            curr_frame->pixel_data[i+2] != prev_frame->pixel_data[i+2])){
            changes++;
        }
    }
    return changes;
}


void print_frame(Frame* frame, int characters, int color){

    draw_frame(NULL, frame,characters ,color );
}

double get_scale_factor(int w, int h,double wmax,double hmax){

    if(wmax == -1 || hmax == -1) return 1;

    double scaler = wmax / ((w == 0) ? 1: w)/2;
    if(h*scaler > hmax){
        scaler = hmax / ((h == 0) ? 1: h);
    }
    return scaler;
}

int render_media (Settings *settings){
    system("clear");
    char str[200];
    int comp = 3;

    double scaler = get_scale_factor(settings->width, settings->height, settings->max_width,settings->max_height);

    int H = settings->height * scaler;
    int W = settings->width * scaler;

    sprintf(str,"ffmpeg -i \"%s\" -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -vf \"scale=%d:%d\" - -hide_banner -loglevel 8",settings->path, (int)(W), (int)(H));
    FILE *pipein = popen(str, "r");

    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg

    if (!pipein) {
        perror("popen");
        return 1;
    }

    Frame *curr_frame = NULL;
    Frame *prev_frame = NULL;

    if(settings->hide_cursor) puts(HIDE_CURSOR);

    int play = 1;
    int frame = 0;
    // Process video frames
    while (1)
    {
        //basic media contoll
        char input = getKeyPressed();
        if(input == ' '){
           play = !play;
        }
        if(input == 'q'){
            break;
        }

        if(play){
            unsigned char *data = malloc(sizeof(unsigned char) * H*W*comp);
            // Read a frame from the input pipe into the buffer
            int count = fread(data, 1, H*W*comp, pipein);

            // If we didn't get a frame of video, we're probably at the end
            if (count != H*W*comp){
                free(data);
                break;
            }

            if(curr_frame != NULL){
                if(prev_frame != NULL){
                    free_frame(prev_frame);
                }
                prev_frame = curr_frame;
            }

            curr_frame = new_frame_data(data,W,H,comp);

            clock_t t;
            t = clock();

            int chan = changes(prev_frame, curr_frame, settings->character_mode, settings->color);
            // the changes are more then half the screen print the whole frame
            if(chan/curr_frame->width > curr_frame->height*2/3){
                print_frame_as_string(prev_frame,curr_frame,settings->character_mode,settings->color);
            }
            else{
                draw_frame(prev_frame,curr_frame,settings->character_mode,settings->color);
            }

            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC * 1000; // in ms
            double delay_for_fps = (1/settings->fps)*1000;

            setCursorPosition(0, H+1);
            printf(WHITE"Time: %d; FPS: %lf; Delay: %lf %lf; changes %d limit %d; COMP %d; Q to quit, SPACE to pause",
               frame,
               settings->fps,
               delay_for_fps-time_taken,
               time_taken,
               chan/curr_frame->width,
               curr_frame->height*2/3,
               curr_frame->comp);

            setCursorPosition(0, 0);

            frame++;
            msleep(delay_for_fps-time_taken);
        }


    }
    if(curr_frame != NULL)
        free_frame(curr_frame);
    if(prev_frame != NULL)
        free_frame(prev_frame);

    // set the cursor at the end
    setCursorPosition(0, H+1);

    // Flush and close input pipe
    fflush(pipein);
    pclose(pipein);

    puts(SHOW_CURSOR);
    return 0;
}

void set_fps(Settings* settings){

    char retrive_str[350];
    sprintf(retrive_str,"ffprobe -v 8 -select_streams v -of default=noprint_wrappers=1:nokey=1 -show_entries stream=avg_frame_rate \"%s\"",settings->path);

    FILE* res = popen(retrive_str,"r");

    if (!res) {
        perror("popen");
        exit(1);
    }

    char *res_str = malloc(sizeof(char) * 35);
    fread(res_str, sizeof(char), 35, res);
    if(strcmp(res_str,"") == 0)
        exit(1);

    char buf[10];
    int index = 0;
    while(res_str[index] != '/'){
        buf[index] = res_str[index];
        index++;
    }
    res_str += index+1;
    settings->fps = (double) atoi(buf)/atoi(res_str);
    free(res_str-1-index);
    pclose(res);
}

int set_res(Settings *settings){
    char retrive_str[100];
    sprintf(retrive_str,"ffprobe -v 8 -select_streams v -show_entries stream=width,height -of csv=p=0:s=x \"%s\"",settings->path);

    FILE* res = popen(retrive_str,"r");

    if (!res) {
        perror("popen");
        exit(1);
    }
    char res_str[100];
    fread(res_str, sizeof(char), 10, res);

    if(strcmp(res_str,"") == 0)
        exit(1);

    int w_len = 0;
    char num_holder[5];
    for(int i = 0; i < strlen(res_str); i++){
        if(res_str[i] == 'x'){
            num_holder[i] = '\0';
            (settings->width) = atoi(num_holder);
            w_len = i+1;
            continue;
        }
        num_holder[i-w_len] = res_str[i];
    }
    (settings->height) = atoi(num_holder);

    fflush(res);
    pclose(res);
    return 1;
}
