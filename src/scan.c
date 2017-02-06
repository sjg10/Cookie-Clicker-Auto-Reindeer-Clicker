#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wand/MagickWand.h>

#define MAX_STR_LEN 100

/* 30% and 100 px width [min] are from CSS */
#define regionLeft(windowWidth) ((int)(0.3 * windowWidth) + 16)
#define regionWidth(windowWidth) (100)
/* Heights are guesses */
#define regionTop(windowHeight) ((int)(windowHeight / 3))
#define regionHeight(windowHeight) ((int)(windowHeight / 3) * 2)

/**
 * Create the appropriate search string to capture the needed search area
 * of the cookie clicker window for use with MagickReadImage
 *
 * @param[in] windowID of the window to capture
 * @param[in/out] wand An initialised MagickWand. Will be overwritten.
 * @param[out] String to write result to.
 * @return true iff successful with no error
 */
bool createCaptureString(int windowID, MagickWand * wand, char * str) {
    int strLen = sprintf(str, "x:%u", windowID);
    if(strLen > MAX_STR_LEN) {
        printf("ERROR: max string len exceeded\n");
        return false;
    }
    else {
        MagickReadImage(wand, str);
        int totalHeight = MagickGetImageHeight(wand);
        int totalWidth = MagickGetImageWidth(wand);
        printf("Measured window size %ux%u\n",totalWidth,totalHeight);
        strLen = sprintf(str, "x:%u[%ux%u+%u+%u]",
                windowID,
                regionWidth(totalWidth),
                regionHeight(totalHeight),
                regionLeft(totalWidth),
                regionTop(totalHeight));
        if(strLen > MAX_STR_LEN) {
            printf("ERROR: max string len exceeded\n");
            return false;
        }
        else {
            printf("Created X capture string \'%s\'\n",str);
            return true;
        }   
    }
}

void initiateScan(int windowID) {
    MagickWandGenesis();
    MagickWand *wand = NULL;
    wand = NewMagickWand();
    char searchString[MAX_STR_LEN + 1];
    if(!createCaptureString(windowID, wand, searchString)) {
        printf("ERROR\n");
    }
    else {
        MagickReadImage(wand, searchString);
        MagickWriteImage(wand,"screen_test.png");
        wand=DestroyMagickWand(wand);
        MagickWandTerminus();
    }
}
