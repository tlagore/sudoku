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
		boards[i] = pm.generateBoard(PuzzleMaker::EASY);
		delete boards[i];
	}

	//Interface menu;
	//while (menu.mainMenu());

	printf("Exit? (hit any key)..\n");
	getchar();
    return 0;
}
