/* to_sudoku.c
   written by Sayre Christenson, 2013-08-22

   makes a matrix of sudoku solutions

   [row][column]
*/

#include"sudoku.h"

void to_sudoku(int matrix_solution[9][9], struct in_node *head, int *cover_solution)
{

	// read in initial problem
	while (head != NULL) {

		matrix_solution[head->row - 1][head->col - 1] = head->val;
		head = head->next;
	}
	
	// read in cover information
	int i = 0;

	while (cover_solution[i] != 0) {

		insert(matrix_solution, cover_solution[i++]);
	}
}


/* converts a row from the cover solution to an entry in the solution */
void insert(int matrix_solution[9][9], int element)
{
	element--;					/* otherwise division doesn't work */

	int row = element/81;  		/* starting at 0 */
	int col = (element % 81)/9; /* starting at 0 */
	int val = (element % 9);

	printf("%d, %d, %d\n", row, col, val);

	matrix_solution[row][col] = ++val;
}
