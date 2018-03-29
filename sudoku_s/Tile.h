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

	int getActualValue() { return actualValue; }
	void setActualValue(int value) {
		if (value >= 1 && value <= 9)
			this->actualValue = value;
	}

	void addPossibleValue(int value);
	void removePossibleValue(int value);
	void clearPossibleValues();

	int getRow() { return row; }

	int getColumn() { return column; }
	unordered_set<int> getPossibleValues() { 	
		return possibleValues; 
	};

private:
	int actualValue;
	unordered_set<int> possibleValues;
	int row;
	int column;
};

