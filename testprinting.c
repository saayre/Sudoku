/*
  testprinting.c
  written by Sayre Christenson, 2013-08-01

  for testing i/o of each part
*/

#include"sudoku.h"

/* for testing read_sudoku */
void test_print(struct in_node *list)
{
	int last_col = 0;
	int i;
	while (list != NULL) {

		if (list->col < last_col) {
			printf("\n");
			last_col = 0;
		}

		for (i = 1; i < list->col - last_col; i++) {
			printf("  ");
		}
		
		printf(" %d", list->val);

		last_col = list->col;
		list = list->next;
	}
	
	printf("\n");
}

/* for testing index creating functions in to_cover */
void test_print2(struct in_node *list)
{
	int val, row, col;

	for (col = 1 ; col < 10 ; col += 3) {
		for (row = 1 ; row < 10 ; row += 3) {
			printf("row, col\t%d, %d\n", row, col);
			for (val = 1 ; val < 10 ; val++) {
				// printf("%d\n", mk_block_index(val, row, col));
			}
			printf("\n");
		}
		printf("\n");
	}
}


/* for testing up to original_populate in to_cover */
void test_print3(struct col_header *control_row)
{
	int i = 0;
	while (i < 4*9*9) {

		if (control_row->n_elements > 0) printf("%d\t", i);
		control_row = control_row->right;
		i++;
	}
}


/* for testing past original_populate in to_cover */
void test_print4(struct col_header *control_row)
{
	int i;
	int j;

	for (i = 0 ; i < 9 ; i++) {
		for (j = 0 ; j < 9 ; j++) {

			if (control_row->n_elements == 1) {
				int num = control_row->down->row;
				num -= j * 9;
				num -= i * 9 * 9;

				printf(" %d", num);

				control_row = control_row->right;
			}
		}
		printf("\n");
	}
}


/* for testing everything before dancing_links algorithm */
void test_print5(struct col_header *problem)
{
	struct col_header *first = problem;
	int count = 0;

	do {
		printf("n_elements = %d", problem->n_elements);
		
		struct cover_cell *cell = problem->down;
		printf("\n\trows = ");
		do {

			printf("%d, ", cell->row);
			cell = cell->down;
		} while (cell != problem->down);
		getchar();
		
		count++;
		problem = problem->right;
	} while (first != problem);

	printf("\n%d\n", count);
}


/* testing through dancing_links */
void test_print6(int *solution)
{
	int i = 0;
	while (solution[i] != 0) printf("%d ", solution[i++]);
}


/* for testing to_cover again */
void test_print7(struct col_header *problem)
{
	printf("Checking column headers sideways\n");

	/* checking linked towards right */
	struct col_header *check = problem;
	int iter_count = 0;
	do {
		check = check->right;
		iter_count++;
		if (iter_count > 10) {
			printf("shit\n");
			exit(EXIT_FAILURE);
		}
	} while (check != problem);
	printf("Correctly linked to the right\n");

	/* checking linked towards left */
	check = problem;
	iter_count = 0;
	do {
		check = check->left;
		iter_count++;
		if (iter_count > 10) {
			printf("shit\n");
			exit(EXIT_FAILURE);
		}
	} while (check != problem);
	printf("Correctly linked to the left\n");

	/* checking if each row links through itself */
	/* checking if each column links through itself, with the headers correct */
}


/* for testing new_read */
void test_print8(int *matrix)
{
	int i, j;
	for (i = 0 ; i < 9 ; i++) {
		for (j = 0 ; j < 9 ; j++) {

			printf("%d ", matrix[9*i + j]);
		}
		printf("\n");
	}
}


/* prints available rows entering a problem */
void print_rows(struct col_header *problem)
{

	struct col_header *loop = problem;
	do {

		struct cover_cell *cell = loop->down;
		do {

			printf("%d\n", cell->row);
			
			cell = cell->down;
		} while (cell != loop->down);

		printf("\t");
		loop = loop->right;
	} while (loop != problem);
}


/* makes a crash file */
void crash_file(struct col_header *problem)
{
	FILE *out = fopen("crashed", "w");

	// headers
	struct col_header *loop = problem;
	do {

		fprintf(out, "header = %p\n", loop);
		fprintf(out, "right = %p\n", loop->right);
		fprintf(out, "left = %p\n", loop->left);
		fprintf(out, "up = %p\n", loop->up);
		fprintf(out, "down = %p\n", loop->down);
		fprintf(out, "n_elements = %d\n\n", loop->n_elements);

		loop = loop->right;
	} while (loop != problem);

	do {

		struct cover_cell *cell = loop->down;
		do {

			fprintf(out, "cell = %p\n", cell);
			fprintf(out, "up = %p\n", cell->up);
			fprintf(out, "down = %p\n", cell->down);
			fprintf(out, "right = %p\n", cell->right);
			fprintf(out, "left = %p\n", cell->left);
			fprintf(out, "col = %p\n", cell->col);
			fprintf(out, "row = %d\n\n", cell->row);
			
			cell = cell->down;
		} while (cell != loop->down);

		loop = loop->right;
	} while (loop != problem);

	fclose(out);
}


/* prints a crash */
void crash_print(struct col_header *problem)
{
	struct col_header *loop = problem;
	do {

		struct cover_cell *cell = loop->down;
		do {

			getchar();
			printf("cell = %p\n", cell);
			printf("up = %p\n", cell->up);
			printf("down = %p\n", cell->down);
			printf("right = %p\n", cell->right);
			printf("left = %p\n", cell->left);
			printf("col = %p\n", cell->col);
			printf("row = %d\n\n", cell->row);
			
			cell = cell->down;
		} while (cell != loop->down);

		loop = loop->right;
	} while (loop != problem);

}
