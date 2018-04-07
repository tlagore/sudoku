// sudoku_s.cpp : Defines the entry point for the console application.
//
 #include "stdafx.h"


#include "Interface.h"
#include "PuzzleMaker.h"

int main()
{
	printf("Entering\n\n\n");

	Board *boards[10];

	PuzzleMaker pm(500);

	for (int i = 0; i < 10; i++) {
		printf("Generating board...\n");
		boards[i] = pm.generateBoard(PuzzleMaker::EASY);
		printf("Generated board:\n");
		boards[i]->printBoard();
		printf("Deleting board.\n");
		delete boards[i];
	}

	//Interface menu;
	//while (menu.mainMenu());

	printf("Exit? (hit any key)..\n");
	getchar();
    return 0;
}
