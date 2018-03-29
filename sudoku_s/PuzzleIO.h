#pragma once

#include <string>
#include "Board.h"

/*
PuzzleIO Handles puzzle input and output

*/
class PuzzleIO
{
public:
	PuzzleIO();
	~PuzzleIO();

	vector<Board*> readFromFile(string fileName);
private:
	bool fileExists(string fileName);
	void generateBoardArray(string line, int puzzle[BOARD_SIZE][BOARD_SIZE]);

};



