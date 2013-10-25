/*
 display.c
 written by Sayre Christenson, 2013-07-25

 contains functions for displaying sudoku grids

 [row][column]
*/

#include"sudoku.h"

/* for final display */
void display(int matrix_solution[9][9])
{
	int i, j;
	for (i = 0 ; i < 9 ; i++) {
		if (i % 3 == 0) printf("\n");

		for (j = 0 ; j < 9 ; j++) {
			if (j % 3 == 0) printf(" ");

			printf("%d", matrix_solution[i][j]);
		}
		printf("\n");
	}
}
