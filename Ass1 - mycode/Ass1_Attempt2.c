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
void stage2(char*** text);
/*This function grap a word from the text and assign it to the argument var*/
/*Then, it returns the last char it read.*/
char getword(word_t word);
/*This function read a word into a char**/
void read_into_word(char* place_to_store, word_t word, int *word_count_for_curr_line);
/*This function response for updatinf the number of lines in the test*/
void update_line_count(char last_char_read_by_getword, int *line_count,
                            int *word_count_for_curr_line, char **line);


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
    char ***text = (char***)malloc(INITIAL_NUMBER_OF_LINE * sizeof(char***));

    if (!stage1(argc, argv)){
        exit(EXIT_FAILURE);
    }

    stage2(text);
    printf("%s\n", text[0][0]);
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

void stage2(char*** text){
    word_t a_word;
    char last_char_read_by_getword;
    int line_count = 0, word_count_for_curr_line = 0,
    curr_max_line_hold = INITIAL_NUMBER_OF_LINE,
    curr_max_word_hold = INITIAL_NUMBER_OF_WORDS_PER_LINE;

    text[line_count] = (char**)malloc(INITIAL_NUMBER_OF_WORDS_PER_LINE * sizeof(char**));

    last_char_read_by_getword = getword(a_word);

    text[line_count][word_count_for_curr_line] = (char*)malloc(INITIAL_NUMBER_OF_CHARS * sizeof(char*));
    read_into_word(text[line_count][word_count_for_curr_line], a_word, &word_count_for_curr_line);

    update_line_count(last_char_read_by_getword, &line_count, &word_count_for_curr_line,
                        text[line_count]);

    while (last_char_read_by_getword != EOF){
        last_char_read_by_getword = getword(a_word);

        update_line_count(last_char_read_by_getword, &line_count, &word_count_for_curr_line,
                            text[line_count]);

        //check whether needs to reallocate memory for text
        if (line_count >= curr_max_line_hold){
            curr_max_line_hold += INITIAL_NUMBER_OF_LINE;
            text = (char ***)realloc(text, curr_max_line_hold*sizeof(char***));
        }

        //check whether needs to reallocate memory for current line
        if (word_count_for_curr_line >= curr_max_word_hold){
            curr_max_word_hold += INITIAL_NUMBER_OF_WORDS_PER_LINE;
            text[line_count] = (char **)realloc(text[line_count],
                                        curr_max_word_hold*sizeof(char**));
        }

        read_into_word(text[line_count][word_count_for_curr_line], a_word, &word_count_for_curr_line);
    }

    text[line_count] = NULL;
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

void read_into_word(char* place_to_store, word_t word, int *word_count_for_curr_line){
    int char_count = 0, curr_max_char_hold = INITIAL_NUMBER_OF_CHARS;

    while (word[char_count] != NULL_BYTE){
        // Check whether needs to realocate the array
        if (char_count >= curr_max_char_hold){
            curr_max_char_hold += INITIAL_NUMBER_OF_CHARS;
            place_to_store = (char *)realloc(place_to_store, curr_max_char_hold * sizeof(char*));
        }

        // Append the char
        place_to_store[char_count] = word[char_count];
        char_count++;
    }

    place_to_store[char_count] = NULL_BYTE;
    word_count_for_curr_line++;
}

void update_line_count(char last_char_read_by_getword, int *line_count,
                                int *word_count_for_curr_line, char **line){
    if (last_char_read_by_getword == NEWLINE){
        *line_count++;
        line[*word_count_for_curr_line] = NULL;
        *word_count_for_curr_line = 0;
    }
}
