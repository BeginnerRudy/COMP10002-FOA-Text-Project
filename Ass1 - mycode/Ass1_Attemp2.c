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

/**Define the Constant used in this program*/

// A word from the input could have up to 200 chars
#define MAX_CHARS_PER_WORD 50



#typedef char[MAX_CHARS_PER_WORD] word_t;


int
main(int argc, char *argv[]) {
    return 0;
}
