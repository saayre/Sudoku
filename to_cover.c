/*
  to_cover.c
  written by Sayre Christenson, 2013-07-14

  converts a sudoku matrix to an exact cover problem with an initial line

  reads the linked list and marks a 1 for the appropriate cover matrix
  next, attaches full 
*/

#include"sudoku.h"

/*
  main conversion function
  returns a vector of pointers to the column heads of a linked grid

  00. Set up column headers
  10. Assemble full cover matrix
  20. Remove initial rows
*/

// column_pointers only for speeding up the finding of columns
struct col_header *column_pointers[4*9*9];

int loop_count = 0;

struct col_header * to_cover(struct in_node *list)
{
	/* 00. Set up column headers */
	set_up_headers();

	/* 10. Assemble full cover matrix */
	populate_sparse_matrix();

	/* 20. Remove initial rows */
	return eliminate_conflicts(list);
}



/* makes a row of column headers */
void set_up_headers(void)
{
	// column_pointers is global
	int i;
	struct col_header *node;
	struct col_header *prev_node = init_col_header();
	column_pointers[0] = prev_node;

	for (i = 1; i < 4*9*9 ; i++) {

		node = init_col_header();
		column_pointers[i] = node;
				
		prev_node->right = node;
		node->left = prev_node;

		prev_node = node;
	}

	column_pointers[0]->left = column_pointers[4*9*9 - 1];
	column_pointers[0]->left->right = column_pointers[0];
}

/* generates sparse matrix without initial conditions */
void populate_sparse_matrix()
{
	/* indices for element location */
	int matrix_row, position_index, row_index, col_index, block_index;

	/* every combination of val,row,col */
	int val, row, col;
	
	for (row = 1 ; row < 10 ; row++) {
		for (col = 1 ; col < 10 ; col++) {
			for (val = 1 ; val < 10 ; val++) {

				matrix_row = mk_matrix_row(val, row, col);
				position_index = mk_position_index(row, col);
				row_index = mk_row_index(val, row);
				col_index = mk_col_index(val, col);
				block_index = mk_block_index(val, row, col);

				struct cover_cell *position_cell = add_element(matrix_row, position_index);
				struct cover_cell *row_cell = add_element(matrix_row, row_index);
				struct cover_cell *col_cell = add_element(matrix_row, col_index);
				struct cover_cell *block_cell = add_element(matrix_row, block_index);

				connect_row(position_cell, row_cell, col_cell, block_cell);
			}
		}
	}
}

/* removes conflicting rows & cols from pure matrix */
struct col_header *eliminate_conflicts(struct in_node *list)
{
	int position_index, row_index, col_index, block_index;  // each call removes entire row

	while (list != NULL) {

		position_index = mk_position_index(list->row, list->col);
		row_index = mk_row_index(list->val, list->row);
		col_index = mk_col_index(list->val, list->col);
		block_index = mk_block_index(list->val, list->row, list->col);

		remove_column(column_pointers[position_index]);
		remove_column(column_pointers[row_index]);
		remove_column(column_pointers[col_index]);
		remove_column(column_pointers[block_index]);

		list = list->next;
	}

	// frees empty column headers, finds first non-empty header
	struct col_header *not_empty_header = NULL;
	int i;
	for (i = 4*9*9 - 1 ; i >= 0 ; i--) {

		if (column_pointers[i]->n_elements == 0) {

			free(column_pointers[i]);
		}
		else {

			not_empty_header = column_pointers[i];
		}
	}

	// case of full puzzle
	if (not_empty_header == NULL) {

		not_empty_header = init_col_header();
		not_empty_header->right = not_empty_header;
	}
	
	return not_empty_header;
}
				   
/* initializes a column header */
struct col_header * init_col_header(void)
{
	struct col_header *node = malloc(sizeof(struct col_header));
	node->down = NULL;
	node->up = NULL;
	node->right = NULL;
	node->left = NULL;
	node->n_elements = 0;

	return node;
}

/* finds the row of this element
   starts at 0
*/
int mk_matrix_row(int val, int row, int col)
{
	int index = val;
	index += (col - 1) * 9;
	index += (row - 1) * 9 * 9;
	return index;
}

/*
  9 macro-columns for each row
  9 sub-columns for each column
*/
int mk_position_index(int row, int col)
{
	int index = (row - 1) * 9;
	index += col;
	index -= 1;
	return index;
}

/* offset
   9 macro-columns for each row
   9 sub-columns for each value
*/
int mk_row_index(int val, int row)
{
	int index = 9 * 9;
	index += (row - 1) * 9;
	index += val;
	index -= 1;
	return index;
}

/* second offset
   9 macro-columsn for each column
   9 sub-columns for each value
*/
int mk_col_index(int val, int col)
{
	int index = 2 * 9 * 9;
	index += (col - 1) * 9;
	index += val;
	index -= 1;
	return index;
}

/* third offset
   3 macro-columns for each column division
   3 mid-columns for each row division
   9 sub-columns for each value
*/
int mk_block_index(int val, int row, int col)
{
	int index = 3 * 9 * 9;
	index += ((col - 1) / 3) * 27;
	index += ((row - 1)/ 3) * 9;
	index += val;
	index -= 1;
	return index;
}

/* attaches an element to the cover_matrix */
struct cover_cell * add_element(int row, int index)
{
	struct col_header *header = column_pointers[index];
	struct cover_cell *new_element = init_cover_cell(row, header);
	struct cover_cell *first_cell = header->down;
	struct cover_cell *last_cell = header->up;

	if (header->n_elements == 0) {

		header->down = new_element;
		header->up = new_element;
	}
	else {

		// most or least row
		if (row < first_cell->row || row > last_cell->row) {

			new_element->down = first_cell;
			new_element->up = last_cell;

			first_cell->up = new_element;
			last_cell->down = new_element;

			if (row < first_cell->row) {
				header->down = new_element;
			}
			else {
				header->up = new_element;
			}
		}
		else {

			struct cover_cell *cell = first_cell;

			// undershoot cell location
			while (row < cell->row) cell = cell->down;

			new_element->up = cell;
			new_element->down = cell->down;

			cell->down->up = new_element;
			cell->down = new_element;
		}
	}

	header->n_elements += 1;
	return new_element;
}

/* interconnects each cell in a row */
void connect_row(struct cover_cell *position_cell,
				 struct cover_cell *row_cell,
				 struct cover_cell *col_cell,
				 struct cover_cell *block_cell)
{
	position_cell->left = block_cell;
	position_cell->right = row_cell;

	row_cell->left = position_cell;
	row_cell->right = col_cell;

	col_cell->left = row_cell;
	col_cell->right = block_cell;
		
	block_cell->left = col_cell;
	block_cell->right = position_cell;
}

/* removes a column and removes and frees its elements */
void remove_column(struct col_header *header)
{

	// shortcut: previously removed header
	if (header->n_elements == 0) return;
	
	// disconnect header
	unlink_header(header);

	// disconnect cells
	struct cover_cell *cell = header->down;

	// prevents already cleared headers from seg faulting
	while (header->n_elements != 0) {
		
		remove_row(cell);		/* leaves cell intact */

		// free cell
		if (cell != header->up) {

			cell = cell->down;
			free(cell->up);
			(header->n_elements)--;
		}
		else {

			free(cell);
			(header->n_elements)--;
		}
	}

	// column header freed back in eliminate_conflicts
}

/* initializes a cover_matrix element */
struct cover_cell * init_cover_cell(int row, struct col_header *header)
{
	struct cover_cell *node = malloc(sizeof(struct cover_cell));
	node->down = NULL;
	node->up  = NULL;
	node->right = NULL;
	node->left = NULL;
	node->col = header;
	node->row = row;

	return node;
}

/* eliminates a row from the grid, leaving cell still linked */
void remove_row(struct cover_cell *cell)
{
	cell->left = NULL;  // sets an endpoint
	cell = cell->right;

	while (cell->left != NULL) {

		/* unlink from cells */
		cell->up->down = cell->down;
		cell->down->up = cell->up;

		/* unlink from header */
		if (cell->col->down == cell) cell->col->down = cell->down;
		if (cell->col->up == cell) cell->col->up = cell->up;

		/* check for removing last element of column */
		(cell->col->n_elements)--;
		if (cell->col->n_elements == 0) unlink_header(cell->col);
			
		cell = cell->right;
		free(cell->left);
	}
}

/* testing for correctly set up blank matrix */
void testprinter(struct col_header *first)
{

	struct col_header *iter = first;
	int count = 0;
	do {
		iter = iter->right;
		count++;
		if (count > 1000) {
			printf("right failure\n");
			exit(EXIT_FAILURE);
		}
	} while (iter != first);
	printf("firstcount = %d\n", count);

	iter = first;
	count = 0;
	do {
		iter = iter->left;
		count++;
		if (count > 1000) {
			printf("left failure\n");
			exit(EXIT_FAILURE);
		}
	} while (iter != first);
	printf("secondcount = %d\n", count);

	/* testing columns */
	struct cover_cell *iter_cell;
	iter = first;
	do {
		iter_cell = iter->down;

		if (iter->up != iter_cell->up) {
			printf("up failure\n");
			exit(EXIT_FAILURE);
		}

		if (iter->up->down != iter_cell) {
			printf("down failure\n");
			exit(EXIT_FAILURE);
		}

		count = 0;
		do {
			if (iter_cell->col != iter) {
				printf("cell points to wrong column\n");
				exit(EXIT_FAILURE);
			}

			/* testing rows */
			struct cover_cell *rowcheck = iter_cell;
			int ji = 0;
			do {
				ji++;
				rowcheck = rowcheck->right;

				if (ji > 4) {
					printf("row right fail\n");
					exit(EXIT_FAILURE);
				}
				
			} while (rowcheck != iter_cell);

			rowcheck = iter_cell;
			ji = 0;
			do {
				ji++;
				rowcheck = rowcheck->left;

				if (ji > 4) {
					printf("row left fail\n");
					exit(EXIT_FAILURE);
				}
				
			} while (rowcheck != iter_cell);

			count++;
			iter_cell = iter_cell->down;

			if (count > 9) {
				printf("vert loop failure\n");
				exit(EXIT_FAILURE);
			}
			
		} while (iter_cell != iter->down);

		iter = iter->right;
	} while (iter != first);

	/* testing rows */
	

}
