#pragma once

#include <string>
#include "Board.h"

#define PUZZLE_SIZE 9
/*
PuzzleIO Handles puzzle input and output

*/
class PuzzleIO
{
public:
	PuzzleIO();
	~PuzzleIO();

	void readFromFile(string fileName);
private:
	bool fileExists(string fileName);
	void generateBoardArray(string line, int puzzle[PUZZLE_SIZE][PUZZLE_SIZE]);

};



