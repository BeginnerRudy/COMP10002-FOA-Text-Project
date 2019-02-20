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
#define INITIAL_NUMBER_OF_WORDS_PER_LINE 100
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
char getword(line_t* line, int *word_count, int *byte_count);

line_t line_linitializer(int line_count);
void line_update(char last_char_read_by_getword, int *word_count,
                int *byte_count, int *line_index, int *curr_max_word_hold,
                line_t **local_text);

// void print_line(char** line){
//     int word_count = 0;
//     printf("[ ");
//     while (line[word_count] != NULL){
//         printf("%s, ", line[word_count++]);
//     }
//     printf("]\n");
// }
//
// void print_text(line_t *text){
//     int line_count = 0;
//     printf("[\n");
//     while (text[line_count] != NULL){
//         print_line(text[line_count++]);
//     }
//     printf("]\n");
// }



/***************The main function started*************************************/
int
main(int argc, char *argv[]) {
    // A text is an array of variables of type line_t.
    line_t *text;

    if (!stage1(argc, argv)){
        exit(EXIT_FAILURE);
    }

    // Let stage2 do the job of reading text and do some statistics
    text = stage2();

    printf("%d\n", text[0].line_index);
    printf("%d\n", text[0].word_count);
    printf("%d\n", text[0].byte_count);
    printf("%s\n", text[0].line[0]);
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
    line_t *local_text = (line_t*)malloc(INITIAL_NUMBER_OF_LINE*sizeof(line_t*));
    int curr_max_line_hold = INITIAL_NUMBER_OF_LINE,
        curr_max_word_hold = INITIAL_NUMBER_OF_WORDS_PER_LINE,
        line_count = 0, word_count = 0, byte_count = 0;
    char last_char_read_by_getword;

    local_text[line_count] = line_linitializer(line_count);
    last_char_read_by_getword = getword(&local_text[line_count], &word_count, &byte_count);
    line_update(last_char_read_by_getword, &word_count, &byte_count, &line_count,
         &curr_max_word_hold, &local_text);

    while (last_char_read_by_getword != EOF){
        // decide weather to reallocate for a line inside a line_t
        if (word_count >= curr_max_word_hold){
            curr_max_word_hold += INITIAL_NUMBER_OF_WORDS_PER_LINE;
            local_text[line_count].line = (char**)realloc(local_text[line_count].line,
                                                            curr_max_word_hold*sizeof(char**));
        }

        last_char_read_by_getword = getword(&local_text[line_count], &word_count, &byte_count);
        line_update(last_char_read_by_getword, &word_count, &byte_count, &line_count,
             &curr_max_word_hold, &local_text);
    }

    printf("\n\n%s\n\n", local_text[0].line[0]);

    // local_text[line_count] = NULL;
    return local_text;
}

void line_update(char last_char_read_by_getword, int *word_count,
                int *byte_count, int *line_index, int* curr_max_word_hold,
                line_t **local_text){
    if (last_char_read_by_getword == NEWLINE){
        //assign final value to current line
        (*local_text)[*line_index].word_count = *word_count;
        (*local_text)[*line_index].byte_count = *byte_count;

        //reset all values to zero while increment line index and allocate a new line
        *word_count = *byte_count = 0;
        *curr_max_word_hold = INITIAL_NUMBER_OF_WORDS_PER_LINE;
        *line_index++;



        (*local_text)[*line_index] = line_linitializer(*line_index);
    }
}

char getword(line_t* line, int *word_count, int *byte_count){
    char c;
    int char_count = 0, curr_max_char_hold = INITIAL_NUMBER_OF_CHARS;
    // allocate the place to store word
    line->line[*word_count] = (char*)malloc(INITIAL_NUMBER_OF_CHARS * sizeof(char*));

    // skip the non-alpabetic chars
    // once meet the EOF or newline char, stop the loop
    while (!isalpha(c=getchar()) && c != EOF && c != NEWLINE) {
        printf("%c", c);
        *byte_count += 1;
    }

    // if it is the end of the file or a line, stop the function
    if (c == EOF || c == NEWLINE){
        printf("%c", c);
        *byte_count++;
        return c;
    }

    // Append the 1st char to word
    printf("%c", c);
    *byte_count++;
    line->line[*word_count][char_count++] = c;

    // Keep reading
    while (isalpha(c = getchar())){
        // decide wheather to reallocate memory
        if (char_count >= curr_max_char_hold){
            curr_max_char_hold += INITIAL_NUMBER_OF_CHARS;
            line->line[*word_count] = (char*)realloc(line->line[*word_count], curr_max_char_hold);
        }
        printf("%c", c);
        *byte_count++;
        line->line[*word_count][char_count++] = c;
    }

    printf("%c", c);
    *byte_count++;
    // decide wheather to reallocate memory for NULL byte
    if (char_count >= curr_max_char_hold){
        curr_max_char_hold ++;
        line->line[*word_count] = (char*)realloc(line->line[*word_count], curr_max_char_hold);
    }

    // Append NULL byte at last
    line->line[*word_count][char_count] = NULL_BYTE;
    *word_count++;

    return c;
}

line_t line_linitializer(int line_count){
    // declare and allocate a line
    line_t line;
    line.word_count = 0;
    line.byte_count = 0;
    line.score = 0.0;
    line.line_index = line_count;
    line.line = (char**)malloc(INITIAL_NUMBER_OF_WORDS_PER_LINE * sizeof(char**));

    return line;
}
