#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scan.h"

/** Parse input parameters and return the first argument as an integer
 * to use as the windowID
 *
 * @param[in] argc Argument Count
 * @param[in] argv Array of null terminated argument strings
 *
 * @return The first argument as a. If parsing fails returns 0.
 */
int parseInput(int argc, char ** argv) {
    /* TODO: use a popen call to 
     * xwininfo -root -all | grep "Cookie Clicker" | awk '{print "ibase=16;", toupper(substr($1,3))}' | bc
     * or use xlib if you can figure it
     * */
    int retval = 0;
    if(argc == 2) {
        retval = atoi(argv[1]);
    }
    return retval;
}
/** Print a usage line.
 *
 * @param[in]  progName a null terminated string of the program name.
 */
void printHelp(char * progName) {
    printf("Usage is %s windowID, where windowID is the window id\n",
            progName);
}

int main(int argc, char ** argv) {
    int windowID = parseInput(argc,argv);
    if (windowID == 0) {
        printHelp(argv[0]);
        return (-1);
    }
    else {
        initiateScan(windowID);
    }
    return 0;
}
