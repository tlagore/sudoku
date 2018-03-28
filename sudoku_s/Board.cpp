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

Board::Board(int inputForNewBoard[BOARD_SIZE][BOARD_SIZE])
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			board[row][column] = new Tile(inputForNewBoard[row][column], row, column);
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
				
			if (!isValidBox(*board[row][column]))
				return false;
		}
	}
	
}

bool Board::isValidRow(Tile tile)
{
	int currRow = tile.getRow();
	
	for (int column = 0; column < BOARD_SIZE; column++)
	{
		Tile currTile = *board[currRow][column];

		if (currTile.getColumn() == tile.getColumn())
			continue;
		else if (currTile.getActualValue() == tile.getActualValue())
			return false;

	}

	return true;
}

bool Board::isValidColumn(Tile tile)
{
	int currColumn = tile.getColumn();

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		Tile currTile = *board[row][currColumn];

		if (currTile.getRow() == tile.getRow())
			continue;
		else if (currTile.getActualValue() == tile.getActualValue())
			return false;
	}

	return true;
}

bool Board::isValidBox(Tile tile)
{
	int currRow = tile.getRow();
	int currColumn = tile.getColumn();
	
	int rowStartingPointOfBox = currRow - (currRow % 3);
	int columnStartingPointOfBox = currColumn - (currColumn % 3);
	
	int boxRowMax = rowStartingPointOfBox + BOX_SIZE;
	int boxColumnMax = columnStartingPointOfBox + BOX_SIZE;

	for (int row = rowStartingPointOfBox; row < boxRowMax; row++)
	{
		for (int column = columnStartingPointOfBox; column < boxColumnMax; column++)
		{
			Tile currTile = *board[row][column];

			if (currTile.getRow() == currRow && currTile.getColumn() == currColumn)
				continue;
			else if (currTile.getActualValue() == tile.getActualValue())
				return false;
		}
	}

	return true;
}

