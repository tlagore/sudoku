 #include "stdafx.h"
#include "Board.h"


Board::Board()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			board[row][column] = new Tile(INTIAL_VALUE, row, column);
		}
	}
}

Board::~Board()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			delete board[row][column];
		}
	}
}

Tile Board::getTile(int row, int column)
{
	return *board[row][column];
}

void Board::setTileActualValue(int value, int row, int column)
{
	board[row][column]->setActualValue(value);
}

bool Board::isValidState()
{

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			if (!isValidRow(*board[row][column]))
				return false;

			if (!isValidColumn(*board[row][column]))
				return false;
				
			if (!isValidColumn(*board[row][column]))
				return false;
		}
	}
	//TODO: IMPLEMENT LOGIC
	
}

bool Board::isValidRow(Tile tile)
{
	return false;
}

bool Board::isValidColumn(Tile tile)
{
	return false;
}

bool Board::isValidBox(Tile tile)
{
	return false;
}

