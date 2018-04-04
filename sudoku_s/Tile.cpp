#include "stdafx.h"
#include "Tile.h"

//Default Constructors
Tile::Tile() : actualValue{ -1 }, row{ -1 }, column{ -1 } {}

//Constructor sets value, row and column
Tile::Tile(int value, int row, int column) : actualValue{ value }, row{ row }, column{ column } {}

//Copy constructor
Tile::Tile(const Tile &tile) : actualValue{ tile.actualValue }, row{ tile.row }, column{ tile.column } {
	for (auto val : tile.candidateValues)
		this->candidateValues.insert(val);
}

//Override assignment operator
Tile & Tile::operator=(Tile otherTile)
{
	this->actualValue = otherTile.actualValue;
	this->row = otherTile.row;
	this->column = otherTile.column;
	for (auto value : otherTile.candidateValues)
		this->candidateValues.insert(value);
	
	return *this;
}

Tile::~Tile()
{

}
/*
Add a candidate value to the candidateValues.

Doesn't need to check for duplicates because candidateValues is an unordered_set and will not insert duplicates
*/
void Tile::addCandidateValue(int value)
{
	if(value >= MIN_SUDOKU_VALUE && value <= MAX_SUDOKU_VALUE)
		this->candidateValues.insert(value);
}


/*
	attempts to remove a candidate value from the tiles candidateValue list. 
	If the value is present and is removed, true is returned, else false
*/
bool Tile::removeCandidateValue(int value)
{
	if (this->candidateValues.find(value) != this->candidateValues.end())
	{
		this->candidateValues.erase(value);
		return true;
	}

	return false;
}

/*
clear all candidate values from the Tile
*/
void Tile::clearCandidateValues()
{
	this->candidateValues.clear();
}

int Tile::getColumn() 
{ 
	return column; 
}

int Tile::getRow()
{
	 return row;
}

unordered_set<int> Tile::getCandidateValues()
{
	return candidateValues;
}


int Tile::getActualValue()
{ 
	return actualValue;
}

/*
	setActualValue sets the actual value of the tile to the specified value and clears it's candidate list
*/
void Tile::setActualValue(int value) 
{
	if (value >= MIN_SUDOKU_VALUE && value <= MAX_SUDOKU_VALUE)
	{
		this->actualValue = value;
		this->candidateValues.clear();
	}
}

/*
print all candidate values for the tile
*/
void Tile::printCandidateValues()
{
	for (auto candidate : candidateValues)
	{
		printf("%d ", candidate);
	}
}
