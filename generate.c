#include "generate.h"
#include "vector.h"

bool nextCellToFill(char board[9][9], point* position) {

    //search for the first unknown cell. can be chosen more intelligently
    int col, row;
    for (col=0; col < 9; col++){
        for (row=0; row < 9; row++){
            if (board[row][col] == '.')
            {
                position->y = row;
                position->x = col;
                return true;
            }
        }
    }
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
    
    int i, j, k, row, col;
    point box;

    // loop over all fixed board numbers to initialize possibilities
    for (row = 0; row < 9; row ++){
        for (col = 0; col < 9; col ++) {
            if (board[row][col] != '.') {
                // vector_free(possibilities[row][col]);
                // Null or 'x' for no possibility? choose Null;

                // check current col
                for (i = 1; i < 9; i++)
                {
                    vector_delete_element_with_value(possibilities[row][(col + i) % 9], (void*)(unsigned long) board[row][col]);
                    if (row == 8 && (col + i)%9 == 8)
                    {
                        printf("deleting %c from clolumn %d \n", board[row][col], col + 1);
                        vector_print_char(possibilities[8][8]);
                    }

                }

                // check current row
                for (i = 1; i < 9; i++)
                {
                    vector_delete_element_with_value(possibilities[(row + i) % 9][(col)], (void*)(unsigned long) board[row][col]);
                    if ((row + i) % 9 == 8 && col  == 8)
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
                        if (3 * box.y + i == 8 && (3 * box.x + j) == 8)//  && i == 2 && j == 2)
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
    vector_print_char(possibilities[8][8]);

}

bool solve(char board[9][9]) {
    vector* possibilities[9][9];
    vector* options;

    int col, row, i;
    point pos;
    pos.x = 0;
    pos.y = 0;
    if (nextCellToFill(board, &pos))
    {
        // printf("position %d, %d, value %c \n", row, col, board[row][col]);
        // options is a vector
        options = getNumberToInsert(board, possibilities, &pos);
        for (i = 0; i < options->count; i++) {
            // is that transformation correct?
            board[pos.x][pos.y] = (char)(unsigned long) options->data[i];
            // solve cells that are currently empty
            if (solve(board)== true)
                return true;
        }
        return false;
    }

    return true;
}
