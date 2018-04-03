#include "stdafx.h"
#include "Tile.h"

//Default Constructors
Tile::Tile() : actualValue{ -1 }, row{ -1 }, column{ -1 } {}

//Constructor sets value, row and column
Tile::Tile(int value, int row, int column) : actualValue{ value }, row{ row }, column{ column } {}

//Copy constructor
Tile::Tile(const Tile &tile) : actualValue{ tile.actualValue }, row{ tile.row }, column{ tile.column } {
	for (auto val : tile.possibleValues)
		this->possibleValues.insert(val);
}

//Override assignment operator
Tile & Tile::operator=(Tile otherTile)
{
	this->actualValue = otherTile.actualValue;
	this->row = otherTile.row;
	this->column = otherTile.column;
	for (auto value : otherTile.possibleValues)
		this->possibleValues.insert(value);
	
	return *this;
}

Tile::~Tile()
{

}

void Tile::addPossibleValue(int value)
{
	this->possibleValues.insert(value);
}


/*
	attempts to remove a possible value from the tiles possibleValue list. If the value is present and is removed, true is returned, else false
*/
bool Tile::removePossibleValue(int value)
{
	if (this->possibleValues.find(value) != this->possibleValues.end())
	{
		this->possibleValues.erase(value);
		return true;
	}

	return false;
}

void Tile::clearPossibleValues()
{
	this->possibleValues.clear();
}

int Tile::getColumn() 
{ 
	return column; 
}

int Tile::getRow()
{
	 return row;
}


unordered_set<int> Tile::getPossibleValues()
{
	return possibleValues;
}


int Tile::getActualValue()
{ 
	return actualValue;
}

void Tile::setActualValue(int value) 
{
	if (value >= 1 && value <= 9)
	{
		this->actualValue = value;
		this->possibleValues.clear();
	}
}

void Tile::printPossibleValues()
{
	for (auto possible : possibleValues)
	{
		printf("%d ", possible);
	}
}
