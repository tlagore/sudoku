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

	tilesToSolve = 81;
}

Board::Board(const Board &rhs)
{
	this->tilesToSolve = rhs.tilesToSolve;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int column = 0; column < BOARD_SIZE; column++) {
			this->board[row][column] = rhs.board[row][column];
		}
	}
}

Board::Board(int inputForNewBoard[BOARD_SIZE][BOARD_SIZE])
{
	tilesToSolve = BOARD_SIZE * BOARD_SIZE;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			board[row][column] = new Tile(inputForNewBoard[row][column], row, column);
			if (inputForNewBoard[row][column] != -1)
				tilesToSolve--;
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
	return *(board[row][column]);
}

void Board::setTileActualValue(int value, int row, int column)
{
	if (board[row][column]->getActualValue() == -1)
		tilesToSolve--;

	board[row][column]->setActualValue(value);
}

void Board::setTileCandidateValues(int row, int col, unordered_set<int> candidateVals)
{
	Tile *tile = this->board[row][col];
	tile->clearCandidateValues();

	for (auto value : candidateVals) {
		tile->addCandidateValue(value);
	}
}

void Board::addTileCandidateValue(int value, int row, int col) {
	this->board[row][col]->addCandidateValue(value);
}

void Board::clearTileCandidateValues(int row, int col)
{
	this->board[row][col]->clearCandidateValues();
}

bool Board::isValidState()
{

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			if (!isValidRow(*(board[row][column])))
				return false;

			if (!isValidColumn(*(board[row][column])))
				return false;
				
			if (!isValidBox(*(board[row][column])))
				return false;
		}
	}
	return true;
}

bool Board::isValidRow(Tile tile)
{
	int currRow = tile.getRow();
	
	for (int column = 0; column < BOARD_SIZE; column++)
	{
		Tile currTile = *(board[currRow][column]);

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
		Tile currTile = *(board[row][currColumn]);

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
			Tile currTile = *(board[row][column]);

			if (currTile.getRow() == currRow && currTile.getColumn() == currColumn)
				continue;
			else if (currTile.getActualValue() == tile.getActualValue())
				return false;
		}
	}

	return true;
}

void Board::printCandidateValues()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			Tile *tile = this->board[row][col];
			int actualTileValue = tile->getActualValue();

			if (actualTileValue == -1)
			{
				printf("Tile(row %d, col %d): ", row, col);
				tile->printCandidateValues();
				printf("\n");
			}
		}
	}
}

bool Board::removeCandidateValue(int value, int row, int column)
{
	return this->board[row][column]->removeCandidateValue(value);
}
bool Board::isSolved()
{
	return tilesToSolve == 0;
}
void Board::printBoard()
{
	printf("===================================================================\n");
	char space = ' ';
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		printf("||");
		for (int column = 0; column < BOARD_SIZE; column += 3)
		{
			int first = board[row][column]->getActualValue();
			int second = board[row][column + 1]->getActualValue();
			int third = board[row][column + 2]->getActualValue();
			first = first == -1 ? (int)space : first + (int)ZERO;
			second = second == -1 ? (int)space : second + (int)ZERO;
			third = third == -1 ? (int)space : third + (int)ZERO;
			printf("%3c%-3c|%3c%-3c|%3c%-3c||", first, space, second, space, third, space);
		}
		printf("\n");
		if ((row+1) % 3 == 0)
			printf("====================================================================\n");
		else if(row != 8)
			printf("--------------------------------------------------------------------\n");
	}
}

