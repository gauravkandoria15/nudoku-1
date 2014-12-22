#include "generate.h"
#include <stdio.h>

// char board[9][9];




void nextCellToFill(char board[9][9], Point* position) {

    //search for the first unknown cell. can be chosen more intelligently
    int col, row;
    for (col=0; col < 9; col++){
        for (row=0; row < 9; row++){
            if (board[row][col] == '.')
            {
                position->y = row;
                position->x = col;
            }
            position = 0;

            printf("test");
    printf("%p",position);
    fflush(stdout);
        }
    }
}

char** solve(char board[9][9]) {

    int col, row;
    Point pos;
    pos.x = 0;
    pos.y = 0;
    printf("%p",&pos);
    nextCellToFill(board, &pos);
    if (&pos != 0)
        for (col=0; col < 9; col++){
            for (row=0; row < 9; row++){
                // printf("position %d, %d, value %c \n", row, col, board[row][col]);
                if (board[row][col] == '.')
                {

                    // solve cells that are currently empty
                }

            }
        }
    return board;
}
