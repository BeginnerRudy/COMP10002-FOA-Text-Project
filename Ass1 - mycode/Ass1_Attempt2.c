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
// Define alias, TRUE for 1 as well as FALSE for 0.
#define TRUE 1
#define FALSE 0
// Since the queries in argv start from index 1, we make a constant for it.
#define THE_START_INDEX_OF_QUERY 1

/**Define new data type and strucTRUE*****************************************/

// The new type word_t is used to storing a word.
typedef char word_t[MAX_CHARS_PER_WORD];
typedef int bool;

/**Define the function prototypes*********************************************/

/*This function is responsible for all the activity of stage1, TRUE means
pass the stage1 successfully, FALSE means failed to pass.*/
bool stage1(int argc, char *argv[]);
// This function returns TRUE if word consisted of all lowercase chars,
// otherwise, return FALSE.
bool does_contain_uppercase_for_word(word_t word);
// Add a newline to the print out content
void add_a_new_line_char();


/***************The main function started*************************************/
int
main(int argc, char *argv[]) {
    // run stage 1, if stage1 returns FALSE, terminate this program.
    if (!stage1(argc, argv)){
        exit(EXIT_FAILURE);
    }

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
        if (does_contain_uppercase_for_word(argv[i])){
            printf("S1: %s: invalid character(s) in query\n", argv[i]);
            return FALSE;
        }
    }

    // Pass all the two test, return TRUE, good!
    return TRUE;
}

bool does_contain_uppercase_for_word(word_t word){
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
