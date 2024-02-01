#include <printer.h>
#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"
#include <string.h>

/* #define W 854 */
/* #define H 480 */
/* unsigned char data[1000][1000][3] = {0}; */

int play_video (Settings *settings)
{

    int H = settings->height;
    int W = settings->width;

    char str[200];
    int comp = 3;

    sprintf(str,"ffmpeg -i %s -f image2pipe -vcodec rawvideo -pix_fmt rgb24 - -hide_banner -loglevel error",settings->path);
    FILE *pipein = popen(str, "r");

    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg

    if (!pipein) {
        perror("popen");
        return 1;
    }

    // Process video frames
    while (1)
    {

        unsigned char *data = malloc(sizeof(unsigned char) * H*W*comp);
        // Read a frame from the input pipe into the buffer
        int count = fread(data, 1, H*W*comp, pipein);
        // If we didn't get a frame of video, we're probably at the end
        if (count != H*W*comp) break;


        Frame *my_frame = new_frame_data(data, W, H, comp);

        double scaler = get_scale_factor(my_frame->width, my_frame->height, termWidth(), termHeight());
        scale_frame(my_frame, my_frame->width * scaler, my_frame->height * scaler);

        print_frame(my_frame, settings->character_mode, settings->color);

        free_frame(my_frame);
        msleep(33);
    }

    // Flush and close input pipe
    fflush(pipein);
    pclose(pipein);

    return 0;
}

int set_res(Settings *settings){
    char retrive_str[100];
    sprintf(retrive_str,"ffprobe -v error -select_streams v -show_entries stream=width,height -of csv=p=0:s=x %s",settings->path);

    FILE* res = popen(retrive_str,"r");
    char res_str[100];
    fread(res_str, sizeof(char), 10, res);

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

    printf("Width is %d and height is %d\n", settings->width,settings->height);

    fflush(res);
    pclose(res);
    return 1;
}

int main(int argv, char **argc)
{
    Settings settings;
    settings.character_mode = 3;
    settings.color = 1;
    settings.path = argc[1];

    set_res(&settings);

    int result = play_video(&settings);
    return result;
}
