#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCHARS 20
#define MAXROWS 30
#define MAXCHARSPERLINE 100
typedef char word_t [MAXCHARS+1];



#define DEBUG 1
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif
int
mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}


int
main(int argc, char *argv[]) {
    int c,line=1, bytes_count=0, words_count=0, lastC=0,
    i, j, k=0, valid=1, invalid_query_index;
    word_t a_word, word_per_line[MAXCHARSPERLINE];

    if (argc<=1) {
        printf("S1: No query specified, must provide at least one word\n");
        exit(EXIT_FAILURE);
    }
    printf("S1: query = ");
    for (i=1;i<argc;i++) {
        printf("%s ",argv[i]);
        for (j=0;j<strlen(argv[i]);j++) {
            if (isupper(argv[i][j]) == 1) {
                valid=0;
                invalid_query_index = i;
                break;
            }
        }
    }
    printf("\n");
    if (valid==0) {
        printf("S1: %s: invalid character(s) in query", argv[invalid_query_index]);
    } else {
        printf("---\n");
    }

    while ((c=mygetchar()) != EOF) {
        if (c != '\n') {
            bytes_count++;
            printf("%c", c);
            if ( !isalpha(c) ) {
                /*do nothing*/
            }
            if (isalpha(c) || c == '\'' || c == '-' || c == '.') {
                a_word[k++] = c;
                if ( c == ' ')
            }     
        } 
        if ( c == '\n') {
            //if (words_count != 0){
            printf("\nS2: line = %d, bytes = %d, words = %d\n", line, bytes_count, words_count+1);
            //}
            bytes_count=0;
            words_count=0;
            lastC=0;
            line++;
        }
    }
    printf("\nS2: line = %d, bytes = %d, words = %d\n", line, bytes_count, words_count);   
    return 0;
}