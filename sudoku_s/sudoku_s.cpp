// sudoku_s.cpp : Defines the entry point for the console application.
//
 #include "stdafx.h"


#include "Interface.h"
#include "PuzzleMaker.h"

int main()
{
	printf("Entering\n\n\n");


	PuzzleMaker pm;

	Board *board = pm.generateBoard(PuzzleMaker::EASY);
	delete board;

	//Interface menu;
	//while (menu.mainMenu());

	printf("Exit? (hit any key)..\n");
	char clearExtra[1];
	cin.getline(clearExtra, 1);
	getchar();
    return 0;
}
