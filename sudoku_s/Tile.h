#pragma once


#include <vector>
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

	int getRow() { return row; }

	int getColumn() { return column; }
	vector<int> getPossibleValues() { return possibleValues; };

private:
	int actualValue;
	vector<int> possibleValues;
	int row;
	int column;
};

