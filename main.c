#include <stdio.h>
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

int main(void) {
    printf("%d\n",(getBrightness(0x02,0,0)*9)/255);

    unsigned char red = 0xFF;
    //char *mychars = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@@";
    //char *mychars = " .,-isk#";
    char *mychars = " .:-=+*#%@";
    int width, height, comp;
    unsigned char *data = stbi_load("john.jpg", &width, &height, &comp, 0);
    width = width;
    height = height;
    int count = 0;
    if (data) {
        printf("width = %d, height = %d, comp = %d (channels)\n", width, height, comp);
        char pixel[comp];
        int pixelc = 0;
        for (size_t i = 0; i < (width*height)*comp; i++) {

            printf("%c",mychars[(data[i]*(strlen(mychars)-1)/255)]);

            if(i%comp==0)
                count++;
            if(count == width){
                printf("\n");
                count = 0;
            }
        }
        printf("\n");
    }
    return 0;
}
