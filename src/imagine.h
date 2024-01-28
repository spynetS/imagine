#ifndef IMAGINE_H_
#define IMAGINE_H_

typedef struct frame{
    unsigned char* pixel_data;
    int width, height, comp;

} Frame;


void print_frame(Frame* frame);

Frame *newFrame(char *path);

void free_frame(Frame* frame);

void draw_frame(Frame* prev_frame, Frame *new_frame);

void print_frame(Frame* frame);

int getBrightness(int red, int green, int blue);

/**
  * frame - Loads frame buffer with the strings
  * option - choses the char to print to strings
  *      0 ascii with low detail
  *      1 ascii with high detail
  *      2 unicode block
  * color - choses color mode
  *      0 no color
  *      1 forground color
  *      2 background color
  * */
int load_frame(Frame *frame, int option, int color);


#endif // IMAGINE_H_
