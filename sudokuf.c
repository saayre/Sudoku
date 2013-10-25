/*
 sudokuf.c
 written by Sayre Christenson, 2013-07-06

 function files for sudoku solver
*/

#include"sudoku.h"


int ** read_sudoku(char * infile)
{
	FILE * file = openr(infile);

	int element;
	while ((element = getchar(file)) != EOF) {
		
