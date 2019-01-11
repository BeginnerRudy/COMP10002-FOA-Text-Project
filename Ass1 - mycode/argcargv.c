#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int
main(int argc, char *argv[]) {
    int k;
    for (k=0;k<argc;k++) {
        printf("argv[%d]: %s lenth: %d \n",k,argv[k], strlen(argv[k]));
    }
    return 0;
}