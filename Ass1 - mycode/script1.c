/*
* Modifications by Renjie Meng , September 2017
* Name: Renjie Meng; Login Name: renjiem
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
 
/*DEBUG*/
#define DEBUG 0
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif
 
/*Global Variables*/
#define MAXCHARS 200            /*max number of chars per word*/
#define MAXQUERYNUMBER 20       /*max number of queries*/
#define CH_NEWLINE '\n'         /*newline character*/
#define CH_NULLBYTE '\0'        /*null byte*/
typedef char word_t[MAXCHARS];  /*an array of chars of 
                                        size MAXCHARS*/
typedef struct {
    int line_num;
    double scores_num;
    char line_chars[256];
}info_s_t;                     /*infomation about scores and chars 
                                relative to specific line number*/
 
/****************************************************************/
  
/* function prototypes */
int     mygetchar();
int     is_prefix_match(char [], char []);
double  score_per_line(int [], int, int);
int     stage1(int argc, char* argv[], word_t query_words[]);
void    do_print_stage4(info_s_t A[], int index);
void    do_sort_stage4(info_s_t A[], int index);
void    do_swap_stage4(info_s_t *A, info_s_t *B);
void    do_compare_on_stage4(info_s_t A[], int index);
void    do_reset(int *line, double *score, int *bytes_count,
                 int *words_count,int *end,int *len,
                 int *char_index);
void    do_when_not_newline(int c, int *bytes_count,
                            int *char_index,
                            int stage4_index
                            ,info_s_t stage4_store[]);
void    do_when_end_of_words(int c, char last_one_char, 
                            int *end, 
                            word_t oneword, int len, 
                            int query_index,
                            int *words_count,
                            word_t query_words[],
                            int each_quert_matched_per_line[]);
void    do_when_start_of_words(int *end,int c,int *len);
void    do_add_char_into_words(int c, word_t oneword, int *len);
void    do_print_stage2_3(int line, int bytes_count, 
                            int words_count, double score,
                            int *newline);
void    put_new_line(int *newline, int c);
/****************************************************************/
  
/* main program controls all the action
*/
int
main(int argc, char *argv[]) {
    int query_index,len=0,c,bytes_count=0, words_count=0, 
        line=1,end=0,stage4_index=0,char_index=0,
        each_quert_matched_per_line[MAXQUERYNUMBER] = {0},
        newline=0;
    double score;
    word_t query_words[MAXQUERYNUMBER], oneword ;
    char last_one_char;
    info_s_t stage4_store[6];

    /*stage1*/
    query_index=stage1(argc, argv, query_words);

    /*stage2&3&4*/
    while ((c = mygetchar()) != EOF ) {
        /*put new line char at end of each printing out put*/
        put_new_line(&newline, c);
        /*non-newline char, count bytes and store
         its char then print out the char*/
        do_when_not_newline(c,&bytes_count,&char_index,
                            stage4_index,stage4_store);     
        /*determine if it is the end of the word*/
        do_when_end_of_words(c, last_one_char, &end, oneword, len,
            query_index, &words_count, query_words,
                        each_quert_matched_per_line);
        /*determine if it is the start of the word*/
        do_when_start_of_words(&end,c,&len);
        /*blank line, no output, only reset the parameters*/
        if (c == CH_NEWLINE && !bytes_count) {
            do_reset(&line,&score,&bytes_count,&words_count,
                                    &end,&len,&char_index);
        /*non-blank line, generate an output*/
        }else if (c == CH_NEWLINE && bytes_count) {
            score = score_per_line(each_quert_matched_per_line, 
                                    query_index, words_count);
            stage4_store[stage4_index].line_chars[char_index]
                                =CH_NULLBYTE;
            if (!score) {
                /*non-score,do nothing*/
            }else if (score && stage4_index==5){
                /*when there are more than 5 inputs,
                    check the 6th infomation*/
                stage4_store[stage4_index].line_num=line;
                stage4_store[stage4_index].scores_num=score;
                do_compare_on_stage4(stage4_store,stage4_index);
                do_sort_stage4(stage4_store,stage4_index);
            }else if (score && 0<=stage4_index
                             && stage4_index<=4) {
                /*when there are less or equal than 5 info, 
                    just add in and sort*/
                stage4_store[stage4_index].line_num=line;
                stage4_store[stage4_index].scores_num=score;
                stage4_index++;
                do_sort_stage4(stage4_store,stage4_index);
            }
            /*print*/
            do_print_stage2_3(line,bytes_count,words_count,score,
                                &newline);
            DUMP_DBL(score);
            do_reset(&line,&score,&bytes_count,
                &words_count,&end,&len,&char_index);    
        }
        last_one_char=c;
        /*add letter and digit into word*/
        do_add_char_into_words(c, oneword, &len);
    }
    printf("---------------------------------------------\n");
    do_print_stage4(stage4_store,stage4_index);
    return 0;
}

/****************************************************************/

/*the offcial getchar()
*/
int
mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}

/****************************************************************/

/*find out if the query is prefix of the word
*/
int
is_prefix_match(char query[], char word[]) {
    int i;
    for (i=0; query[i]!=CH_NULLBYTE;i++) {
        /*if there are unmatched words, return not prefix match*/
        if (query[i] != tolower(word[i])) {
            return 0;
        }
    }
    /*finally, yes! matched!*/
    return 1;
}

/****************************************************************/

/*score eahc line
*/

double
score_per_line(int num_match_per_query[], int k, 
                        int words_count) {
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

/****************************************************************/
/*stage 1
*check input query and store each query for matching test later
*/
int
stage1(int argc, char* argv[], word_t query_words[]) {
    int i,j,k=0,valid=1, invalid_query_index;
    if (argc<=1) {
        /*no input query, exit.*/
        printf("S1: No query specified, :");
        printf("must provide at least one word\n");
        exit(EXIT_FAILURE);
    }
    printf("S1: query = ");
    for (i=1;i < argc; i++) {
        printf("%s ",argv[i]);
        /*store each query*/
        strcpy(query_words[k], argv[i]);
        k++;
    }
    for (i=1;i < argc; i++) {
        for (j=0; j < strlen(argv[i]); j++) {
            /*now, check if all query is lowercase*/
            if (isupper(argv[i][j]) == 1 || 
                (!isalpha(argv[i][j]) && !isdigit(argv[i][j]))){
                /*if query is invalid, 
                    record its index and stop the loop*/
                valid=0;
                invalid_query_index = i;
                break;
            }
        }
    }
    /*output of stage 1*/
    printf("\n");
    for (i=1;i < argc; i++) {
        /*if there is an empty query, exit*/
        if (!strlen(argv[i])){
            printf("S1: there is an empty query");
            printf(" :invalid character(s) in query\n");
            exit(EXIT_FAILURE);
        }
    }
    if (valid==0) {
        /*invalid query, stop here*/
        printf("S1: %s: invalid character(s) in query\n",
             argv[invalid_query_index]);
        exit(EXIT_FAILURE);
    } else {
        /*valid query, keep going*/
        printf("---\n");
    }
    return k;
}

/****************************************************************/

/*print out the stage 4
*/
void
do_print_stage4(info_s_t A[], int index) {
    int i;
    for (i=0;i<index;i++) {
        printf("S4: line = %d, score = %.3lf\n",
            A[i].line_num,A[i].scores_num);
        printf("%s\n",A[i].line_chars);
        printf("---\n");
    }
}

/****************************************************************/
/*sort the infomation in A according to its score 
*/
void 
do_sort_stage4(info_s_t A[], int index) {
    int i,j;
    for (i=1;i<index;i++){
        for(j=i-1;j>=0;j--){
            if (A[j].scores_num < A[j+1].scores_num){
                do_swap_stage4(&A[j],&A[j+1]);
            }else if ((A[j].scores_num == A[j+1].scores_num)
            &&(A[j].line_num > A[j+1].line_num)){
                do_swap_stage4(&A[j],&A[j+1]);
            }
        }
    }
}

/****************************************************************/

/*swap two infomation.
*/
void do_swap_stage4(info_s_t *A, info_s_t *B){
    info_s_t C;
    C=*A;
    *A=*B;
    *B=C;
}

/****************************************************************/

/*determine if the sixth input greater than the smallest
in the infomation array
*/
void
do_compare_on_stage4(info_s_t A[], int index){
    if (A[5].scores_num>A[4].scores_num){
        do_swap_stage4(&A[5],&A[4]);
    }else if ((A[5].scores_num==A[4].scores_num)
                &&(A[5].line_num<A[4].line_num)){
        do_swap_stage4(&A[5],&A[4]);
    }
}

/****************************************************************/

/*reset all value to 0 except line
*/
void    
do_reset(int *line,double *score, int *bytes_count,
    int *words_count,int *end,int *len, int *char_index){
        int i;
        i=*line+1;
        *line=i;
        *score = 0.000;
        *bytes_count=0;
        *words_count=0;
        *end=0;
        *len=0;
        *char_index=0;
    }

/****************************************************************/

/*pout a newline character when it's a newline
*/
void
put_new_line(int *newline, int c){
    if (*newline && (c!=EOF)) {
        printf("\n");
        *newline=0;
    }
}

/****************************************************************/

/*when the char is not newline,
*print out the char
*store it and count byte
*/
void 
do_when_not_newline(int c, int *bytes_count,int *char_index,
                        int stage4_index,info_s_t stage4_store[]){
    int i,k;
    if( c != CH_NEWLINE) {
        stage4_store[stage4_index].line_chars[*char_index]=c;
        printf("%c",c);
        i=*char_index+1;
        k=*bytes_count+1;
        *char_index=i;
        *bytes_count=k;
    }
}

/****************************************************************/

/*when it's end of a word
*count word and check query
*put a null byte at the end of the word
*/

void
do_when_end_of_words(int c, char last_one_char, int *end, 
                    word_t oneword, int len, int query_index,
                    int *words_count, word_t query_words[],
                    int each_quert_matched_per_line[]){
    int n,i;
    if ((!isalpha(c) && !isdigit(c)) && 
        (isalpha(last_one_char) || isdigit(last_one_char)) ){
        *end=1;
        /*now it's start of a new word,put a null byte there.*/
        oneword[len] = CH_NULLBYTE;
        for (n=0;n<query_index;n++) {
            each_quert_matched_per_line[n] += 
            is_prefix_match(query_words[n], oneword);
        }
        i=*words_count+1;
        *words_count=i;
    }
}

/****************************************************************/

/*when it's start of a word
*set variable len to 0 in order to store a new word
*set end to false
*/

void
do_when_start_of_words(int *end,int c,int *len){
    if (*end && (isalpha(c) || isdigit(c) || c== CH_NEWLINE)) {
        *end=0;
        *len=0;
    }
}

/****************************************************************/

/*add char into a word
*/

void
do_add_char_into_words(int c, word_t oneword, int *len){
    int i;
    if (isalpha(c) || isdigit(c)) {
        oneword[*len] = c;
        i=*len+1;
        *len=i;
    } 
}

/****************************************************************/

/*print out stage2&3
*/

void    
do_print_stage2_3(int line, int bytes_count, int words_count,
                    double score, int *newline){
    printf("\nS2: line = %d, bytes = %d, words = %d\n", 
            line, bytes_count, words_count);
    printf("S3: line = %d, score = %.3lf \n", line, score );
    printf("---");
    *newline=1;
}
/*Algorithms are fun,sir!*/