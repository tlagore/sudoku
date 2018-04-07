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
			this->board[row][column] = new Tile(rhs.board[row][column]->getActualValue(), row, column);
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

/*
	Sets the tiles actual value to supplied value. 
	If this takes a tile from an unsolved state to a solved state, the number of tiles to solve is decremented.
	If this takes a tile form a solved state to an unsolved state, the number of tiles to solve is incremented.
*/
void Board::setTileActualValue(int value, int row, int column)
{
	if (board[row][column]->getActualValue() == UNSOLVED && value != UNSOLVED)
		this->tilesToSolve--;
	else if (board[row][column]->getActualValue() != UNSOLVED && value == UNSOLVED)
		this->tilesToSolve++;

	board[row][column]->setActualValue(value);
}

/*
	Sets a specified tiles candidate values to a supplied set of candidate values
*/
void Board::setTileCandidateValues(int row, int col, unordered_set<int> candidateVals)
{
	Tile *tile = this->board[row][col];
	tile->clearCandidateValues();

	for (auto value : candidateVals) {
		tile->addCandidateValue(value);
	}
}

/*
	Adds a specified candidate value to a specified tile
*/
void Board::addTileCandidateValue(int value, int row, int col) {
	this->board[row][col]->addCandidateValue(value);
}

/*
	Clears all candidate values of a specified tile
*/
void Board::clearTileCandidateValues(int row, int col)
{
	this->board[row][col]->clearCandidateValues();
}

int Board::getTilesLeftToSolve()
{
	return this->tilesToSolve;
}

/*
	Checks to see that the board is in a valid state.

	Checks each row, each column, and each region for conflicts.

	Note: isValidState does not confirm that a puzzle is a solvable, valid sudoku, only that the currently supplied
	values do not conflict with any other value. For example, an empty puzzle would return as valid, even though it is 
	an unsolvable puzzle.
*/
bool Board::isValidState()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			if (board[row][column]->getActualValue() != UNSOLVED) {
				if (!isValidRow(*(board[row][column]))) {
					printf("Bad row, row: %d col: %d\n", row, column);
					return false;
				}

				if (!isValidColumn(*(board[row][column]))) {
					printf("Bad column, row: %d col: %d\n", row, column);
					return false;
				}

				if (!isValidRegion(*(board[row][column]))) {
					printf("Bad region, row: %d col: %d\n", row, column);
					return false;
				}
			}
		}
	}
	return true;
}

/*
	checks to see if a row is in a valid state (no duplicates)
*/
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

/*
	checks to see if a column is in a valid state (no duplicates)
*/
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

/*
	checks to see if a region is in a valid state (no duplicates)
*/
bool Board::isValidRegion(Tile tile)
{
	int currRow = tile.getRow();
	int currColumn = tile.getColumn();
	
	int rowStartingPointOfRegion = currRow - (currRow % REGION_SIZE);
	int columnStartingPointOfRegion = currColumn - (currColumn % REGION_SIZE);
	
	int regionRowMax = rowStartingPointOfRegion + REGION_SIZE;
	int regionColumnMax = columnStartingPointOfRegion + REGION_SIZE;

	for (int row = rowStartingPointOfRegion; row < regionRowMax; row++)
	{
		for (int column = columnStartingPointOfRegion; column < regionColumnMax; column++)
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

