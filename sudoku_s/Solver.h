#pragma once

#include "Board.h"
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <vector> 

class Solver
{
public:

	Solver();
	Solver(Board *board);
	
	bool solve();
	
	Board* getBoard();

	void reset(Board *board);

	int getNumberSolved();
	~Solver();

private:
	Board *board;
	int numSolved = 0;
	void generateCandidateValues();
	void generateTileValues(int row, int column);
	void removeRowValues(int row, int column, unordered_set<int>* candidateValues);
	void removeColValues(int row, int column, unordered_set<int>* candidateValues);
	void removeBoxValues(int row, int column, unordered_set<int>* candidateValues);
	void solveTile(Tile tile);
	void cancelBox(int value, int row, int column);
	void cancelRow(int value, int row, int column);
	void cancelColumn(int value, int row, int column);
	bool removeValue(Tile tile, int value);
	bool performSolve();
	bool checkRowUnion(int row, int column);
	bool checkColumnUnion(int row, int column);

	///Tyrone attempt at unsolved cancel functions
	///Note: These functions call cancelRowSkipSameBox and cancelColumnSkipSameBox
	bool checkPointedTuple(int curRow, int curCol);
	bool checkSetsContainsUnique(const unordered_map<int, unordered_set<int>> *pSets, int curRow, int curCol, string type);
	bool setContains(unordered_set<int>, int value);
	///

	bool checkUnsolvedCancel(int currRow, int currColumn);
	bool cancelRowSkipSameBox(int candidateValue, int currRow, int currColumn);
	bool cancelColumnSkipSameBox(int candidateValue, int currRow, int currColumn);
	int checkForValueInBox(int currCandidate, int currRow, int currColumn);
	bool checkForValueMissing(unordered_set<int> candidateUnionValues, Tile tile);
	bool isOpenTile(int value);
	bool checkBoxLineReduction(int row, int column);
};

