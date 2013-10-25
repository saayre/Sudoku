/*
 sudoku.h
 written by Sayre Christenson, 2013-07-06

 header file for sudoku solver
*/

#include<stdio.h>
#include<stdlib.h>

#ifndef SUDOKU_H
#define SUDOKU_H

struct in_node {
	unsigned char val;
	unsigned char row;
	unsigned char col;

	struct in_node *next;
};

struct cover_cell {
	struct cover_cell *up;
	struct cover_cell *down;
	struct cover_cell *left;
	struct cover_cell *right;
	struct col_header *col;

	int row;
};

struct col_header {
	struct cover_cell *down;
	struct cover_cell *up;

	struct col_header *right;
	struct col_header *left;

	int n_elements;
};



/* read_sudoku */
struct in_node *read_sudoku(char *infile);

struct in_node *mk_in_node(int val, int row, int col);

/* to_cover */
struct col_header *to_cover(struct in_node *list);

void set_up_headers(void);
void populate_sparse_matrix(void);
struct col_header *eliminate_conflicts(struct in_node *head);

struct col_header *init_col_header(void);
int mk_matrix_row(int val, int row, int col);
int mk_position_index(int row, int col);
int mk_row_index(int val, int row);
int mk_col_index(int val, int col);
int mk_block_index(int val, int row, int col);
struct cover_cell *add_element(int row, int index);
void connect_row(struct cover_cell *position_cell,
				 struct cover_cell *row_cell,
				 struct cover_cell *col_cell,
				 struct cover_cell *block_cell);
void remove_column(struct col_header *header);

struct cover_cell *init_cover_cell(int row, struct col_header *header);
void remove_row(struct cover_cell *cell);

void testprinter(struct col_header *test);


/* dancing_links */
int *dancing_links(struct col_header *input_column);

struct col_header *shortest_column(struct col_header *input_column);
struct col_header *unlink_header(struct col_header *header);
void unlink_column(struct col_header *col);
struct col_header *cover_series(struct cover_cell *start, int series);
void push_row(int row);
void unlink_element_from_column(struct cover_cell *element);
void relink_element_to_column(struct cover_cell *element);
void relink_header(struct col_header *header);
void pop_row(void);
void print_state(struct col_header *grid);
int column_count(struct col_header *start);


/* to_sudoku */
void to_sudoku(int matrix_solution[9][9], struct in_node *head, int *cover_solution);
void insert(int matrix_solution[9][9], int element);


/* display */
void display(int matrix_solution[9][9]);


/* testing */
void test_print(struct in_node *list);
void test_print2(struct in_node *list);
void test_print3(struct col_header *control_row);
void test_print4(struct col_header *control_row);
void test_print5(struct col_header *problem);
void test_print6(int *solution);
void test_print7(struct col_header *problem);
void test_print8(int *matrix);
void print_rows(struct col_header *problem);
void crash_file(struct col_header *problem);
void crash_print(struct col_header *problem);

void hihi(void);


/* new_read */
void new_read(char *filename, int *matrix);


#endif
