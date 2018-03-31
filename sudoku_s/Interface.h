#pragma once
#include <iostream>
#include <unordered_set>
#include "Board.h"
#include "PuzzleIO.h"
#include "Solver.h"
class Interface
{
public:

	Interface();
	Interface(string fromFile);
	void printDifferentBoardPtrs();
	bool mainMenu();
	void solveSpecificBoardMenu();
	void solveSpecificBoard(int boardNum);
	void solveAllBoards();
	~Interface();

private:
	PuzzleIO pio;
	std::vector<Board*> boards;
	Solver solver;
	bool isOpen = true;
};

