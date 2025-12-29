// Alfred Roos 2023

#ifndef IMAGINE_H_
#define IMAGINE_H_

typedef struct frame {
  unsigned char *pixel_data;
  int width, height, comp;
} Frame;

typedef struct settings {
  int character_mode;
  int color;
  int width;
  int height;
  int hide_cursor;
  double fps;
  double max_width;
  double max_height;
  char *path;
  int playing;
  int mute;
  int debug;
  int viewer;
} Settings;

Frame *new_frame_data(unsigned char *pixels, int width, int height, int comp);

Frame *new_frame(char *path);

void free_frame(Frame *frame);

void draw_frame(Frame *prev_frame, Frame *new_frame, int character, int color);

void print_frame(Frame *frame, int characters, int color);

int getBrightness(int red, int green, int blue);

int load_frame(Frame *frame, int option, int color);

void scale_frame(Frame *frame, int width, int height);

int set_fps(Settings *settings);

int set_res(Settings *settings);
int render_media(Settings *settings);

void print_image(Settings *settings);
void print_folder(Settings *settings);
double get_scale_factor(int w, int h, double wmax, double hmax);

#endif // IMAGINE_H_
