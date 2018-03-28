#pragma once


#include <vector>
#define MAX_SUDOKU_VALUE 9

using namespace std;

class Tile
{
public:
	Tile() : actualValue{ -1 }, row{ -1 }, column{ -1 } {}
	Tile(int value, int row, int column) : actualValue{ value }, row{ row }, column{ column } {}
	Tile(const Tile &tile) : actualValue{ tile.actualValue }, row{ tile.row }, column{ tile.column } {}
	Tile& operator=(Tile otherTile) { 
		otherTile.actualValue = this->actualValue;
		otherTile.row = this->row;
		otherTile.column = this->column;
		for (auto value : this->possibleValues)
			otherTile.possibleValues.push_back(value);
	}

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

