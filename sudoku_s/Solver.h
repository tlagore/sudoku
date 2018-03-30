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
	bool checkRowUnion(int row, int column);
	bool checkColumnUnion(int row, int column);
	bool checkUnsolvedCancel(int currRow, int currColumn);
	void cancelRowSkipSameBox(int possibleValue, int currRow, int currColumn);
	void cancelColumnSkipSameBox(int possibleValue, int currRow, int currColumn);
	bool isInBoxRow(int boxCol, int currCol);
	bool isInBoxCol(int boxRow, int boxCol);
	int checkForValueInBox(int currPossible, int currRow, int currColumn);
	bool checkForValueMissing(unordered_set<int> possibleUnionValues, Tile tile);
	void printPossibleValues();
	bool isOpenTile(int value);
	bool checkBoxLineReduction(int row, int column);
};

