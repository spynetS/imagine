#include "viewer.h"
#include "imagine.h"
#include <stdio.h>
#include "../lib/printer/src/printer.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define DA_INIT_CAP 256

#define da_append(da, item)						\
  do {									\
    if ((da)->count >= (da)->capacity) {				\
      (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2; \
      (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
      if ((da)->items == NULL) {                                        \
	fprintf(stderr, "ERROR: could not allocate memory\n");		\
	exit(1);							\
      }                                                                 \
    }									\
    (da)->items[(da)->count++] = (item);				\
  } while (0)

#define da_free(da) free((da).items)



typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} String_Array;



void init_media(Settings* settings) {
  if(settings->width == 0 || settings->height == 0){
    set_res(settings);
  }
  
  if(set_fps(settings) == 0){
    puts("no input file provided, -h for help");
    exit(1);
  }
  
}

int is_image_file(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return 0;
    
    return (strcmp(ext, ".jpg") == 0 || 
            strcmp(ext, ".jpeg") == 0 ||
            strcmp(ext, ".png") == 0 ||
            strcmp(ext, ".gif") == 0 ||
            strcmp(ext, ".bmp") == 0);
}

void render(Settings* settings, const char* path,  String_Array *paths, int index)
{
  sprintf(settings->path,"%s/%s",path, paths->items[index]);
  init_media(settings);
  render_media(settings);
}

int start_viewing(Settings* settings) {

  disableEcho();
  String_Array paths = {0};
  char* path = "/home/spy/Pictures";
  
  DIR *dir = opendir(path);
  if (!dir) {
    perror("opendir");
    return 1;
  }

  
  
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    // Skip . and ..
    if (strcmp(entry->d_name, ".") == 0 || 
	strcmp(entry->d_name, "..") == 0)
      continue;
            
    if (is_image_file(entry->d_name)) {
      da_append(&paths, entry->d_name);
      printf("Found image: %s\n", entry->d_name);
    }
  }
  closedir(dir);

  printf("navigate with 'A' and 'D' to go forward and backwards\n");
  
  int index = 0;
  while(1) {
    if(kbhit()){
      switch(getchar()){
      case 'q':
	return 0;
      case 'd':
	index = (index + 1) % paths.count;
	break;
      case 'a':
	index = (index - 1) % paths.count;
	break;
	
      }
      render(settings,path,&paths,index);
    }
    
  }


  return 0;
}
