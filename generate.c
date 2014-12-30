#include "generate.h"
#include "vector.h"

bool debug = false;

void copyBoard(char dstBoard[9][9], char srcBoard[9][9]) {
    int row, col;
    for (row = 0; row < 9; row ++) {
        for (col = 0; col < 9; col ++) {
            dstBoard[row][col] = srcBoard[row][col];
        }
    }
    return;
}

void printBoard(char board[9][9]) {
    int i, j;
    for (i = 0; i<9; i++) {
        for (j = 0; j<9; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

vector* getNumberToInsert(char board[9][9], vector* possibilities[9][9], point* position){
    // loop over everything that is still possible
    // insert first one (something smarter is also possible)

    // rank options

    // return ranked options, best one first.
    return possibilities[position->y][position->x];

}

bool findWellLocatedNumbers(char* boardSnipplet[9], vector** possibilitiesSnipplet[9], int *least, options* options){
    // now check if there are numbers, that are only allowed in few posistions
    int elementNumber, nFound, i;
    int count[9];

    // count the occurences of the possibility of each number in the row 'row'.
    for (elementNumber = 0; elementNumber < 9; elementNumber++){
        if (*boardSnipplet[elementNumber] == '.') {
            for (i = 0; i < (*possibilitiesSnipplet[elementNumber])->count; i++) {
                // add one in the corresponding slot.
                count[ (int) ((*possibilitiesSnipplet[elementNumber])->data[i] - '0')] ++;
            }

        }
    }

    // check which entry has the lowest count
    for (i = 0; i < 9; i++) {
        // check if there is a better option.
        if (0 < count[i] && count[i] < *least){
            nFound = 0;
            *least = count[i];
            options->count = count[i];
            // find the positions in the row where the possibilities occured
            for (elementNumber = 0; elementNumber < 9; elementNumber++) {
                // possibilities is only initialized where board is '.', therefore check fist.
                if (*boardSnipplet[elementNumber] == '.' && vector_contains_value(*possibilitiesSnipplet[elementNumber], (void*) (unsigned long) (i + '0'))) {
                    options->pos[nFound].x = elementNumber;
                    options->pos[nFound].y = 0;
                    options->val[nFound] = i + '0';
                    nFound++;
                }
            }
        }
    }
    if (nFound == 0)
        return false;
    else
        return true;
}

bool nextCellToFill(char board[9][9], vector* possibilities[9][9], options* options) {

    // return false if: no more possibilities to fill. this can be if the board is full, or there is no way to complete the board. 
    //search for the first unknown cell. can be chosen more intelligently
    int col, row, i, least, nFound;
    int count[9];


    least = 10;

    for (col=0; col < 9; col++) {
        for (row=0; row < 9; row++) {
            if (board[row][col] == '.') {
                // if there is only one possibility left, choose it immediatly.
                if (possibilities[row][col]->count == 1) {
                    options->count = 1;
                    options->pos[0].x = col;
                    options->pos[0].y = row;
                    options->val[0] =  (char) (unsigned long) possibilities[row][col]->data[0];
                    return true;
                }

                // find position with the minimal number of possibilities.
                if (possibilities[row][col]->count < least) {
                    least = possibilities[row][col]->count;
                    options->count = possibilities[row][col]->count;
                    for (i = 0; i < options->count; i++) {
                        options->pos[i].x = col;
                        options->pos[i].y = row;
                        options->val[i] = (char) ((unsigned long) possibilities[row][col]->data[i]);
                    }

                    if (debug) {
                        printf("position %d, %d possible value %c", options->pos[i].x, options->pos[i].y, options->val[i]);
                        printf("least %d, pos %d,%d \n",least, row, col);
                        // printf("test %p\n",options->pos[0]);
                        fflush(stdout);
                    }
                }
            }
        }
    }

    // if there is at least one cell with no legal value, or if no empty cell is left return false
    if (least == 0 || least == 10)
        return false;

    // now check if there are numbers, that are only allowed in few posistions
    // check rows first
    for (row = 0; row < 9; row++){
        // reset counter.
        for (i = 0; i < 9; i++){
            count[i] = 0;
        }

        // count the occurences of the possibility of each number in the row 'row'.
        for (col = 0; col < 9; col++){
            if (board[row][col] == '.') {
                for (i = 0; i < possibilities[row][col]->count; i++) {
                    // add one in the corresponding slot.
                    count[ (int) (possibilities[row][col]->data[i] - '0')] ++;
                }

            }
        }

        // check which entry has the lowest count
        for (i = 0; i < 9; i++) {
            // check if there is a better option.
            if (0 < count[i] && count[i] < least){
                nFound = 0;
                least = count[i];
                options->count = count[i];
                // find the positions in the row where the possibilities occured
                for (col = 0; col < 9; col++) {
                    // possibilities is only initialized where board is '.', therefore check fist.
                    if (board[row][col] == '.' && vector_contains_value(possibilities[row][col], (void*) (unsigned long) (i + '0'))) {
                        options->pos[nFound].x = col;
                        options->pos[nFound].y = row;
                        options->val[nFound] = i + '0';
                        nFound++;
                    }
                }
            }
        }
    }
    return true;
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

bool boardIsSolved(char board[9][9]) {
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

bool solve(char board[9][9], char boardSolved[9][9]) {
    // jede instanz von solve braucht eine eigene instanz von possibilities und board.
    vector* possibilities[9][9];
    options options;
    // printBoard(board);
    char boardLocal[9][9];


    // if the board is solved and the solution is valid return true
    if (boardIsSolved(board)) {
        // pass solution to top.
        copyBoard(boardSolved, board);
        return true;
    }



    int i, j;
    for (i=0; i < 9; i++) {
        for (j=0; j < 9; j++) {
            boardLocal[i][j] = board[i][j];
        }
    }

    getPossibilities(boardLocal, possibilities);


    if (nextCellToFill(boardLocal, possibilities, &options))
    {
        // printf("position %d, %d, value %c \n", pos.x, pos.y, boardLocal[pos.y][pos.x]);
        // options is a vector
        // options = getNumberToInsert(boardLocal, possibilities, &pos);
        // vector_print_char(options);
        // printf("%d", options->count);
        for (i = 0; i < options.count; i++) {
            // is that transformation correct?
            boardLocal[options.pos[i].y][options.pos[i].x] = options.val[i];
            // printf("try %c. at position %d, %d\n", options->data[i], pos.x, pos.y, boardLocal[pos.y][pos.x]);
            // solve cells that are currently empty
            if (solve(boardLocal, boardSolved)== true) {
                // success!
                // pass the good news to top layer.
                return true;
            }
        }
        // return false, if in one cell all possibilities are illegal.
        return false;
    } else {
        // return false, if there is at least one cell with no legal value.
        return false;
    }
}
