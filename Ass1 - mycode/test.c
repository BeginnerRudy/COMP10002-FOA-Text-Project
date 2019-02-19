#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void foo1(char *c){
    c = (char*)malloc(4*sizeof(char*));

    char *foo = "foo";

    for (int i = 0; i < 3; i++){
        c[i] = foo[i];
    }

    c[3] = '\0';

    printf("a2\n" );
}

void foo(char**c){
    c = (char**)malloc(sizeof(char**));
    printf("a1\n" );
    foo1(c[0]);
    printf("a3\n" );
}

int main(int argc, char *argv[]){
    char ***c;
    c = (char***)malloc(sizeof(char***));

    foo(c[0]);
    printf("a4\n" );
    printf("%s\n", **c);
    return 0;
}
