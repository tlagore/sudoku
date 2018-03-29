// sudoku_s.cpp : Defines the entry point for the console application.
//
 #include "stdafx.h"
#include "Board.h"

#include "PuzzleIO.h"
#include <iostream>

int main()
{
	printf("Entering\n");

	PuzzleIO pio;
	std::vector<Board*> boards = pio.readFromFile("..\\sudoku.txt");

	for (std::vector<Board*>::iterator it = boards.begin(); it != boards.end(); ++it) {
		printf("board pointer: %p\n ", *it);
	}
	boards[0]->printBoard();
	printf("Exiting..\n");

	getchar();
    return 0;
}

