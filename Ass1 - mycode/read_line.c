#include <stdio.h>
#include <stdlib.h>

void foo(char** line){
    char *foo ="foo";
    line = (char*)malloc(10*sizeof(char*));

    for (int i = 0; i < 3; i++){
        line[i] = foo[i];
    }

    line[3] = '\0';
}

int main(int argc, char *argv[]){
    char **line;
    line = (char**)malloc(10*sizeof(char**));
    // *line = (char*)malloc(10*sizeof(char*));
    foo(*line);
    printf("%s\n", *line);
}
