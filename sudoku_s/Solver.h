#pragma once

#include "Board.h"
#include <deque>
class Solver
{
public:

	Solver();
	Solver(Board *board);
	
	bool solve();
	
	Board getBoard();

	void reset(Board *board);

	int getNumberSolved();
	void printPossibleValues();
	~Solver();

private:

	deque<Tile> singleValueTiles;
	Board *board;
	int numSolved = 0;
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
	int checkForValueInBox(int currPossible, int currRow, int currColumn);
	bool isInPossibleValues(unordered_set<int> possibleValues, int possible);
	bool checkForValueMissing(unordered_set<int> possibleUnionValues, Tile tile);
	bool isOpenTile(int value);
	bool checkBoxLineReduction(int row, int column);
};

