#pragma once


#include <unordered_set>
#define MAX_SUDOKU_VALUE 9

using namespace std;

class Tile
{
public:
	Tile();
	Tile(int value, int row, int column);
	Tile(const Tile &tile);
	Tile& operator=(Tile otherTile);

	~Tile();


	void addPossibleValue(int value);
	void removePossibleValue(int value);
	void clearPossibleValues();

	int getColumn();

	int getRow();

	
	unordered_set<int> getPossibleValues();

	int getActualValue();

	void setActualValue(int value);

private:
	int actualValue;
	unordered_set<int> possibleValues;
	int row;
	int column;
};

