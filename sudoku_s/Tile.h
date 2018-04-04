#pragma once


#include <unordered_set>
#define MAX_SUDOKU_VALUE 9
#define MIN_SUDOKU_VALUE 1

using namespace std;

/*
	Tile maintains the information of a single tile for the sudoku board.

	@author Tyrone Lagore & Johnny Simmonds
	@version 4/4/2018
*/
class Tile
{
public:
	Tile();
	Tile(int value, int row, int column);
	Tile(const Tile &tile);
	Tile& operator=(Tile otherTile);

	~Tile();


	void addCandidateValue(int value);
	bool removeCandidateValue(int value);
	void clearCandidateValues();

	int getColumn();

	int getRow();

	
	unordered_set<int> getCandidateValues();

	int getActualValue();

	void setActualValue(int value);
	void printCandidateValues();

private:
	int actualValue;
	unordered_set<int> candidateValues;
	int row;
	int column;
};

