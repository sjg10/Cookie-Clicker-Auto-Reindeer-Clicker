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
 * @file Entry point and module for discovering the Cookie Clicker window.
 */


/* ---------------------------------------------------------------- */
/* Include Files                                                    */
/* ---------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "scan.h"

/* ---------------------------------------------------------------- */
/* Local Defines                                                    */
/* ---------------------------------------------------------------- */

/* Number of charachters to read in for popen calls */
#define BUFSIZE 100

/* ---------------------------------------------------------------- */
/* Function Definitions                                             */
/* ---------------------------------------------------------------- */

/** 
 * Uses xtools to get windowID.
 *
 * @return The windowID of Cookie Clicker. If parsing fails returns 0.
 */
int getWindowID(void) {
    int retval = 0;
    const char * cmd = "xwininfo -root -all | "
                       "grep \"\\- Cookie Clicker \\-\" | "
                       "awk '{print \"ibase=16;\", toupper(substr($1,3))}' | "
                       "bc";
    FILE *ptr;
    char buf[BUFSIZE];

    if (NULL == (ptr = popen(cmd, "r"))) {
        printf ("Unexpected error running xwininfo: %s\n",strerror(errno));
    }
    else {
        if(fgets(buf, sizeof(buf), ptr) == NULL) {
            printf("Error finding cookie clicker window.\n"
                   "Did xwininfo fail(x11-utils not installed)\n"
                   "Or is cookie clicker actually open in your browser, and the selected tab?\n");
        }
        else {
            if(pclose(ptr) != 0) {
                printf("Error parsing xwininfo\n"
                        "Is there more than one window with \"- Cookie Clicker -\" in the title?\n");
            }
            else {
                printf("getWindowID received %s\n", buf);
                retval = atoi(buf);
            }
        }
    }
    return retval;
}

int main(int argc, char ** argv) {
    int windowID = getWindowID();
    if (windowID == 0) {
        printf("Error reading windowID\n");
        return (-1);
    }
    else {
        initiateScan(windowID);
        return 0;
    }
}
