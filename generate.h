#ifndef GENERATE_C
#define GENERATE_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* getopt */

char* generate();

struct point {
    int x;
    int y;
};

typedef struct point Point;

struct possibilities {
    char possibleValues[9][9][9];
};

typedef struct possibilities Possibility;

char** solve(char[9][9]);
void nextCellToFill(char[9][9], Point*);
  
#endif // GENERATE_C
