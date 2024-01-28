

#include <stdio.h>
#include "imagine.h"

int main(){
    char str[60];
    scanf("%s",str);
    Frame *frame = newFrame(str);
    load_frame(frame,2);
    draw_frame(NULL,frame);
    free_frame(frame);
}
