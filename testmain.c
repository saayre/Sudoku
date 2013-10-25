#include"sudoku.h"

void hihi(void)
{
	printf("hihi\n");
}

int main(void)
{
	
	struct in_node *list = read_sudoku("testfile");

	struct col_header *problem = to_cover(list);

	int *solution = dancing_links(problem);

	int i = 0;
	while (solution[i] != 0) printf("%d, ", solution[i++]);
	printf("\n");

	int matrix_solution[9][9] = {{0}};
	to_sudoku(matrix_solution, list, solution);

	display(matrix_solution);
	
	return 0;
}
