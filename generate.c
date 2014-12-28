#include "generate.h"
#include "vector.h"

bool debug = false;

bool nextCellToFill(char board[9][9], vector* possibilities[9][9], point* position) {

    // return false if: no more possibilities to fill. this can be if the board is full, or there is no way to complete the board. 
    //search for the first unknown cell. can be chosen more intelligently
    int col, row, least;

    least = 10;

    for (col=0; col < 9; col++) {
        for (row=0; row < 9; row++) {
            if (board[row][col] == '.') {
                // if there is only one possibility left, choose it immediatly.
                if (possibilities[row][col]->count == 1) {
                    position->x = col;
                    position->y = row;
                    return true;
                }

                // find position with the minimal number of possibilities.
                if (possibilities[row][col]->count < least) {
                    least = possibilities[row][col]->count;
                    position->x = col;
                    position->y = row;

                    if (debug) {
                    printf("least %d, pos %d,%d \n",least, row, col);
                    printf("test %p\n",position);
                    fflush(stdout);
                    }
                }
            }
        }
    }

    // check wheter there have been empty fields left.
    if (least != 10) 
        return true;

    // if no empty field has been found return false.
    return false;
}

vector* getNumberToInsert(char board[9][9], vector* possibilities[9][9], point* position){
    // loop over everything that is still possible
    // insert first one (something smarter is also possible)

    // rank options

    // return ranked options, best one first.
    return possibilities[position->y][position->x];

}

void getPossibilities(char board[9][9], vector* possibilities[9][9]){
    // calc possibilities with board

    int i, j, row, col;
    char chars[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    point box;

    // loop over all fixed board numbers to initialize possibilities
    for (col = 0; col < 9; col++) {
        for (row = 0; row < 9; row++) {
            if (board[row][col] == '.') {
                possibilities[row][col] = vector_init(9);
                // if (row == 8 && col == 8)
                for (i = 0; i < 9; i++) {

                    vector_add_element(possibilities[row][col], (void*) (unsigned long) chars[i]);
                    // possibilities[row][col]->data[i] = (void*)(unsigned long) chars[i];

                }
            } else {
                possibilities[row][col] = NULL;
            }
        }
    }

    for (row = 0; row < 9; row ++){
        for (col = 0; col < 9; col ++) {
            if (board[row][col] != '.') {
                // vector_free(possibilities[row][col]);
                // Null or 'x' for no possibility? choose Null;

                // check current col
                for (i = 1; i < 9; i++)
                {
                    vector_delete_element_with_value(possibilities[row][(col + i) % 9], (void*)(unsigned long) board[row][col]);
                    if (row == 8 && (col + i)%9 == 8 && debug)
                    {
                        printf("deleting %c from clolumn %d \n", board[row][col], col + 1);
                        vector_print_char(possibilities[8][8]);
                    }

                }

                // check current row
                for (i = 1; i < 9; i++)
                {
                    vector_delete_element_with_value(possibilities[(row + i) % 9][(col)], (void*)(unsigned long) board[row][col]);
                    if ((row + i) % 9 == 8 && col  == 8 && debug)
                    {
                        printf("deleting %c from row %d \n", board[row][col], row + 1);
                        vector_print_char(possibilities[8][8]);
                    }
                }

                // check current box
                box.y = row/3;
                box.x = col/3;
                for (i = 0; i < 3; i++){
                    for (j = 0; j < 3; j++){
                        vector_delete_element_with_value(possibilities[(3 * box.y + i )][(3 * box.x + j)], (void*)(unsigned long) board[row][col]);
                        if (3 * box.y + i == 8 && (3 * box.x + j) == 8 && debug)//  && i == 2 && j == 2)
                        {
                            printf("deleting %c from (%d,%d)\n", board[row][col], col + 1, row + 1);
                            vector_print_char(possibilities[8][8]);
                        }
                    }
                }

                // vector_init(possibilities[col][row]);
            } 
        }
    }
    if (debug)
        vector_print_char(possibilities[8][8]);

}

bool solve(char board[9][9]) {
    // jede instanz von solve braucht eine eigene instanz von possibilities und board.
    vector* possibilities[9][9];
    vector* options;

    int col, row, i;
    point pos;
    pos.x = 0;
    pos.y = 0;

    getPossibilities(board, possibilities);

    if (nextCellToFill(board, possibilities, &pos))
    {
        printf("position %d, %d, value %c \n", pos.x, pos.y, board[pos.y][pos.x]);
        // options is a vector
        options = getNumberToInsert(board, possibilities, &pos);
        vector_print_char(options);
        printf("%d", options->count);
        for (i = 0; i < options->count; i++) {
            // is that transformation correct?
            board[pos.y][pos.x] = (char)(unsigned long) options->data[i];
            printf("try %c. new value: %c\n", options->data[i], board[pos.y][pos.x]);
            // solve cells that are currently empty
            if (solve(board)== true)
                return true;
        }
        return false;
    } else {
        // return true, if there is nothing left to fill.
        return true;
    }
}
