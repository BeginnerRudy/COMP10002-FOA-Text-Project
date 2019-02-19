#include <stdio.h>
#include <stdlib.h>

void foo_word(char* word){
    char *foo ="foo";
    word = (char*)malloc(10*sizeof(char*));

    for (int i = 0; i < 3; i++){
        word[i] = foo[i];
    }

    word[3] = '\0';
}

void foo_line(char **line){
    *line = (char*)malloc(10*sizeof(char*));
    foo_word(*line);
}

void foo_text(char ***text){
    *text = (char**)malloc(10*sizeof(char**));
    foo_line(*text);
}

int main(int argc, char *argv[]){
    char ***text;
    text = (char***)malloc(10*sizeof(char***));
    foo_text(text);
    printf("%s\n", **text);
}
