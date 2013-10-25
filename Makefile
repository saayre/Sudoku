CC=gcc
CFLAGS=-Wall -g

all: read_sudoku.o dancing_links.o to_sudoku.o display.o sudoku

read_sudoku.o: read_sudoku.c
	$(CC) $(CFLAGS) -c read_sudoku.c

to_cover.o: to_cover.c
	$(CC) $(CFLAGS) -c to_cover.c

dancing_links.o: dancing_links.c
	$(CC) $(CFLAGS) -c dancing_links.c

to_sudoku.o: to_sudoku.c
	$(CC) $(CFLAGS) -c to_sudoku.c

display.o: display.c 
	$(CC) $(CFLAGS) -c display.c

testprinting.o: testprinting.c
	$(CC) $(CFLAGS) -c testprinting.c

sudoku: read_sudoku.o to_sudoku.o dancing_links.o to_sudoku.o display.o sudoku.c
	$(CC) $(CFLAGS) sudoku.c read_sudoku.o to_sudoku.o dancing_links.o to_sudoku.o display.o -o sudoku

testmain: testmain.c read_sudoku.o testprinting.o to_cover.o dancing_links.o to_sudoku.o display.o
	$(CC) $(CFLAGS) testmain.c read_sudoku.o testprinting.o to_cover.o dancing_links.o to_sudoku.o display.o -o testmain

new_read: new_read.c
	$(CC) $(CFLAGS) -c new_read.c

test2: testmain2.c new_read.o testprinting.o
	$(CC) $(CFLAGS) testmain2.c new_read.o testprinting.o -o test2

clean:
	rm sudoku testmain read_sudoku.o to_cover.o dancing_links.o to_sudoku.o display.o testprinting.o new_read.o test2
