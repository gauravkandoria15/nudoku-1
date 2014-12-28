#include "generate.h"
#include "vector.h"

bool debug = false;

void printBoard(char board[9][9]) {
    int i, j;
    for (i = 0; i<9; i++)
    {
        for (j = 0; j<9; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

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

bool boardSolved(char board[9][9]) {
    // check if board is filled.
    int row,col;

    // check for '.'
    for (row = 0; row < 9; row ++){
        for (col = 0; col < 9; col ++) {
            if (board[row][col] == '.') {
                return false;
            }
        }
    }
    return true;
}

bool checkBoard(char board[9][9]) {
    // check consistency of board.
    int i, j, row,col;

    char chars[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    vector* values = vector_init(9);
    vector* tmp;
    for (i = 0; i < 9; i++) {
        vector_add_element(values, (void*) (unsigned long) chars[i]);
    }

    tmp = vector_copy(values);


    // check rows
    printf("checking row: ");
    for (row = 0; row < 9; row ++){
        printf ("%d ", row);
        for (col = 0; col < 9; col ++) {
            // printf("deleting %c from:",board[row][col]);
            // vector_print_char(tmp);
            vector_delete_element_with_value(tmp, (void*) (unsigned long) board[row][col]);
        }
        if (tmp->count != 0){
            printf("(%d, %d), row wrong\n", row, col);
            return false;
        }
        vector_free(tmp);
        tmp = vector_copy(values);
    }
    printf ("\n");

    printf("checking col: ");
    // check cols
    for (col = 0; col < 9; col ++) {
        printf ("%d ", col);
        for (row = 0; row < 9; row ++){
            vector_delete_element_with_value(tmp, (void*) (unsigned long) board[row][col]);
        }
        if (tmp->count != 0) {
            printf("(%d, %d), col wrong\n", row, col);
            return false;
        }
        vector_free(tmp);
        tmp = vector_copy(values);
    }
    printf ("\n");

    // check boxes
    printf("checking box: ");
    for (i = 0; i < 9; i += 3) {
        for (j = 0; j < 9; j += 3) {
            printf ("(%d,%d) ", i,j);
            for (row = i; row < i + 3; row++) {
                for (col = j; col < j + 3; col++) {
                    vector_delete_element_with_value(tmp,  (void*) (unsigned long) board[row][col]);
                }
            }
            if (tmp->count !=0) {
                printf("(%d, %d), box wrong\n", row, col);
                return false;
            }
            vector_free(tmp);
            tmp = vector_copy(values);
        }
    }
    printf ("\n");

    return true;
}

bool solve(char board[9][9]) {
    // jede instanz von solve braucht eine eigene instanz von possibilities und board.
    vector* possibilities[9][9];
    vector* options;
    printBoard(board);
    char boardLocal[9][9];




    int i, j;
    for (i=0; i < 9; i++) {
        for (j=0; j < 9; j++) {
            boardLocal[i][j] = board[i][j];
        }
    }

    point pos;
    pos.x = 0;
    pos.y = 0;

    getPossibilities(boardLocal, possibilities);

    if (nextCellToFill(boardLocal, possibilities, &pos))
    {
        printf("position %d, %d, value %c \n", pos.x, pos.y, boardLocal[pos.y][pos.x]);
        // options is a vector
        options = getNumberToInsert(boardLocal, possibilities, &pos);
        vector_print_char(options);
        printf("%d", options->count);
        for (i = 0; i < options->count; i++) {
            // is that transformation correct?
            boardLocal[pos.y][pos.x] = (char)(unsigned long) options->data[i];
            printf("try %c. new value: %c\n", options->data[i], boardLocal[pos.y][pos.x]);
            // solve cells that are currently empty
            if (solve(boardLocal)== true)
                return true;
        }
        return false;
    } else {
        // return true, if there is nothing left to fill.
        return true;
    }
}
