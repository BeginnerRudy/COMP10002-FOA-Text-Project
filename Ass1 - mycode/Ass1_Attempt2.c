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
#define NULL_BYTE '\0'
#define NEWLINE '\n'
#define INITIAL_NUMBER_OF_WORDS_PER_LINE 10
#define INITIAL_NUMBER_OF_LINE 10
#define INITIAL_NUMBER_OF_CHARS 3
// Define alias, TRUE for 1 as well as FALSE for 0.
#define TRUE 1
#define FALSE 0
// Since the queries in argv start from index 1, we make a constant for it.
#define THE_START_INDEX_OF_QUERY 1

/**Define new data type and structrue*****************************************/

// The new type word_t is used to storing a word.
typedef char word_t[MAX_CHARS_PER_WORD];
// The bool type is an int that indicate boolean
typedef int bool;

typedef struct{
    char **line;
    int byte_count;
    int word_count;
    int line_index;
    double score;
}line_t;

/**Define the function prototypes*********************************************/

/*This function is responsible for all the activity of stage1, TRUE means
pass the stage1 successfully, FALSE means failed to pass.*/
bool stage1(int argc, char *argv[]);
// This function returns TRUE if word consisted of all lowercase chars,
// otherwise, return FALSE.
bool does_a_word_contain_uppercase(word_t word);
// Add a newline to the print out content
void add_a_new_line_char();
/*This function is responsible for all the activity of stage2.*/
line_t* stage2();
/*This function grap a word from the text and assign it to the argument var*/
/*Then, it returns the last char it read.*/
char getword(word_t word);

void print_line(char** line){
    int word_count = 0;
    printf("[ ");
    while (line[word_count] != NULL){
        printf("%s, ", line[word_count++]);
    }
    printf("]\n");
}

void print_text(char*** text){
    int line_count = 0;
    printf("[\n");
    while (text[line_count] != NULL){
        print_line(text[line_count++]);
    }
    printf("]\n");
}



/***************The main function started*************************************/
int
main(int argc, char *argv[]) {
    // A text is an array of variables of type line_t.
    line_t *text;

    if (!stage1(argc, argv)){
        exit(EXIT_FAILURE);
    }

    text = stage2();
    text[0].byte_count;
    // printf("%s\n", text[0][0]);
    // print_text(text);
    return 0;
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
        if (does_a_word_contain_uppercase(argv[i]) == TRUE){
            printf("S1: %s: invalid character(s) in query\n", argv[i]);
            return FALSE;
        }
    }

    // Pass all the two test, return TRUE, good!
    return TRUE;
}

bool does_a_word_contain_uppercase(word_t word){
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

line_t* stage2(){
    line_t local_text;

    local_text.byte_count = 10;

    return &local_text;
}

char getword(word_t word){
    char c;
    int count = 0;

    // skip the non-alpabetic chars
    // once meet the EOF, stop the loop
    while (!isalpha(c=getchar()) && c != EOF && c != NEWLINE) {
    }

    // if it is the end of the file, stop the function
    if (c == EOF || c == NEWLINE){
        return c;
    }

    // Append the 1st char to word
    word[count++] = c;

    // Keep reading
    while (isalpha(c = getchar())){
        word[count++] = c;
    }

    // Append NULL byte at last
    word[count] = NULL_BYTE;

    return c;
}
