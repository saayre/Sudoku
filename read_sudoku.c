/*
  read_sudoku.c
  written by Sayre Christenson, 2013-07-14

  reads a sudoku style matrix, formatted like

  00015343-       (any character not between 1
  1235 1960        and 9 is read as empty)
  13X144560

  into an exact cover problem
*/

#include"sudoku.h"

/* main reading function

   first_line_toggle is used for finding puzzle size

   10. Loop through characters
   20. Check for newline
   30. Check for reasonable value
   40. Add new node to list
   
*/
struct in_node * read_sudoku(char * infile)
{
	int row = 1;
	int col = 0;
	int val;

	struct in_node *head = NULL;
	struct in_node *tail = NULL;

	/* 10. Loop through characters */
	FILE *file = fopen(infile, "r");
	char c;
	while ((c = fgetc(file)) != EOF) {

		/* 20. Check for newline */
		if (c == '\n') {
			row++;
			col = 0;
			continue;
		}
		
		else {
			val = c - '0';
			col++;
		}

		/* 30. Check for reasonable value */
		if (val < 1 || val > 9) continue;

		/* 40. Add new node to list */
		struct in_node *new_node = mk_in_node(val, row, col);
		if (head == NULL) {
			
			head = new_node;
			tail = new_node;
		}
		else {
			
			tail->next = new_node;
			tail = new_node;
		}
	}

	return head;
}


/* initializes each node */
struct in_node * mk_in_node(int val, int row, int col)
{
	struct in_node *new_node = malloc(sizeof(struct in_node));

	new_node->val = val;
	new_node->row = row;
	new_node->col = col;
	new_node->next = NULL;

	return new_node;
}
