#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DEBUG 1
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif

void stage1(int argc,char* argv[]);
void stage2(int argc,char* argv[]);
int words(const char *sentence);

int
main(int argc, char *argv[]) {
    stage1(argc, argv);
    stage2(argc, argv);
    return 0;
}


void
stage1(int argc, char *argv[]) {
    int i, j, valid=1, invalid_query_index;
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
 }

 void
 stage2(int argc,char* argv[]) {
    int i=1;
	char const* const fileName = argv[1]; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s", line); 
        printf("S2: line = %d, words = %i\n",i,words(line));
        printf("%d",sizeof(line));
        i++;
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);
 }

 /* count amount of words in one line*/
int 
words(const char *sentence)
{
    int count=0,i,len;
    char lastC;
    len=strlen(sentence);
    if(len > 0)
    {
        lastC = sentence[0];
    }
    for(i=0; i<=len; i++)
    {
        if((sentence[i]==' ' || sentence[i]=='\0') && lastC != ' ')
        {
            count++;
        }
        lastC = sentence[i];
    }
    return count;
}
