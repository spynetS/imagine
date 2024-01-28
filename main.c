#include <stdio.h>
#include "printer.h"
#include <stdlib.h>
#define BG_BLACK  "\x1b[40m"
#define BG_RED    "\x1b[41m"
#define BG_GREEN  "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE   "\x1b[44m"
#define BG_PURPLE "\x1b[45m"
#define BG_CYAN   "\x1b[46m"
#define BG_WHITE  "\x1b[47m"


#define BLACK  "\x1b[30m"
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define PURPLE "\x1b[35m"
#define CYAN   "\x1b[36m"
#define WHITE  "\x1b[37m"
#define BG     "██"

#define RESET "\x1b[0m"
#define BG_RESET "\x1b[10m"

#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const size_t NUM_PIXELS_TO_PRINT = 1000U;

int getBrightness(int red, int green, int blue){
    //0.2126*R + 0.7152*G + 0.0722*B
    if(red < 0) red = 0;
    if(green < 0) green = 0;
    if(blue < 0) blue = 0;
    /* return (int)(0.299 * red + 0.587 * green + 0.114 * blue); */
    return (int)(red +green + blue);
}

int drawImage(char *path, int size){
    char *mychars = " .:-=+*#%@";
    //char *mychars = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
    int width, height, comp;
    unsigned char *data = stbi_load(path, &width, &height, &comp, 0);
    width = width;
    height = height;
    int count = 0;
    if (data) {
        int steps = 2;
        int y = 0;
        int index = 0;
        char frame[width*height*28];
        char frame2[width*height*28];
        for (size_t i = 0; i < (width*height)*comp; i+=comp*size) {

            char pixel[50];
            char color[28];
            //sprintf(color,"\x1b[38;5;%dm",data[i]);
            sprintf(color,"\033[38;2;%d;%d;%dm",data[i],data[i+1],data[i+2]);
            sprintf(pixel,"%s%c"RESET,color,mychars[(data[i]*(strlen(mychars)-1)/255)]);
            for(int j = 0; j < strlen(pixel); j++){
                frame[index] = pixel[j];
                index++;
            }

            count +=1;

            if(count == width/size){
                frame[index] = '\n';
                index++;
                count = 0;
            }
        }
        frame[index+1] = '\0';
        printf("%s\n",frame);
    }
    return 1;
}

int main() {

    char *path = "sergen";

    for(int i = 0; i < 2840; i +=1){
        char str[10];
        if(i > 999)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 99)
            sprintf(str,"./%s/%d.png",path,i);
        else if(i > 9)
            sprintf(str,"./%s/0%d.png",path,i);
        else
            sprintf(str,"./%s/00%d.png",path,i);

        drawImage(str,1);
        //fflush(stdout);
        printf("\nframe %s\n", str);
        msleep(1000);
        system("clear");
    }
    return 0;
}
