#include <printer.h>
#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"

/* #define W 854 */
/* #define H 480 */
#define W 256
#define H 144
/* unsigned char data[1000][1000][3] = {0}; */

int play_video (Settings *settings)
{

    /* int H = settings->height; */
    /* int W = settings->width; */

    char str[200];
    int comp = 3;

    sprintf(str,"ffmpeg -i %s -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -",settings->path);
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

        print_frame(my_frame, comp, 1);

        free_frame(my_frame);
        msleep(33);
    }

    // Flush and close input pipe
    fflush(pipein);
    pclose(pipein);

    return 0;
}

int main(int argv, char **argc)
{
    Settings settings;
    settings.path = argc[1];

    int res = play_video(&settings);
    return res;
}
