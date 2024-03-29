#pragma once

#include "Board.h"
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <vector> 

/*
	Solver maintains and solves a given sudoku board using sudoku cancellation deduction techniques


	@author Tyrone Lagore & Johnny Simmonds
	@version 4/4/2018
*/
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
	void removeRegionValues(int row, int column, unordered_set<int>* candidateValues);
	void solveTile(Tile tile);
	void cancelRegion(int value, int row, int column);
	void cancelRow(int value, int row, int column);
	void cancelColumn(int value, int row, int column);
	bool removeValue(Tile tile, int value);
	bool performSolve();
	bool checkRowUnion(int row, int column);
	bool checkColumnUnion(int row, int column);

	///Tyrone attempt at unsolved cancel functions
	///Note: These functions call cancelRowSkipSameRegion and cancelColumnSkipSameRegion
	bool checkPointedTuple(int curRow, int curCol);
	bool checkSetsContainsUnique(const unordered_map<int, unordered_set<int>> *pSets, int curRow, int curCol, string type);
	bool setContains(unordered_set<int>, int value);
	///

	bool checkUnsolvedCancel(int currRow, int currColumn);
	bool cancelRowSkipSameRegion(int candidateValue, int currRow, int currColumn);
	bool cancelColumnSkipSameRegion(int candidateValue, int currRow, int currColumn);
	int checkForValueInRegion(int currCandidate, int currRow, int currColumn);
	//bool isInCandidateValues(unordered_set<int> candidateValues, int candidate);
	bool checkForValueMissing(unordered_set<int> candidateUnionValues, Tile tile);
	bool isOpenTile(int value);
	bool checkRegionLineReduction(int row, int column);
};

