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
	bool isSolved = false;
	
	Solver solver(boards[0]);
	isSolved = solver.solve();
	int failedBoards = 0;
	vector<int> boardsFailed;
	vector<int> boardsPassed;
	for (int boardNum = 1; boardNum < boards.size(); boardNum++)
	{
		if (isSolved)
		{
			boardsPassed.push_back(boardNum);
		}
		else
		{
			boardsFailed.push_back(boardNum);
		}
		solver.reset(boards[boardNum]);
		isSolved = solver.solve();
	}

	printf("Number of Boards failed: %d\n", boardsFailed.size());
	printf("Boards that Failed: \n");
	for (int numBoardsFailed = 0; numBoardsFailed < boardsFailed.size(); numBoardsFailed++)
	{
	
		printf("Board number: %d \n", boardsFailed[numBoardsFailed]);
	}
	/*Board brd = solver.getBoard();
	brd.printBoard();*/

	//boards[7]->printBoard();
	//solver.reset(boards[7]);
	//solver.solve();
	//solver.getBoard().printBoard();

	printf("Exiting..\n");


	getchar();
    return 0;
}
