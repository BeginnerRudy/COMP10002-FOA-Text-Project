#include <stdio.h>
#include <stdlib.h>

void foo(char** word){
    char *foo ="foo";
    *word = (char*)malloc(10*sizeof(char*));
    for (int i = 0; i < 3; i++){
        (*word)[i] = foo[i];
    }

    (*word)[3] = '\0';
}

int main(int argc, char *argv[]){
    char * word;
    foo(&word);
    printf("%s\n", word);
}
