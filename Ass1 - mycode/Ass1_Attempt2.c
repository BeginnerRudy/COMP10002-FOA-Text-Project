#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
string.h provides {
                    1. strlen() for calculating the length of a word.
                    }
*/

/**
ctype.h provides {
                    1. islower() for deciding whether a char is in lowercase.
                    }
*/

/**
stdlib.h provides {
                    1. exit(EXIT_FAILURE) for exiting the program.
                    }
*/



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
char read_by_word(word_t* word);

/***************The main function started*************************************/
int
main(int argc, char *argv[]) {
    text_t text;

    // run stage 1, if stage1 returns FALSE, terminate this program.
    if (!stage1(argc, argv)){
        exit(EXIT_FAILURE);
    }

    // run stage 2
    text = stage2();

    printf("%d\n", text.curr_max_line_hold);
    printf("%d\n", text.lines->word_count   );
    return 0;
}

text_t stage2(){
    text_t text;
    char flag_of_line;
    // initialize text
    text.line_index = 0;
    text.curr_max_line_hold = INITIAL_NUMBER_OF_LINE_PER_TEXT;
    text.lines = (line_t*)malloc(INITIAL_NUMBER_OF_LINE_PER_TEXT * sizeof(line_t*));


    //keep reading words into text line by line, till the end of a file
    while ((flag_of_line = read_by_line(&text.lines[text.line_index], text.line_index)) != EOF){
        /*if we finish reading a line, then we need to create a new line*/
        if (flag_of_line == NEWLINE){
            // update text to move to the reading task of next line
            update_text(&text);
       }
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
    line->words = (word_t*)malloc(INITIAL_NUMBER_OF_WORD_PER_LINE * sizeof(word_t*));

    //local variable
    char flag_of_word;

    // keep reading words into line word by word, till meet a newline char
    while ((flag_of_word = read_by_word(&line->words[line->word_count])) != NEWLINE
            && flag_of_word != EOF){
        // update line statistics and line->words memory
        update_line(line);
    }

    return flag_of_word;
}

void update_line(line_t *line){
    line->word_count++;

    // check memory
    if (line->word_count >= line->curr_max_word_hold){
        line->curr_max_word_hold += INITIAL_NUMBER_OF_WORD_PER_LINE;
        line->words = (word_t*)realloc(line->words,
                                line->curr_max_word_hold * sizeof(word_t*));
    }
}

char read_by_word(word_t* word){
    return EOF;
}

void update_text(text_t *text){
    // increase the number of line stored in text
    text->line_index++;

    //check whether need to expand memory for allocating new line
    if (text->line_index >= text->curr_max_line_hold){
        text->curr_max_line_hold += INITIAL_NUMBER_OF_LINE_PER_TEXT;
        text->lines = (line_t*)realloc(text->lines,
                                        text->curr_max_line_hold * sizeof(line_t*));
    }
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
