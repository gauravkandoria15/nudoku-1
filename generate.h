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

struct possibilities {
    char possibleValues[9][9][9];
};

typedef struct possibilities Possibility;

bool solve(char[9][9]);
bool nextCellToFill(char board[9][9], vector* possibilities[9][9], point* position);
void getPossibilities(char board[9][9], vector* possibilities[9][9]);
void printBoard(char board[9][9]);
bool checkBoard(char board[9][9]);
  
#endif // GENERATE_C
