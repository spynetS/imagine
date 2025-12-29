// Alfred Roos 2023
#include "imagine.h"
#include "../lib/printer/src/printer.h"
#include <dirent.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RENDER 1 // at 0 we will not render (for debug reasons)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// This is the chars used sorted from low to high brightness
char *ascii0 = " .isk@";
char *ascii1 =
	" `.-':_,^=;><+!rc*/"
	"z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

// We have to use a string array because unicode is larger then a char
char *unicode1[] = {"░", "▒", "▓", "█"};
int unicode = 4;

Frame *new_frame_data(unsigned char *pixels, int width, int height, int comp) {

  Frame *new_frame = malloc(sizeof(Frame));
  new_frame->width = width;
  new_frame->height = height;
  new_frame->comp = comp;

  new_frame->pixel_data = pixels;

  return new_frame;
}

void free_frame(Frame *frame) {
  free(frame->pixel_data);
  free(frame);
}

int brightness(int red, int green, int blue) {
  // 0.299 r + 0.587 g + 0.114 b
  // return red;
  return (0.299 * red + green * 0.587 + blue * 0.114);
}

int set_pixel(char *str, int i, Frame *frame, int characters, int color) {
  int contrast = 0;
  char color_str[28] = "";
	int written = 0;
  switch (color) {
  case 1:
		snprintf(color_str,sizeof(color_str), "\033[38;2;%d;%d;%dm", frame->pixel_data[i],
						 frame->pixel_data[i + 1], frame->pixel_data[i + 2]);
    break;
  case 2:
    snprintf(color_str,sizeof(color_str), "\033[48;2;%d;%d;%dm",
						 MIN(frame->pixel_data[i] + contrast, 255),
						 MIN(frame->pixel_data[i + 1] + contrast, 255),
						 MIN(frame->pixel_data[i + 2] + contrast, 255));
    break;
  }
	
  int index = 0;
  size_t str_size = sizeof(char) * 50 * frame->width * frame->height;
  switch (characters) {
  case 0:
		written = snprintf(str,str_size, "%s%c ", color_str,
											 ascii0[(brightness(frame->pixel_data[i], frame->pixel_data[i + 1],
																					frame->pixel_data[i + 2]) *
															 (strlen(ascii0) - 1) / 255)]);
    break;
  case 1:
		written = snprintf(str,str_size, "%s%c ", color_str,
											 ascii1[(brightness(frame->pixel_data[i], frame->pixel_data[i + 1],
																					frame->pixel_data[i + 2]) *
															 (strlen(ascii1) - 1) / 255)]);
    break;
  case 2:
    index = (int)(brightness(frame->pixel_data[i], frame->pixel_data[i + 1],
                             frame->pixel_data[i + 2]) *
                  (unicode - 1) / 255);
		written = snprintf(str,str_size, "%s%s%s", color_str, unicode1[index], unicode1[index]);
    break;
  case 3:
		written = snprintf(str,str_size, "%s██", color_str);
    break;
  }
	return written;
}

void print_frame_as_string(Frame *curr_frame, int characters,
                           int color) {
	if (characters == 3)
		color = 1;

	size_t output_size = 50 * curr_frame->width * curr_frame->height;
	char *output = malloc(output_size);
	if (!output) {
		perror("malloc");
		return;
	}

	size_t offset = 0;
	int x = 0;

	for (int i = 0; i < curr_frame->width * curr_frame->height * curr_frame->comp; i += curr_frame->comp) {
		// set_pixel should write into output + offset, returning how many chars it wrote
		int written = set_pixel(output + offset, i, curr_frame, characters, color);
		offset += written;

		x += 2; // assuming 2 chars per pixel
		if (x == curr_frame->width * 2) {
			if (offset < output_size - 1) {
				output[offset++] = '\n'; // add newline safely
			}
			x = 0;
		}

		if (offset >= output_size - 1) {
			break; // prevent overflow
		}
	}

	output[offset] = '\0'; // null-terminate
	printf("%s", output);
	free(output);
}

void draw_frame(Frame *prev_frame, Frame *curr_frame, int characters,
                int color) {

  int offsetX = 0;
  int offsetY = 0;

  int x = 0;
  int y = 0;
  if (characters == 3)
    color = 1;
  for (int i = 0; i < curr_frame->width * curr_frame->height * curr_frame->comp;
       i += curr_frame->comp) {
    if (prev_frame == NULL ||
        (curr_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
         curr_frame->pixel_data[i + 1] != prev_frame->pixel_data[i + 1] ||
         curr_frame->pixel_data[i + 2] != prev_frame->pixel_data[i + 2])) {

      char str[50];		 
      set_pixel(str, i, curr_frame, characters, color);
      setCharAt(x + offsetX, y + offsetY, str);
    }
    x += 2; // we add 2 becuase we print 2 chars
    if (x == curr_frame->width * 2) {
      x = 0;
      y++;
    }
  }
}

int changes(Frame *prev_frame, Frame *curr_frame) {
  int changes = 0;
  for (int i = 0; i < curr_frame->width * curr_frame->height * curr_frame->comp;
       i += curr_frame->comp) {
    if (prev_frame == NULL ||
        (curr_frame->pixel_data[i] != prev_frame->pixel_data[i] ||
         curr_frame->pixel_data[i + 1] != prev_frame->pixel_data[i + 1] ||
         curr_frame->pixel_data[i + 2] != prev_frame->pixel_data[i + 2])) {
      changes++;
    }
  }
  return changes;
}

void print_frame(Frame *frame, int characters, int color) {

  draw_frame(NULL, frame, characters, color);
}

// returns the scaling factor based on the w and h when the max is wmax, hmax 
double get_scale_factor(int w, int h, double wmax, double hmax) {

  if (wmax == -1 || hmax == -1)
    return 1;

  double scaler = wmax / ((w == 0) ? 1 : w) / 2;
  if (h * scaler > hmax) {
    scaler = hmax / ((h == 0) ? 1 : h);
  }
  return scaler;
}

void *play_sound(void *vargp) {
  char str[200];

  const char *uid = "my_unique_identifier";
  setenv("UID", uid, 1);

  Settings *settings = (Settings *)vargp;
  if(settings->mute == 0){
    snprintf(str,sizeof(str), "ffplay -nodisp -autoexit '%s' > /dev/null 2>&1 &",
						 settings->path);
  }

	int ret = system(str);
	if (ret == -1) {
    perror("system");
	}

  return NULL;
}

int render_media(Settings *settings) {
#if RENDER	
  if(system("clear") == -1){
		perror("system");
	}
#endif
  char str[200];
  int comp = 3;

  double scaler = get_scale_factor(settings->width, settings->height,
                                   settings->max_width, settings->max_height);

  int H = settings->height * scaler;
  int W = settings->width * scaler;

  snprintf(str,sizeof(str),
					 "ffmpeg -i \"%s\" -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -vf "
					 "\"scale=%d:%d\" - -hide_banner -loglevel 8",
					 settings->path, (int)(W), (int)(H));

  FILE *pipein = popen(str, "r");
  pthread_t thread_id;
	// valgrind reports memory leak but it shouldnt be
  pthread_create(&thread_id, NULL, play_sound, settings); 

  if (!pipein) {
    perror("popen");
    return 1;
  }

  Frame *curr_frame = NULL;
  Frame *prev_frame = NULL;

  if (settings->hide_cursor)
    puts(HIDE_CURSOR);

  int frame = 0;
  // Process video frames
  while (1) {
    // basic media contoll
    char input = getKeyPressed();
    if (input == 'q') {
      settings->playing = 1;
      char tmp[200];
      snprintf(tmp,sizeof(tmp), "killall ffplay");
      if(system(tmp)==-1){
				perror("system");
			}
      pthread_exit(&thread_id);
      break;
    }
    if (input == ' ') {
      /*
      ** This killall seems very unsafe and
      ** should really be rewritten
      */
      if (settings->playing) {
        char tmp[200];
        if(!settings->mute){
          snprintf(tmp,sizeof(tmp), "killall -STOP ffplay");
					if(system(tmp)==-1){
						perror("system");
					}
        }
        setCursorPosition(W/2,H/2);
        printf(WHITE"PAUSED");
      }
      else {
        char tmp[200];
        if(!settings->mute){
          snprintf(tmp,sizeof(tmp), "killall -CONT ffplay");
					if(system(tmp)==-1){
						perror("system");
					}
        }
        setCursorPosition(0, 1);
        print_frame_as_string(curr_frame, settings->character_mode,
															settings->color);

      }
      settings->playing = !settings->playing;
    }
    if (input == 'q') {
      char tmp[200];
      if(!settings->mute){
        snprintf(tmp,sizeof(tmp), "killall ffplay");
				if(system(tmp)==-1){
					perror("system");
				}
      }

      break;
    }

    if (settings->playing) {
      unsigned char *data = malloc(sizeof(unsigned char) * H * W * comp);
      // Read a frame from the input pipe into the buffer
      int count = fread(data, 1, H * W * comp, pipein);

      // If we didn't get a frame of video, we're probably at the end
      if (count != H * W * comp) {
        free(data); // we free the new data collected
        break;
      }

      if (curr_frame != NULL) {
        if (prev_frame != NULL) {
          free_frame(prev_frame);
        }
        prev_frame = curr_frame;
      }

      curr_frame = new_frame_data(data, W, H, comp);

      clock_t t;
      t = clock();

      int chan = changes(prev_frame, curr_frame);
      // the changes are more then half the screen print the whole frame
#if RENDER
      if (chan / curr_frame->width > curr_frame->height * 2 / 3) {

        print_frame_as_string(curr_frame, settings->character_mode,
                              settings->color);
      } else {
        draw_frame(prev_frame, curr_frame, settings->character_mode,
                   settings->color);
      }
#endif				
      t = clock() - t;
      double time_taken = ((double)t) / CLOCKS_PER_SEC * 1000; // in ms
      double delay_for_fps = (1 / settings->fps) * 1000;

      setCursorPosition(0, H + 1);
      if(settings->debug){
        printf(WHITE "Time: %d; FPS: %lf; Delay: %lf %lf; changes %d limit %d; "
							 "COMP %d; Q to quit, SPACE to pause",
							 frame, settings->fps, delay_for_fps - time_taken, time_taken,
							 chan / curr_frame->width, curr_frame->height * 2 / 3,
							 curr_frame->comp);
      }
      setCursorPosition(0, 0);

      frame++;
      msleep(delay_for_fps - time_taken);
    }
  }


  if (curr_frame != NULL)
    free_frame(curr_frame);
  if (prev_frame != NULL)
    free_frame(prev_frame);
	
  // set the cursor at the end
  setCursorPosition(0, H + 1);

  // Flush and close input pipe
  fflush(pipein);
  pclose(pipein);


  puts(SHOW_CURSOR);
  return 0;
}

int set_fps(Settings *settings) {
	// when run it will return a string, example 25/1
  char retrive_str[350];
  snprintf(
					 retrive_str,
					 sizeof(retrive_str),
					 "ffprobe -v 8 -select_streams v -of default=noprint_wrappers=1:nokey=1 "
					 "-show_entries stream=avg_frame_rate '%s'",
					 settings->path);

  FILE *res = popen(retrive_str, "r");

  if (!res) {
    perror("popen");
    exit(1);
  }

  char *fps_str = malloc(sizeof(char) * 35);
  if(fread(fps_str, sizeof(char), 35, res) == 0){
		return 0;
	}
  if (strcmp(fps_str, "") == 0)
    exit(1);

	// we add the chars to a buffer untill we find a / then cancel
  char buf[10];
  int index = 0;
  while (fps_str[index] != '/') {
    buf[index] = fps_str[index];
    index++;
  }
  fps_str += index + 1;
  settings->fps = (double)atoi(buf) / atoi(fps_str);
  free(fps_str - 1 - index);
  pclose(res);
	return 1;
}

int set_res(Settings *settings) {

  char retrive_str[256];
	// when run it will return a string of [width]x[height]
  snprintf(retrive_str,
           sizeof(retrive_str),
					 "ffprobe -v 8 -select_streams v -show_entries stream=width,height "
					 "-of csv=p=0:s=x \"%s\"",
					 settings->path);

  FILE *res = popen(retrive_str, "r");

  if (!res) {
    perror("popen");
    exit(1);
  }
  char res_str[100];
  if(fread(res_str, sizeof(char), 10, res) == 0){
		return 0;
	}

  if (strcmp(res_str, "") == 0)
    exit(1);

	// we parse the string and save the width and height based on where the x is
  int w_len = 0;
  char num_holder[5];
  for (size_t i = 0; i < strlen(res_str); i++) {
    if (res_str[i] == 'x') {
      num_holder[i] = '\0';
      (settings->width) = atoi(num_holder);
      w_len = i + 1;
      continue;
    }
    num_holder[i - w_len] = res_str[i];
  }
  (settings->height) = atoi(num_holder);

  fflush(res);
  pclose(res);
  return 1;
}
