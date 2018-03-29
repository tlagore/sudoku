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

void Tile::removePossibleValue(int value)
{
	this->possibleValues.erase(value);
}

void Tile::clearPossibleValues()
{
	this->possibleValues.clear();
}

