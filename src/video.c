#include <printer.h>
#include <stdio.h>
#include <stdlib.h>
#include "imagine.h"
#include "printer.h"

// Video resolution
#define W 854
#define H 480

// Allocate a buffer to store one frame
unsigned char frame[H][W][3] = {0};

int main()
{
    int x, y, count;

    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg
    FILE *pipein = popen("ffmpeg -i comp.mp4 -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");

    if (!pipein) {
        perror("popen");
        return 1;
    }

    // Process video frames
    while (1)
    {
        // Read a frame from the input pipe into the buffer
        count = fread(frame, 1, H*W*3, pipein);

        // If we didn't get a frame of video, we're probably at the end
        if (count != H*W*3) break;

        unsigned char *data = malloc(sizeof(unsigned char) * H*W*3);

        // Process this frame
        int i = 0;
        for (y = 0; y < H; ++y)
            for (x = 0; x < W; ++x)
            {
                data[i] = frame[y][x][0];
                i++;
                data[i] = frame[y][x][1];
                i++;
                data[i] = frame[y][x][2];
                i++;
            }

        Frame *my_frame = new_frame_data(data, W, H, 3);

        if (!my_frame) {
            perror("new_frame_data");
            free(data);
            break;
        }

        double scaler = get_scale_factor(my_frame->width, my_frame->height, termWidth(), termHeight());

        scale_frame(my_frame, my_frame->width * scaler, my_frame->height * scaler);

        print_frame(my_frame, 3, 1);

        free_frame(my_frame);
        //free(data);
    }

    // Flush and close input pipe
    fflush(pipein);
    pclose(pipein);

    return 0;
}
