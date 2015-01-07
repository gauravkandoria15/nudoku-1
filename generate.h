#ifndef GENERATE_C
#define GENERATE_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

char* generate();

typedef struct point {
    int x;
    int y;
} point;

typedef struct options {
    point pos[9];
    char val[9];
    int count;
} options;

struct possibilities {
    char possibleValues[9][9][9];
};

typedef struct possibilities Possibility;

bool solve(char board[9][9], char boardSolved[9][9]);
void printBoard(char board[9][9]);
bool checkBoard(char board[9][9]);
  
#endif // GENERATE_C
