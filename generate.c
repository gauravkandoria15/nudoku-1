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
    // just insert the possibilitie vector 
    // (something smarter is also possible eg. ranking options by how much choices 
    // they leave on the most restricted cell)

    // return ranked options, best one first. (no ranking done so far.)
    return possibilities[position->y][position->x];

}

bool findWellLocatedNumbers(char* boardSnipplet[9], vector** possibilitiesSnipplet[9], options* options, int *least){
    // check if there are numbers, that are only allowed in few posistions
    int elementNumber, nFound, i;
    int count[9];
    bool foundBetterSolution = false;

    // initialize count.
    for(i = 0; i < 9; i ++) {
        count[i] = 0;
    }

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
            foundBetterSolution = true;
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
                    // if a unique solution is found, take it and abort the search.
                    if (count[i] == 1)
                        return foundBetterSolution;
                }
            }
        }
    }
    return foundBetterSolution;
}

bool nextCellToFill(char board[9][9], vector* possibilities[9][9], options* options) {

    // return false if: no more possibilities to fill. this can be if the board is full, or there is no way to complete the board. 
    int col, row, i, least;
    point boxOrigin;

    char* boardSnipplet[9];
    vector** possibilitiesSnipplet[9];


    // initialize 'least' with a value higher than the highest possible value (9).
    least = 10;

    for (col=0; col < 9; col++) {
        for (row=0; row < 9; row++) {
            if (board[row][col] == '.') {
                printf("%d ", possibilities[row][col]->count);
                // if there is no possibility to fill this gap return false immediately.
                if (possibilities[row][col]->count == 0) {
                    printf("bad branch\n");
                    return false;
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

                }
            }
        }
    }


    // if there is at least one cell with no legal value, or if no empty cell is left return false
    if (least == 10)
        return false;


    // now check if there are numbers, that are only allowed in few posistions
    // check rows first
    for (row = 0; row < 9; row++){
        for (col = 0; col < 9; col++){
            // write arrays of pointers to the relevant part of the board
            boardSnipplet[col] = &board[row][col];
            possibilitiesSnipplet[col] = &possibilities[row][col];
        }

        if (findWellLocatedNumbers(boardSnipplet, possibilitiesSnipplet, options, &least )){
            // transform from indices of the row to position
            printf("better option in row ");
            for (i = 0; i < options->count; i++){
                options->pos[i].y = row;
                options->pos[i].x = options->pos[i].x;
            }
        }
    }

    // exit if an optimal solution has been found
    if (least == 1){
        printf("least: %d\n", least);
        return true;
    }

    // check collumns
    for (col = 0; col < 9; col++){
        for (row = 0; row < 9; row ++) {
            // write arrays of pointers to the relevant part of the board
            boardSnipplet[row] = &board[row][col];
            possibilitiesSnipplet[row] = &possibilities[row][col];
        }

        if (findWellLocatedNumbers(boardSnipplet, possibilitiesSnipplet, options, &least )){
            // transform from indices of the row to position
            printf("better option in col ");
            for (i = 0; i < options->count; i++){
                options->pos[i].y = options->pos[i].x;
                options->pos[i].x = col;
            }
        }
    }

    // exit if an optimal solution has been found
    if (least == 1) {
        printf("least: %d\n", least);
        return true;
    }

    // check boxes
    for (boxOrigin.y = 0; boxOrigin.y < 9; boxOrigin.y += 3){
        for (boxOrigin.x = 0; boxOrigin.x < 9; boxOrigin.x += 3){
            // in each box:
            for (row = boxOrigin.y; row < boxOrigin.y + 3 ; row++){
                for (col = boxOrigin.x; col < boxOrigin.x + 3 ; col++){
                    boardSnipplet[3 * (row - boxOrigin.y) + (col - boxOrigin.x)] = &board[row][col];
                    possibilitiesSnipplet[3 * (row - boxOrigin.y) + (col - boxOrigin.x)] = &possibilities[row][col];
                }
            }

            if (findWellLocatedNumbers(boardSnipplet, possibilitiesSnipplet, options, &least )){
                // transform from indices of the row to position
                printf("better option in box ");
                for (i = 0; i < options->count; i++){
                    // temp = options->pos[i].x;
                    options->pos[i].y = boxOrigin.y + options->pos[i].x/3;
                    options->pos[i].x = boxOrigin.x + options->pos[i].x % 3;
                }
            }
        }
    }

    printf("least: %d\n", least);
    return true;
}

void getPossibilities(char board[9][9], vector* possibilities[9][9]){
    // find numbes for each unfilled cell, that are compatibel with the board 'board'.

    int i, j, row, col;
    char chars[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    point box;

    // loop over all fixed board numbers to initialize possibilities
    for (col = 0; col < 9; col++) {
        for (row = 0; row < 9; row++) {
            if (board[row][col] == '.') {
                possibilities[row][col] = vector_init(9);
                for (i = 0; i < 9; i++) {
                    vector_add_element(possibilities[row][col], (void*) (unsigned long) chars[i]);
                }
            } else {
                possibilities[row][col] = NULL;
            }
        }
    }

    for (row = 0; row < 9; row ++){
        for (col = 0; col < 9; col ++) {
            if (board[row][col] != '.') {
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
                        if (3 * box.y + i == 8 && (3 * box.x + j) == 8 && debug)
                        {
                            printf("deleting %c from (%d,%d)\n", board[row][col], col + 1, row + 1);
                            vector_print_char(possibilities[8][8]);
                        }
                    }
                }
            } 
        }
    }
    if (debug)
        vector_print_char(possibilities[8][8]);

}

bool boardIsSolved(char board[9][9]) {
    // check if board is filled.
    int row,col;

    // search for '.'
    for (row = 0; row < 9; row ++){
        for (col = 0; col < 9; col ++) {
            if (board[row][col] == '.') {
                // if there is a '.' the board is not solved.
                return false;
            }
        }
    }
    // board is solved if there is no '.' left.
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


void refreshDependentPossibilities(vector* possibilities[9][9], char value, int row, int col) {
    // updates the possibilities after a number has been written in one position of the board.
    int currRow, currCol;

    // delete possibilities from row:
    for (currCol= 0; currCol < 9; currCol++) {
        vector_delete_element_with_value(possibilities[row][currCol], (void*) (unsigned long) value);
    }

    // delete possibilities from col
    for (currRow= 0; currRow < 9; currRow++) {
        vector_delete_element_with_value(possibilities[currRow][col], (void*) (unsigned long) value);
    }
    // delete possibilities from box
    row = (row/3) *3;
    col = (col/3) *3;
    for (currRow = row; currRow < row + 3; currRow ++) {
        for (currCol = col; currCol < col + 3; currCol ++) {
            vector_delete_element_with_value(possibilities[currRow][currCol], (void*) (unsigned long) value);
        }
    }

}

bool fillUnambiguousImmediately(char board[9][9], vector* possibilities[9][9]) {
    // fills the first unambiguous cell of the board then returns true
    // if there is none, returns false
    int row, col;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '.') {
                if (possibilities[row][col]->count == 1) {
                    board[row][col] = (char) possibilities[row][col]->data[0];
                    refreshDependentPossibilities(possibilities, board[row][col], row, col);
                    return true;
                }
            }
        }
    }
    return false;
}


bool solve(char board[9][9], char boardSolved[9][9]) {
    // solves the board
    vector* possibilities[9][9];
    options options;
    int i, j;
    char boardLocal[9][9];

    // if the board is solved and the solution is valid return true
    if (boardIsSolved(board)) {
        // copy solution into the solution board
        copyBoard(boardSolved, board);
        return true;
    }


    getPossibilities(board, possibilities);

    // copy board from calling function into local variable.
    for (i=0; i < 9; i++) {
        for (j=0; j < 9; j++) {
            boardLocal[i][j] = board[i][j];
        }
    }

    // fill all unambigous cells immediately.
    while (fillUnambiguousImmediately(boardLocal, possibilities));

    // check again, if the board is solved. the solution should be always correct.
    if (boardIsSolved(boardLocal)){
        if (checkBoard(boardLocal)) {
            copyBoard(boardSolved, boardLocal);
            return true;
        } 
        else { 
            printf("\n\nfound a wrong solution");
            return false;
        } 
    }

    if (nextCellToFill(boardLocal, possibilities, &options))
    {
        for (i = 0; i < options.count; i++) {
            // try proposed number.
            boardLocal[options.pos[i].y][options.pos[i].x] = options.val[i];
            // solve next cell that are currently empty
            if (solve(boardLocal, boardSolved)== true) {
                // success!
                // pass the good news to top layer.
                return true;
            }
        }
        // return false, if in one cell no possibilitie solves the board.
        return false;
    } else {
        // return false, if there is no next cell to fill.
        return false;
    }
}
