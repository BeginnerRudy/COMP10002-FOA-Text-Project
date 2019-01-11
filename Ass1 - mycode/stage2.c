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
mygetword(char W[], int limit) {
    int c,len=0;
    
    while ((c = mygetchar()) != EOF && !isalpha(c)) {
        /*do nothing*/
    }
    if (c == EOF) {
        return EOF;
    }
    W[len++] = c;
    while (len<limit && (c = mygetchar()) != EOF && (isalpha(c)
             || c == '\'' || c == '-' || c == '.')) {
        W[len++] = c;
    }
    W[len] = '\0';
    return 0;
}

int
count_words(char W[], int count) {
    int i=0,len;
    len=strlen(W);
    while (i<len) {
        if (W[i] == '.' || W[i] =='\'' || W[i] == '.') {
            if (isalpha(W[i-1]) && isalpha(W[i-1])) {
                count++;
            }
        }
    }
    return 0;
}

int
main(int argc, char *argv[]) {
    word_t a_word, all_words_perline[MAXCHARSPERLINE];
    int c, i=0, word_count=1, index_of_words=0;
    if ((c=mygetchar()) != '\n') {
        while (mygetword(a_word,MAXCHARSPERLINE) != EOF) {
                strcpy(all_words_perline[index_of_words], a_word);

                printf("%s \n", a_word);
     }
            }else if (c == '\n') {
                printf("\n\n");
            }

        /*index_of_words++;
        count_words(a_word, word_count);
        printf("words = %d", word_count);
        word_count=1;*/


    }
