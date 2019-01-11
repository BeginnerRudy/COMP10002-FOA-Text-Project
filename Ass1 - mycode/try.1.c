#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


#define DEBUG 0
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif

#define MAXCHARS 200
#define MAXQUERYNUMBER 20
#define MAXCHARSPERLINE 100
#define MAXLINES 5000

typedef char word_t[MAXCHARS];

int mygetchar();
int is_prefix_match(char [], char []);
double score_per_line(int [], int, int);

void stage1(int argc, char* argv[], word_t *, int);
int get_a_word(int c, char*, char);

int
main(int argc, char *argv[]) {
    /*i used to locate qurey in argv*/
    /*j used to locate char in qurey*/
    /*k used to store each query*/
    /*c used to store mygetchar()*/
    /*n is used to iterool the query_words*/
    int k=0,n,c,
    bytes_count=0, words_count=0, line=1;
    double score;
    word_t query_words[MAXQUERYNUMBER], oneword ;
    char last_one_char;

    /*create an int array initialized with all 0, lately used to store number of words each query match per line*/
    int each_quert_matched_per_line[MAXQUERYNUMBER] = {0};

    /*stage1*/
    stage1(argc, argv, query_words,k);
    
    /*stage2&3*/
    while ((c = mygetchar()) != EOF ) {
        if( c != '\n') {
            bytes_count++;
            printf("%c",c);
        }
        if (get_a_word(c, oneword,last_one_char)) {
            print("%s ",oneword);
            for (n=0;n<k;n++) {
                each_quert_matched_per_line[n] += is_prefix_match(query_words[n], oneword);
            }
            words_count++;
        }
        if (c == '\n' && !words_count) {
            line++;
            score = 0.000;
            bytes_count=0;
            words_count=0;
        }else if (c == '\n' && words_count) {
            score = score_per_line(each_quert_matched_per_line, k, words_count);            
            //printf("\nS2: line = %d, bytes = %d, words = %d\n", line, bytes_count, words_count);
            //printf("S3: line = %d, score = %.3lf \n", line, score );
            DUMP_DBL(score);
            line++;
            score = 0.00;
            bytes_count=0;
            words_count=0;
        }
        last_one_char=c;
    }


/*now this block is useless, cause on the dimefox, the last line of text has '\n'*/

    /*    
    oneword[len]='\0';
    for (n=0;n<k;n++) {
        each_quert_matched_per_line[n] += is_prefix_match(query_words[n], oneword);
    }
    score = score_per_line(each_quert_matched_per_line, k, words_count+1);
    printf("\nS2: line = %d, bytes = %d, words = %d \n", line, bytes_count, words_count+1);
    printf("S3: line = %d, score = %.3lf\n", line, score );
*/
    printf("------------------------------------------------");
    return 0;
}



/*stage 1*/
void
stage1(int argc, char* argv[], word_t query_words[], int k) {
    int i,j,valid=1, invalid_query_index;
    if (argc<=1) {
        /*no input query, exit.*/
        printf("S1: No query specified, must provide at least one word\n");
        exit(EXIT_FAILURE);
    }
    printf("S1: query = ");
    for (i=1;i < argc; i++) {
        printf("%s ",argv[i]);
        strcpy(query_words[k], argv[i]);
        k++;
    }
    for (i=1;i < argc; i++) {
        for (j=0; j < strlen(argv[i]); j++) {
            /*now, check if all query is lowercase*/
            if (isupper(argv[i][j]) == 1 || (!isalpha(argv[i][j]) && !isdigit(argv[i][j]))){
                /*if query is invalid, record its index and stop the loop*/
                valid=0;
                invalid_query_index = i;
                break;
            }
        }
    }
    /*output of stage 1*/
    printf("\n");
    for (i=1;i < argc; i++) {
        if (!strlen(argv[i])){
            printf("S1: there is an empty query : invalid character(s) in query\n");
            exit(EXIT_FAILURE);
        }
    }
    if (valid==0) {
        /*invalid query, stop here*/
        printf("S1: %s: invalid character(s) in query\n", argv[invalid_query_index]);
        exit(EXIT_FAILURE);
    } else {
        /*valid query, keep going*/
        printf("---\n");
    }
}



int
get_a_word(int c, char oneword[], char last_one_char) {
    int maybe_end=0,len=0;
    if ((!isalpha(c) && !isdigit(c)) && (isalpha(last_one_char) || isdigit(last_one_char)) ){
        maybe_end=1;
    }
    if (maybe_end && (isalpha(c) || isdigit(c) || c== '\n')) {
        /*now it's end of a word.*/
        oneword[len] = '\0';
        maybe_end=0;
        len=0;
        return 1;
    }
    if (isalpha(c) || isdigit(c)) {
        oneword[len++] = c;
    }
    return 0;
}






int
mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}

int
is_prefix_match(char query[], char word[]) {
    int i;
    for (i=0; query[i]!='\0';i++) {
        /*if there are unmatched words, return not prefix match*/
        if (query[i] != tolower(word[i])) {
            return 0;
        }
    }
    /*finally, yes! matched!*/
    return 1;
}


double
score_per_line(int num_match_per_query[], int k, int words_count) {
    /*i used to iteration.*/
    /*Sum is the numerator part*/
    int i;
    double Sum=0, denominator, result;
    for (i=0;i<k;i++) {
        Sum += log(num_match_per_query[i]+1.0)/log(2);
        num_match_per_query[i]=0;   
    }
    denominator = log(8.5 + words_count)/log(2.0);
    result = Sum/denominator;
    return result;
}


/*2017/09/09*/
/*now i'm facing a question, how to store each sigle words in an array. i can't konw when this word ends
because if i used ' ' to test it. i will fail at all blank line.*/
/*All I want to do is that store each word and end up with '\0' then use count and stage3 funcs run on it*/

/*2017/09/11*/
/*haha, now i solve it. 1:36 A.M */

/*2017/09/11*/
/*another issue in stage2: like
works.
how to count this?*/

/*2017/09/11*/
/*now i have a problem, when i store the word i will miss the first letter of the word*/

/*2017/09/11*/
/*now i face a problem, the last word of the text is not be counted into the is_prefix function*/

/*2017/09/11*/
/*I've already submitted my assignment successfully, now i will continue Week8.
 And I won't do the assignment until this weekend*/ 