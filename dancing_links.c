/*
  dancing_links.c
  written by Sayre Christenson, 2013-07-31

  actually implements Algorithm X to solve the exact cover problem
  returns an array of row numbers that correspond to the solution
*/

#include"sudoku.h"

struct col_header *col_stack[400];
int col_stack_count = 0;

int row_stack[81] = {0};
int row_stack_count = 0;

int *dancing_links(struct col_header *input_column)
{
	struct col_header *chosen_column = shortest_column(input_column);

	// define success
	if (chosen_column->right == chosen_column) return row_stack;

	struct cover_cell *cell = chosen_column->down;
	struct col_header *new_input_column = NULL;

	do {

		/* print_state(chosen_column); */
		/* getchar(); */

		printf("down\n");
		
		push_row(cell->row);
		
		new_input_column = cover_series(cell, 0); /* unlink */

		// keep searching
		if (column_count(new_input_column) >= 4 ||
			column_count(new_input_column) == 1) {

			int *result = dancing_links(new_input_column);

			// propagate success
			if (result != NULL) return result;
		}

		// assume failure
		pop_row();
		new_input_column = cover_series(cell, 1); /* relink */
			
		// next cell
		cell = cell->down;

		printf("up");
		if (cell == chosen_column->down) printf("\t check");
		printf("\n");

	} while (cell != chosen_column->down);

	return NULL;
}		

	
/* returns the shortest, left-most column */
struct col_header *shortest_column(struct col_header *input_column)
{
	// short circuit to success!
	if (input_column->right == input_column) return input_column;
	
	int min_val = input_column->n_elements;
	while (min_val == 0) {

		input_column = input_column->right;
		min_val = input_column->n_elements;
	}
	
	struct col_header *min_col = input_column, *node = input_column->right;
	do {

		if (node->n_elements < min_val) {
			
			min_val = node->n_elements;
			min_col = node;
		}

		node = node->right;
	} while (node != input_column);

	return min_col;
}

/* counts the number of currently linked colums */
int column_count(struct col_header *start)
{
	int count = 1;
	struct col_header *iter = start->right;
	
	while (iter != start) {

		iter = iter->right;
	    count++;
	}

	return count;
}

/* unlinks a column header from the grid */
struct col_header *unlink_header(struct col_header *header)
{
	header->left->right = header->right;
	header->right->left = header->left;

	return header->right;
}

/* unlinks a column's elements from the grid */
void unlink_column(struct col_header *col)
{
	struct cover_cell *cell = col->down;
	do {

		cell->left->right = cell->right;
		cell->right->left = cell->left;

		cell = cell->down;
	} while (cell != col->down);
}

/* adds a row value to the stack */
 void push_row(int row)
{
	row_stack[row_stack_count++] = row;
}

/* unlinks a series of columns and headers for this branch of the algorithm */
/* series = 0 means unlink elements, series = 1 means relink them */
struct col_header *cover_series(struct cover_cell *start, int series)
{

	// iterate along first_row
	struct cover_cell *first_iterator = start;
	struct col_header *new_input_column;
	do {
		/* if (start->row == 390) { */
		/* 	printf("num1\t%d, %p\n", first_iterator->row, first_iterator->col); */
		/* 	getchar(); */
		/* } */

		// iterate down each column
		struct cover_cell *second_iterator = first_iterator;
		do {
			/* if (start->row == 390) { */
			/* 	printf("num2\t%d, %p\n", second_iterator->row, second_iterator->col); */
			/* 	getchar(); */
			/* } */

			// iterate right over each row, unlinking up/down
			struct cover_cell *third_iterator = second_iterator->right;
			while (third_iterator != second_iterator) {
				/* if (start->row == 390) { */
				/* 	printf("num3\t%d, %p\n", third_iterator->row, third_iterator->col); */
				/* 	getchar(); */
				/* } */

				if (third_iterator->row != start->row) {

					if (series == 0) {
					
						unlink_element_from_column(third_iterator);
						// last element from column
						if (third_iterator->col->n_elements == 0) unlink_header(third_iterator->col);
					}
				
					if (series == 1) {

						relink_element_to_column(third_iterator);
						// first element in column
						if (third_iterator->col->n_elements == 1) relink_header(third_iterator->col);
					}
				}

				else {
					// don't unlink initial row elements from column
					// don't repeated unlink the initial row
					if (first_iterator != start) unlink_header(third_iterator->col);
				}

				third_iterator = third_iterator->right;
			}

			second_iterator = second_iterator->down;
		} while (second_iterator != first_iterator);
		
		first_iterator = first_iterator->right;
	} while (first_iterator != start);

	if (series == 0) new_input_column = unlink_header(start->col);
	if (series == 1) relink_header(start->col);

	return (series == 0) ? new_input_column : start->col;
}

/* circumvents a given element within its column */
void unlink_element_from_column(struct cover_cell *element)
{
	element->up->down = element->down;
	element->down->up = element->up;

	// unlink from header if necessary
	if (element->col->down == element) element->col->down = element->down;
	if (element->col->up == element) element->col->up = element->up;

	(element->col->n_elements)--;
}

/* adds a column header to the stack */
void push_col(struct col_header *col)
{
	col_stack[col_stack_count++] = col;
}

/* relinks an element to its column */
void relink_element_to_column(struct cover_cell *element)
{
	element->up->down = element;
	element->down->up = element;

	// relink to header if necessary
	if (element->row < element->col->down->row) element->col->down = element;
	if (element->row > element->col->up->row) element->col->up = element;

	(element->col->n_elements)++;
}

/* relinks a column to the grid */
void relink_header(struct col_header *col)
{
	col->left->right = col;
	col->right->left = col;
}

/* pops a row off the stack */
void pop_row(void)
{
	col_stack[row_stack_count--] = 0;
}

/* lazily prints out the current grid */
void print_state(struct col_header *grid)
{
	
	int col_count = 0;
	struct col_header *test = grid;
	do {

		printf("col #%d\t", col_count);
		struct cover_cell *cell = test->down;
		do {

			printf("%d\t", cell->row);
			cell = cell->down;
		} while (cell != test->down);
		
		test = test->right;
		printf("\n");
		col_count++;
	} while (test != grid);
}
