#pragma once

#include "Board.h"
#include <deque>
class Solver
{
public:

	Solver();
	Solver(Board *board);
	
	void solve();
	
	Board getBoard();

	void reset(Board *board);
	
	~Solver();

private:

	deque<Tile> singleValueTiles;
	Board *board;
	void generatePossibleValues();
	void generateTileValues(int row, int column);
	void removeRowValues(int row, int column, unordered_set<int>* possibleValues);
	void removeColValues(int row, int column, unordered_set<int>* possibleValues);
	void removeBoxValues(int row, int column, unordered_set<int>* possibleValues);
	void cancelBox(int value, int row, int column);
	void cancelRow(int value, int row, int column);
	void cancelColumn(int value, int row, int column);
	void removeValue(Tile tile, int value);
	bool performAdvancedSolve();
	bool checkBoxLineReduction(int row, int column);
};

