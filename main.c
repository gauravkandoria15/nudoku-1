/*
nudoku 0.1

Copyright (C) 2014 Michael "jubalh" Vetter - g.bluehut _a-t_ gmail.com

LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * TODO:
 * read sudoku from CSV
 * save to CSV
 * hint (solve one number)
 * solve wholte sudoku
 * use vi keybindings (and arrows) -> nsudoku.c
 * use nice grid -> nsuds.c
 * two windows, one sudoku grind, one with description of keys
 * save to file not as CSV but the actual grid
 */

/*
 * Conventions:
 * board is the model of the sudoku. a two dimensional array.
 * grid is the visual represantation of the sudoku.
 */

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* getopt */
#include <ncurses.h>
#include "generate.h"


/* DEFINES */
#define VERSION				"0.1"
#define GRID_LINES			19
#define GRID_COLS			37
#define GRID_Y				3
#define GRID_X				3
#define INFO_LINES			25
#define INFO_COLS			20
#define INFO_Y				0
#define INFO_X				GRID_X + GRID_COLS + 5
#define GRID_NUMBER_START_Y 1
#define GRID_NUMBER_START_X 2
#define GRID_LINE_DELTA		4
#define GRID_COL_DELTA		2

#define EXAMPLE_STREAM "4.....8.5.3..........7......2.....6.....8.4......1.......6.32785..2.....1.4......"

/* GLOBALS */
bool g_useColor = true;
WINDOW *grid, *infobox;
char board[9][9];
char chars[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
vector* possibilities[9][9];

/* FUNCTIONS */
void print_version(void)
{
	printf("nudoku version " VERSION "\n\n\
Copyright (C) Michael Vetter 2014\n\
License GPLv3+: GNU GPL version 3 or later.\n\
This is free software, you are free to modify and redistribute it.\n");
}

void print_usage(void)
{
	printf("nudoku [option]\n\n");
	printf("Options:\n");
	printf("-h help:\t\t\tPrint this help\n");
	printf("-v version:\t\tPrint version\n");
	printf("-c nocolor:\t\tDo not use colors\n");
	printf("-l load filename:\tLoad sudoku from file\n");
}

void parse_arguments(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt(argc, argv, "hvcl:")) != -1)
	{
		switch (opt)
		{
			case 'h':
				print_usage();
				exit(EXIT_SUCCESS);
			case 'v':
				print_version();
				exit(EXIT_SUCCESS);
			case 'c':
				g_useColor = false;
				break;
			case 'l':
				printf("not yet implemented\n");
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}
}

void cleanup(void)
{
	endwin();
}

void init_curses(void)
{
	initscr();
	clear();
	atexit(cleanup);
	cbreak();
	noecho();

	if(g_useColor)
	{
		if(has_colors())
		{
			start_color();
			init_pair(1, COLOR_YELLOW, COLOR_GREEN);
			init_pair(2, COLOR_RED, COLOR_BLUE);
		}
		else
		{
			printw( "Your terminal doesn't support colors.\nTry the nocolor (-c) option.\n");
			getch();
			exit(EXIT_FAILURE);
		}
	}
}

void _draw_grid()
{
	int i, j;

	for(i = 0;i < 10;i++)
	{
		for(j = 0;j < 10;j++)
		{
			if (g_useColor)
			{
				if(i % 3 == 0)
					wattron(grid, A_BOLD|COLOR_PAIR(2));
				if(j % 3 == 0)
					wattron(grid, A_BOLD|COLOR_PAIR(2));
			}
			wprintw(grid, "+");
			if(g_useColor && (j % 3 == 0) && (i % 3 != 0))
			{
				wattron(grid, A_BOLD|COLOR_PAIR(1));
			}
			if(j < 9)
				wprintw(grid, "---");
			if(g_useColor && (i % 3 == 0))
			{
				wattron(grid, A_BOLD|COLOR_PAIR(1));
			}
		}
		for(j = 0;j < 10 && i < 9;j++)
		{
			if(g_useColor && (j % 3 == 0))
				wattron(grid, A_BOLD|COLOR_PAIR(2));
			if(j > 0)
				wprintw(grid, "   |");
			else
				wprintw(grid, "|");
			if(g_useColor && (j % 3 == 0))
			{
				wattron(grid, A_BOLD|COLOR_PAIR(1));
			}
		}
	}
}

void init_windows(void)
{
	keypad(stdscr, true);

	grid = newwin(GRID_LINES, GRID_COLS, GRID_Y, GRID_X);
	_draw_grid();

	infobox = newwin(INFO_LINES, INFO_COLS, INFO_Y, INFO_X);
	if (g_useColor)
	{
		wbkgd(infobox, COLOR_PAIR(2));
	}

	wprintw(infobox, "Commands\n");
	wprintw(infobox, " q - Quit\n");
	wprintw(infobox, " r - Redraw\n");
	wprintw(infobox, " h - Move left\n");
	wprintw(infobox, " l - Move right\n");
	wprintw(infobox, " j - Move down\n");
	wprintw(infobox, " k - Move up\n");
}

/*TODO: so far just ignoring longer stream. maybe check for 81*/
bool init_board(char *stream)
{
	int row, col;

	for(row=0; row < 9; row++)
	{
		for(col=0; col < 9; col++)
		{
			char *p = stream++;
			if(!((*p >= 49 && *p <= 57) || *p == '.' ))
				return false;
			board[row][col] = *p;
		}
	}
	return true;
}

void fill_grid(void)
{
	int row, col, x, y;
	char c;

	y = GRID_NUMBER_START_Y;
	for(row=0; row < 9; row++)
	{
		x = GRID_NUMBER_START_X;
		for(col=0; col < 9; col++)
		{
			c = board[row][col];
			if(c == '.')
				c = ' ';
			mvwprintw(grid, y, x, "%c", c);
			x += GRID_LINE_DELTA;
		}
		y += GRID_COL_DELTA;
	}
}

int main2(int argc, char *argv[])
{
	bool run = true;
	int key, x, y;

	parse_arguments(argc, argv);
	init_curses();

	init_windows();

	init_board(EXAMPLE_STREAM);
	fill_grid();

	refresh();
	wrefresh(grid);
	wrefresh(infobox);

	y = GRID_NUMBER_START_Y;
	x = GRID_NUMBER_START_X;
	wmove(grid, y, x);
	while(run)
	{
		//mvprintw(0, 0, "y: %.2d x: %.2d", y, x);
		refresh();
		wrefresh(grid);
		key = getch();
		switch(key)
		{
			case 'h':
			case KEY_LEFT:
				if(x>5)
					x -= GRID_LINE_DELTA;
				break;
			case 'l':
			case KEY_RIGHT:
				if(x<34)
					x += GRID_LINE_DELTA;
				break;
			case 'k':
			case KEY_UP:
				if(y>2)
					y -= GRID_COL_DELTA;
				break;
			case 'j':
			case KEY_DOWN:
				if(y<17)
					y += GRID_COL_DELTA;
				break;
			case 'q':
			case 27:
				run = false;
				break;
			case 'r':
				redrawwin(grid);
				redrawwin(infobox);
				break;
		}
		/*if user inputs a number*/
		if(key >= 49 && key <= 57)
		{
			/*if on empty position*/
			if(board[(y-GRID_NUMBER_START_Y)/GRID_COL_DELTA][(x-GRID_NUMBER_START_X)/GRID_LINE_DELTA] == '.')
			{
				/*add inputted number to grid*/
				wprintw(grid, "%c", key);
			}
		}
		wmove(grid, y,x);
		refresh();
		wrefresh(grid);
		wrefresh(infobox);
	}

	endwin();
	return EXIT_SUCCESS;
}

/* void pointTest(char** board) {
 *   board[3][2]<
 *}
 */
    

int main(){
    int row,col,i, j;

	init_board(EXAMPLE_STREAM);

    for (i = 0; i<9; i++)
    {
        for (j = 0; j<9; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }


    for (col = 0; col < 9; col++) {
        for (row = 0; row < 9; row++) {
            if (board[row][col] == '.') {
                possibilities[row][col] = vector_init(9);
                // if (row == 8 && col == 8)
                for (i = 0; i < 9; i++) {

                    vector_add_element(possibilities[row][col], (void*) (unsigned long) chars[i]);
                    // possibilities[row][col]->data[i] = (void*)(unsigned long) chars[i];

                }
            }
        }
    }
    printf("\n");

    // printf("%p \n", possibilities[8][8]);
    // fflush(stdout);
    // printf("%d \n", possibilities[8][8]->count);
    
    vector_print_char(possibilities[8][8]);


    getPossibilities(board, possibilities);

    vector_print_char(possibilities[8][8]);

    // getPossibilities(board, );
    point* pos = malloc(sizeof(point));

    // nextCellToFill(board, pos);
    // solve(board);

    return EXIT_SUCCESS;
}
