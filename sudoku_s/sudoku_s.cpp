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

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			unordered_set<int> set = brd.getTile(row,col).getPossibleValues();

			printf("Tile (%d,%d): ", row, col);
			for (auto x : brd.getTile(row, col).getPossibleValues())
				printf("%d ", x);

			printf("\n");
		}
		printf("\n");
	}
	printf("Exiting..\n");


	getchar();
    return 0;
}
