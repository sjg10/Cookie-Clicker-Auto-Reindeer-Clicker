#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "scan.h"

/* Number of charachters to read in for popen calls */
#define BUFSIZE 100

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
