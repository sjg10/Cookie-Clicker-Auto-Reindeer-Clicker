#include <stdio.h>
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
    /* TODO */
    return 0;
}
/** Print a usage line.
 *
 * @param[in]  progName a null terminated string of the program name.
 */
void printHelp(char * progName) {
    printf("TODO\n");
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
