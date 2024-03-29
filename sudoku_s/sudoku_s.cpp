// sudoku_s.cpp : Defines the entry point for the console application.
//
 #include "stdafx.h"


#include "Interface.h"
#include "PuzzleMaker.h"

int main()
{
	printf("Entering\n\n\n");

	Board *board;

	
	PuzzleMaker pm(500);

	printf("Generating board...\n");
	board = pm.generateBoard(PuzzleMaker::MEDIUM);
	printf("Generated board:\n");
	board->printBoard();
	printf("Deleting board.\n");
	Solver solver(board);
	solver.solve();
	solver.getBoard()->printBoard();
	delete board;
	/*

	Interface menu;
	while (menu.mainMenu());
	*/
	printf("Exit? (hit any key)..\n");
	getchar();
    return 0;
}
