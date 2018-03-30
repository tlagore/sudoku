// sudoku_s.cpp : Defines the entry point for the console application.
//
 #include "stdafx.h"

#include <iostream>
#include <unordered_set>

#include "Board.h"
#include "PuzzleIO.h"
#include "Solver.h"

int main()
{
	printf("Entering\n");

	PuzzleIO pio;
	std::vector<Board*> boards = pio.readFromFile("..\\sudoku.txt");

	for (std::vector<Board*>::iterator it = boards.begin(); it != boards.end(); ++it) {
		printf("board pointer: %p\n ", *it);
	}
	boards[0]->printBoard();

	Solver solver(boards[0]);
	solver.solve();

	Board brd = solver.getBoard();
	brd.printBoard();

	boards[5]->printBoard();
	solver.reset(boards[5]);
	solver.solve();
	solver.getBoard().printBoard();

	printf("Exiting..\n");


	getchar();
    return 0;
}
