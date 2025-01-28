// Alfred Roos 2023
#include "../lib/printer.h"
#include "flagser.h"
#include "imagine.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Settings settings;

void setOption(int av, char **ac) { settings.character_mode = atoi(ac[1]); }
void setColor(int av, char **ac) { settings.color = atoi(ac[1]); }

void setRes(int av, char **ac) {
  settings.width = atoi(ac[1]);
  settings.height = atoi(ac[2]);
}
void setMute(int av, char **ac) {
  settings.mute = atoi(ac[1]);
}
void setHideCursor(int av, char **ac) { settings.hide_cursor = atoi(ac[1]); }

int main(int argc, char **argv) {

  // system("sox ");

  settings.character_mode = 3;
  settings.color = 1;
  settings.hide_cursor = 1;
  settings.fps = 12;
  settings.playing = 1;
  settings.mute = 0;

  settings.max_width = termWidth();
  settings.max_height = termHeight();

  settings.path = argc[argv - 1];

  addFlag("-t", "--type",
          "Sets the type to be outputed as 0 ascii, 1 assci more detail, 2 unicode block",
          setOption);
  addFlag("-c", "--color",
          "Sets the if we should output with color (escape code) 0 no color, 1 forground color",
          setColor);
  addFlag("-w", "--width", "Set the width ", setRes);

  addFlag("--hide-cursor", "--hide-cursor", "1 to hide 0 to show (default 1)",
          setHideCursor);

  addFlag("-m", "--mute", "1 to mute 0 to not mute (default 0)",
         setMute);

  addHelp();
  parse(argc, argv);

  if(argc == 2 && strcmp(argv[1],"-h")==0){
    exit(1);
  }

  set_res(&settings);
  set_fps(&settings);
  render_media(&settings);

  return 0;
}
