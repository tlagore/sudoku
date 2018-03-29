// sudoku_s.cpp : Defines the entry point for the console application.
//
 #include "stdafx.h"

#include "PuzzleIO.h"
#include <iostream>

int main()
{
	int board[9][9] = { 
		//first block
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		
		//second block
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },

		//third block
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	};
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			printf("%d, ", board[i][j]);
		}
		printf("\n");
	}

	printf("Entering\n");

	PuzzleIO pio;
	std::vector<Board*> boards = pio.readFromFile("..\\sudoku.txt");

	for (std::vector<Board*>::iterator it = boards.begin(); it != boards.end(); ++it) {
		printf("board pointer: %p\n ", *it);
	}

	printf("Exiting..\n");

	getchar();
    return 0;
}

