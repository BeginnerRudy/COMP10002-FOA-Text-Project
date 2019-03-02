#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 1
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif

/**Define the Constant used in this program***********************************/

// A word from the input could have up to 200 chars
#define MAX_CHARS_PER_WORD 50
#define INITIAL_NUMBER_OF_LINE_PER_TEXT 5
#define INITIAL_NUMBER_OF_WORD_PER_LINE 5
#define INITIAL_NUMBER_OF_CHAR_PER_WORD 5
#define NEWLINE '\n'
#define KEEP_READING 'k'
#define NULL_BYTE '\0'
// Define alias, TRUE for 1 as well as FALSE for 0.
#define TRUE 1
#define FALSE 0
// Since the queries in argv start from index 1, we make a constant for it.
#define THE_START_INDEX_OF_QUERY 1

/**Define new data type and strucTRUE*****************************************/

// The new type word_t is used to storing a word.
typedef int bool;

typedef struct{
    char* word;
    int char_count;
    int curr_max_char_count;
}word_t;

typedef struct {
    word_t* words;
    int line_index;
    int word_count;
    int byte_count;
    int curr_max_word_hold;
    double score;
} line_t;

typedef struct {
    line_t* lines;
    int line_index;
    int curr_max_line_hold;
} text_t;

/**Define the function prototypes*********************************************/
int mygetchar();
/*This function is responsible for all the activity of stage1, TRUE means
pass the stage1 successfully, FALSE means failed to pass.*/
bool stage1(int argc, char *argv[]);
// This function returns TRUE if word consisted of all lowercase chars,
// otherwise, return FALSE.
bool does_contain_uppercase_for_word(char word[]);
// Add a newline to the print out content
void add_a_new_line_char();
// response for stage2;
text_t stage2();
// read the line of a text into a line_t variable
char read_by_line(line_t* line, int line_index);
// update the text_t variable
void update_text(text_t *text);
// update the line_t variable
void update_line(line_t *line);
// read a word of a text into a word_t variable
char read_by_word(word_t* word, int *byte_count, int *word_count);
void print_line_summary(int line_index, int word_count, int byte_count);
void stage_separation(){
    printf("\n==========================================================================\n\n");
}
void stage3(text_t *text, int query_num, char* queries[]);
double score_a_line(int num_of_word_in_a_line, int queries_match_count[], int query_size);
void calculating_statistics_for_scoring(int queries_match_count[], int query_num, char *queries[], line_t line);
int count_query_match(char *query, line_t line);
bool is_prefix_matched(char *query, char *word);
/***************The main function started*************************************/
int
main(int argc, char *argv[]) { 
    text_t text;

    // run stage 1, if stage1 returns FALSE, terminate this program.
    if (!stage1(argc, argv)){
        exit(EXIT_FAILURE);
    }

    stage_separation();

    // run stage 2
    text = stage2();

    stage_separation();

    // run stage 3
    stage3(&text, argc, argv);

    return 0;
}

void stage3(text_t *text, int query_num, char* queries[]){
    //scoring line by line
    for (int curr_line_index = 0; curr_line_index < text->line_index; curr_line_index++){
        // calculating the statistics for scoring
        int queries_match_count[query_num - 1];
        calculating_statistics_for_scoring(queries_match_count, query_num, queries, text->lines[curr_line_index]);

        //score curr line
        text->lines[curr_line_index].score = score_a_line(text->lines[curr_line_index].word_count,
                                                            queries_match_count, query_num - 1);
    }
}

double score_a_line(int num_of_word_in_a_line, int queries_match_count[], int query_size){
    double score = 0.0;

    for (int i = 0; i < query_size; i++){
        score += log(1.0 + queries_match_count[i]);
    }

    return score/(log(8.5 + num_of_word_in_a_line));
}

void calculating_statistics_for_scoring(int queries_match_count[], int query_num, char *queries[], line_t line){

    // count number of matches query by query
    for (int i = 1; i <query_num; i++){
        // count current query matched prefix in a line
        queries_match_count[i - 1] = count_query_match(queries[i], line);
    }

    return;
}

int count_query_match(char *query, line_t line){
    int count = 0;

    for (int i = 0; i < line.word_count; i++){
        if (is_prefix_matched(query, line.words[i].word) == TRUE){
            count++;
        }
    }

    return count;
}

bool is_prefix_matched(char *query, char *word){
    int query_len = strlen(query);

    if (strlen(word) < query_len){
        return FALSE;
    }

    for (int i = 0; i < query_len; i++){
        if (query[i] != tolower(word[i]))
            return FALSE;
    }

    return TRUE;
}

text_t stage2(){
    text_t text;
    char flag_of_line;
    // initialize text
    text.line_index = 0;
    text.curr_max_line_hold = INITIAL_NUMBER_OF_LINE_PER_TEXT;
    text.lines = (line_t*)malloc(INITIAL_NUMBER_OF_LINE_PER_TEXT * sizeof(line_t));

    // //keep reading words into text line by line, till the end of a file
    while ((flag_of_line = read_by_line(&text.lines[text.line_index], text.line_index)) != EOF){
            print_line_summary( text.lines[text.line_index].line_index ,
                                text.lines[text.line_index].word_count,
                                text.lines[text.line_index].byte_count);
            update_text(&text);
    }

    return text;
}

char read_by_line(line_t* line, int line_index){

    // initialize this line
    line->line_index = line_index + 1;
    line->word_count = 0;
    line->byte_count = 0;
    line->curr_max_word_hold = INITIAL_NUMBER_OF_WORD_PER_LINE;
    line->score = 0.0;
    line->words = (word_t*)malloc(INITIAL_NUMBER_OF_WORD_PER_LINE * sizeof(word_t));

    //local variable
    char flag_of_word;

    // keep reading words into line word by word, till meet a newline char
    while ((flag_of_word = read_by_word(&line->words[line->word_count],
         &line->byte_count, &line->word_count)) == KEEP_READING){
        // update line statistics and line->words memory
        update_line(line);
    }

    return flag_of_word;
}

void update_text(text_t *text){
    // increase the number of line stored in text
    (*text).line_index = (*text).line_index+1;

    //check whether need to expand memory for allocating new line
    if (text->line_index >= text->curr_max_line_hold){
        text->curr_max_line_hold += INITIAL_NUMBER_OF_LINE_PER_TEXT;
        text->lines = (line_t*)realloc(text->lines,
            text->curr_max_line_hold * sizeof(line_t));
    }
}

void update_line(line_t *line){

    // check memory
    if (line->word_count >= line->curr_max_word_hold){
        line->curr_max_word_hold += INITIAL_NUMBER_OF_WORD_PER_LINE;
        line->words = (word_t*)realloc(line->words,
                                line->curr_max_word_hold * sizeof(word_t));
    }
}

char read_by_word(word_t* word, int *byte_count, int *word_count){
    char c;

    //skip non-alpabetic chars
    while ((c = mygetchar()) != EOF && c != NEWLINE && !isalpha(c)) {
        *byte_count = *byte_count + 1;
        printf("%c", c);
    }

    if (c == EOF || c == NEWLINE){
        printf("%c", c);
        return c;
    }

    //there is a word captured
    //initialize a word
    word->char_count = 0;
    word->curr_max_char_count = INITIAL_NUMBER_OF_CHAR_PER_WORD;
    word->word = (char*)malloc(INITIAL_NUMBER_OF_CHAR_PER_WORD * sizeof(char));

    //append the first char
    printf("%c", c);
    word->word[word->char_count++] = c;
    *byte_count = *byte_count + 1;

    while (isalpha(c = mygetchar()) && c != NEWLINE){
        //check is there is enough memory for char
        if (word->char_count >= word->curr_max_char_count){
            word->curr_max_char_count += INITIAL_NUMBER_OF_CHAR_PER_WORD;
            word->word = (char*)realloc(word->word,
                                        word->curr_max_char_count * sizeof(char));
        }

        printf("%c", c);
        word->word[word->char_count++] = c;
        *byte_count = *byte_count + 1;
    }

    word->word[word->char_count++] = NULL_BYTE;
    *word_count = *word_count + 1;
    printf("%c", c);
    *byte_count = *byte_count + 1;

    if (c == NEWLINE){
        return c;
    }


    return KEEP_READING;
}

bool stage1(int argc, char *argv[]){
    // If there is no arg inputed, response the error message to the user
    if (argc-THE_START_INDEX_OF_QUERY == 0){
         // minus THE_START_INDEX_OF_QUERY, since the first arg is this file's filename.
        printf("S1: No query specified, must provide at least one word\n");
        return FALSE;
    }


    // Display all the qurey in the required format.
    printf("S1: query = ");
    for (int i = THE_START_INDEX_OF_QUERY; i < argc; i++){
        printf("%s ", argv[i]);
    }
    add_a_new_line_char();

    // Check if all words of the args are all in lowercase by a for-loop,
    // if not responses an error message and returns FALSE
    for (int i = THE_START_INDEX_OF_QUERY; i < argc; i++){
        if (does_contain_uppercase_for_word(argv[i])){
            printf("S1: %s: invalid character(s) in query\n", argv[i]);
            return FALSE;
        }
    }

    // Pass all the two test, return TRUE, good!
    return TRUE;
}

bool does_contain_uppercase_for_word(char word[]){
    // Check each char in word for whether is it in lowercase, if not
    // returns FALSE.
    for (int i = 0; i < strlen(word); i++){
        if (!islower(word[i])){
            return TRUE;
        }
    }

    // All chars are in lowercase, returns TRUE!
    return FALSE;
}

void add_a_new_line_char(){
    printf("\n");
}

int mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}

void print_line_summary(int line_index, int word_count, int byte_count){
    printf("S2: line = %d, bytes = %d, words = %d\n", line_index,byte_count, word_count);
}
