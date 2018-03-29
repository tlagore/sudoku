#pragma once

#include "Board.h"

class Solver
{
public:
	Solver();
	Solver(Board *board);

	void solve();

	Board getBoard();

	~Solver();

private:
	Board *board;
	void generatePossibleValues();
	void generateTileValues(int row, int column);
	void removeRowValues(int row, int column, unordered_set<int>* possibleValues);
	void removeColValues(int row, int column, unordered_set<int>* possibleValues);
	void removeBoxValues(int row, int column, unordered_set<int>* possibleValues);
};

