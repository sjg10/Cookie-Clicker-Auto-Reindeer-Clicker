#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wand/MagickWand.h>
#include <time.h>
#include <unistd.h>

#define MAX_STR_LEN 100
/** Period in usec to repeat search */
#define TIME_DELAY ((1000 * 1000) / 2)

/* TODO: improve! */
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

/**
 * Clicks on a location specified by a diff found at (x,y).
 *
 * @param[in] x vertical location of diff within search space.
 * @param[in] y horizantal location of diff within search space.
 *
 */ 
void reactToDiff(long unsigned int x, long unsigned int y) {
    /* TODO */
    printf("MISMATCH: (%lu,%lu)\n",x,y);
}

/**
 * Use the supplied 2 wands to take a sequence of images using the search string and compare them
 * every TIME_DELAY usecs. Click on the changed parts of the image.
 *
 * @param[in] wands an array of two initialised wand pointers
 * @param[in] searchString a search string to use in ReadImage
 * @return function does not return
 */ 
void scanLoop(MagickWand ** wands, char * searchString) {
    printf("Beginning scan... (CTRL-C to exit at any time)\n");
    /* Get first image */
    MagickReadImage(wands[0], searchString);
    MagickReadImage(wands[1], searchString);
    PixelIterator * iterators[2];
    unsigned long number_wands[2];
    unsigned long x,y;
    PixelWand ** pixels[2];
    iterators[0] = NewPixelIterator(wands[0]);
    iterators[1] = NewPixelIterator(wands[1]);
    /* Set next wand t store image */
    int i = 1;
    while(1) {
        clock_t start = clock(), diff;
        /* Get image */
        MagickReadImage(wands[i], searchString);
        pixels[0] = PixelGetNextIteratorRow(iterators[0],&number_wands[0]);
        pixels[1] = PixelGetNextIteratorRow(iterators[1],&number_wands[1]);
        /* Iterate through rows */
        for (x = 0; pixels[0] != (PixelWand **) NULL; x++)
        {
            /* And columns in that row */
            for(y = 0; y < number_wands[0]; y++) {
                /* And compare */
                if (PixelGetIndex(pixels[0][y]) != PixelGetIndex(pixels[1][y])) {
                    /* TODO: comparision doesn't seem to work yet */
                        reactToDiff(x,y);
                        break;
                }
            }
            /* Make sure to increment the rows */
            pixels[0] = PixelGetNextIteratorRow(iterators[0],&number_wands[0]);
            pixels[1] = PixelGetNextIteratorRow(iterators[1],&number_wands[1]);
        }
        /* And reset on completion */
        PixelResetIterator(iterators[0]);
        PixelResetIterator(iterators[1]);
        /* Swap to use next wand next loop */
        i = 1 - i;
        /* And fill in the loop the correct time */
        diff = clock() - start;
        int usec = (diff * 1000 * 1000) / CLOCKS_PER_SEC;
        if(TIME_DELAY > usec) {
            usleep(TIME_DELAY - usec);
        }
        else {
            printf("TIME_DELAY %uus is too short for current mainloop\n", TIME_DELAY);
        }
    }
    /* Free resources */
    iterators[1]=DestroyPixelIterator(iterators[1]);
    iterators[0]=DestroyPixelIterator(iterators[0]);
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
        /* Loop */
        MagickWand * wands[2];
        wands[0] = NewMagickWand();
        wands[1] = NewMagickWand();
        scanLoop(wands, searchString);
        /* Destroy resources */
        wands[1]=DestroyMagickWand(wands[1]);
        wands[0]=DestroyMagickWand(wands[0]);
    }
    MagickWandTerminus();
}
