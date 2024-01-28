#ifndef IMAGINE_H_
#define IMAGINE_H_

typedef struct frame{
    unsigned char* pixel_data;
    int width, height, comp;

} Frame;


Frame *new_frame(char *path);

void free_frame(Frame* frame);

void draw_frame(Frame* prev_frame, Frame *new_frame, int character, int color);

void print_frame(Frame* frame, int characters, int color);

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

void print_image(char *path, int characters, int color);

void print_folder(char *path, int characters, int color);


#endif // IMAGINE_H_
