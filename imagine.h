#ifndef IMAGINE_H_
#define IMAGINE_H_

typedef struct frame{
    char **frame;
    int strings;
    unsigned char* pixel_data;
    int width, height, comp;

} Frame;


void print_frame(Frame* frame);

Frame *newFrame(char *path);

void free_frame(Frame* frame);

void draw_frame(Frame* prev_frame, Frame *new_frame);

void print_frame(Frame* frame);

int getBrightness(int red, int green, int blue);

int load_frame(Frame *frame, int size);


#endif // IMAGINE_H_
