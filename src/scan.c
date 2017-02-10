/*
 *   Copyright 2017 Sam Gonshaw
 *
 *   This file is part of Cookie Clicker Auto Reindeer Clicker.
 *   Cookie Clicker Auto Reindeer Clicker
 *
 *   Cookie Clicker Auto Reindeer Clicker is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Cookie Clicker Auto Reindeer Clicker is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Cookie Clicker Auto Reindeer Clicker.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Module for beginning scans of a window.
 */

/* ---------------------------------------------------------------- */
/* Include Files                                                    */
/* ---------------------------------------------------------------- */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wand/MagickWand.h>
#include <time.h>
#include <unistd.h>   //_getch
#include <termios.h>  //_getch
#include <sys/select.h>
#include <termios.h>

/* ---------------------------------------------------------------- */
/* Local Defines                                                    */
/* ---------------------------------------------------------------- */

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

/* ---------------------------------------------------------------- */
/* Function Definitions                                             */
/* ---------------------------------------------------------------- */

/**
 * Toggles the terminal mode to press-by-press instead of line-by-line for
 * listening for keys. Calling again returns to usual settings.
 * Repeated calls are ignored.
 * Registers itself to be called atexit incase of unexpected termination.
 */ 
void toggleConioTerminalMode()
{
    static int mode = 0;
    static struct termios orig_termios;
    struct termios new_termios;
    switch(mode++) {
        case 0:
            /* take two copies - one for now, one for later */
            tcgetattr(0, &orig_termios);
            memcpy(&new_termios, &orig_termios, sizeof(new_termios));

            /* register cleanup handler, and set the new terminal mode */
            atexit(toggleConioTerminalMode);
            cfmakeraw(&new_termios);
            tcsetattr(0, TCSANOW, &new_termios);
            break;
        case 1:
            /* Restore the old settings */
            tcsetattr(0, TCSANOW, &orig_termios);
            printf("\n");
            break;
        default:
            /* Do nothing else */
            break;
    }
}

/**
 * Listens for a keypress, returns if one has occured since the last kbhit call.
 * @pre toggleConioTerminalMode to enable keypress-by-keypress read.
 * @return the key pressed or 0 if none
 */
int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

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
 * React to a given non-trivial red scale diff.
 *
 * @param[in] wand the diff
 */
void processNonTrivialDiff(MagickWand * wand) {
#if 0
    unsigned long x, y, number_wands;
    PixelIterator * iterator;
    iterator = NewPixelIterator(wand);
    PixelWand ** pixels = PixelGetNextIteratorRow(iterator,&number_wands);
    /* Iterate through rows */
    for (x = 0; pixels != (PixelWand **) NULL; x++)
    {
        /* And columns in that row */
        for(y = 0; y < number_wands; y++) {
            /* TODO: check for red and react if red 
                    reactToDiff(x,y);
                    break;
            */
        }
        /* Make sure to increment the rows */
        pixels = PixelGetNextIteratorRow(iterator,&number_wands);
    }
    /* And reset on completion */
    iterator=DestroyPixelIterator(iterator);
#else
    MagickWriteImage(wand, "my_diff.png");
#endif

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
    printf("Beginning scan... (press any key to exit at any time)\n");
    /* Get first image */
    MagickReadImage(wands[0], searchString);
    MagickReadImage(wands[1], searchString);
    MagickWand * diff_wand;
    diff_wand = NewMagickWand();
    /* Set next wand t store image */
    int i = 1;
    /* Prepare for scanning for keypress */
    toggleConioTerminalMode();
    while(!kbhit()) {
        clock_t start = clock(), diff;
        /* Get image */
        MagickReadImage(wands[i], searchString);
        double distortion;
        //Composite to use: SrcCompositeOp
        diff_wand = MagickCompareImages(wands[0], wands[1],
                AbsoluteErrorMetric, &distortion);
        if(distortion != 0) {
            /* TODO: correct composite setting */
            MagickCompositeImage(diff_wand, wands[0], SrcCompositeOp, 0, 0);
            processNonTrivialDiff(diff_wand);
            break;
        }
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
    printf("Freeing scan resources and exiting...\n");
    diff_wand=DestroyMagickWand(diff_wand);
    toggleConioTerminalMode();
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
