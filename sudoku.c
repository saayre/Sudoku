/* 
sudoku.c
written by Sayre Christenson, 2013-07-06

implements the dancing links algorithm to solve sudoku
*/

/* 
 * 10. Read in Sudoku puzzle
 * 20. Convert to exact cover problem
 * 30. Solve exact cover problem
 * 40. Transform exact cover problem to Sudoku solution
 * 50. Print solution
 */

#include"sudoku.h"

int main(char * infile)
{
	// 10. Read in Sudoku puzzle
	struct inode *sudoku_problem = read_sudoku(infile);

	// 20. Convert to exact cover problem
	struct cover_carrier cover_problem = to_sudoku(sudoku_problem);

	// 30. Solve exact cover problem
	int ** cover_solution = dancing_links(cover_problem);

	// 40. Transform exactcover problem to Sudoku solution
	int ** sudoku_solution = to_sudoku(cover_solution);

	// 50. Print solution
	display(sudoku_solution);

	return 0;
}
