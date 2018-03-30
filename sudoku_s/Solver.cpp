#include "stdafx.h"
#include "Solver.h"
#include <unordered_set>


Solver::Solver()
{
}

Solver::Solver(Board *board)
{
	this->board = board;
}

void Solver::generatePossibleValues()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			Tile tile = this->board->getTile(row, column);
			int actualTileValue = tile.getActualValue();

			if(isOpenTile(actualTileValue))
				generateTileValues(row, column);
		}
	}
}

void Solver::generateTileValues(int row, int column)
{
	unordered_set<int> possibleValues = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	removeRowValues(row, column, &possibleValues);
	removeColValues(row, column, &possibleValues);
	removeBoxValues(row, column, &possibleValues);

	board->setTilePossibleValues(row, column, possibleValues);

	if (possibleValues.size() == 1)
	{
		Tile tile = this->board->getTile(row, column);
		this->singleValueTiles.push_back(this->board->getTile(row, column));
	}

}

void Solver::removeRowValues(int row, int column, unordered_set<int>*possibleValues)
{
	int curValue;

	for (int c = 0; c < BOARD_SIZE; c++) {
		if (column == c)
			continue;

		curValue = board->getTile(row, c).getActualValue();
		if (possibleValues->find(curValue) != possibleValues->end()) {
			possibleValues->erase(curValue);
		}
	}
}

void Solver::removeColValues(int row, int column, unordered_set<int>* possibleValues)
{
	int curValue;

	for (int r = 0; r < BOARD_SIZE; r++) {
		if (row == r)
			continue;

		curValue = board->getTile(r, column).getActualValue();
		if (possibleValues->find(curValue) != possibleValues->end()) {
			possibleValues->erase(curValue);
		}
	}
}

void Solver::removeBoxValues(int row, int column, unordered_set<int>* possibleValues)
{
	int boxRow = row - (row % BOX_SIZE);
	int boxCol = column - (column % BOX_SIZE);
	int tileValue;

	for (int curRow = boxRow; curRow < boxRow + BOX_SIZE; curRow++) {
		for (int curCol = boxCol; curCol < boxCol + BOX_SIZE; curCol++) {
			if (curCol == column && curRow == row)
				continue;

			tileValue = this->board->getTile(curRow, curCol).getActualValue();
			if (possibleValues->find(tileValue) != possibleValues->end()) {
				possibleValues->erase(tileValue);
			}
		}
	}
}

void Solver::solve()
{
	generatePossibleValues();
	int numSolved = 0;
	
	while (!this->board->isSolved()) 
	{

		while (singleValueTiles.size() != 0)
		{

			Tile tile = singleValueTiles.front();
			int row = tile.getRow();
			int col = tile.getColumn();
			int value = *tile.getPossibleValues().begin();

			this->board->setTileActualValue(value, row, col);

			numSolved++;

			cancelRow(value, row, col);
			cancelColumn(value, row, col);
			cancelBox(value, row, col);

			singleValueTiles.pop_front();
		}

		if (!this->board->isSolved()) 
		{
			if (!performAdvancedSolve())
			{
				printf("No possible new solutions!\n");
				getchar();
				printPossibleValues();
				break;
			}
		}
	}

	cout << "Num solved: " << numSolved << endl;
}

bool Solver::performAdvancedSolve() 
{
	bool foundSolution = false;

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			int tileValue = this->board->getTile(row, column).getActualValue();
		
			if (isOpenTile(tileValue)) 
			{
				foundSolution = checkBoxLineReduction(row, column) || foundSolution;
				foundSolution = checkRowUnion(row, column) || foundSolution;
				foundSolution = checkColumnUnion(row, column) || foundSolution;
			}
		}
	}

	return foundSolution;
}


bool Solver::checkRowUnion(int currRow, int currColumn)
{
	bool foundSolution = false;
	Tile otherTile,
		tile = this->board->getTile(currRow, currColumn);
	int actualTileValue;
	unordered_set<int> possibleRowUnionValues;
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		otherTile = this->board->getTile(currRow, col);
		actualTileValue = otherTile.getActualValue();
		if (col == currColumn)
			continue;

		if (isOpenTile(actualTileValue))
		{
			for (auto possible : otherTile.getPossibleValues())
			{
				possibleRowUnionValues.insert(possible);
			}
		}
	}

	foundSolution = checkForValueMissing(possibleRowUnionValues, tile);

	return foundSolution;
}

bool Solver::checkColumnUnion(int currRow, int currColumn)
{
	bool foundSolution = false;
	Tile otherTile,
		tile = this->board->getTile(currRow, currColumn);
	int actualTileValue;
	unordered_set<int> possibleRowUnionValues;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		otherTile = this->board->getTile(row, currColumn);
		actualTileValue = otherTile.getActualValue();
		if (row == currRow)
			continue;

		if (isOpenTile(actualTileValue))
		{
			for (auto possible : otherTile.getPossibleValues())
			{
				possibleRowUnionValues.insert(possible);
			}
		}
	}

	foundSolution = checkForValueMissing(possibleRowUnionValues, tile);

	return foundSolution;
}

bool Solver::checkForValueMissing(unordered_set<int> possibleUnionValues, Tile tile)
{
	for (auto possible : tile.getPossibleValues())
	{
		if (possibleUnionValues.find(possible) == possibleUnionValues.end())
		{
			this->board->clearTilePossibleValues(tile.getRow(), tile.getColumn());
			this->board->addTilePossibleValue(possible, tile.getRow(), tile.getColumn());
			this->singleValueTiles.push_back(this->board->getTile(tile.getRow(), tile.getColumn()));
			return true;	
		}
	}
	return false;
}

void Solver::printPossibleValues()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			Tile tile = this->board->getTile(row, col);
			int actualTileValue = tile.getActualValue();
			
			if (isOpenTile(actualTileValue))
			{
				printf("Tile(row %d, col %d): ", row, col);
				unordered_set<int> possibleValuesOfTile = tile.getPossibleValues();
				for (auto possible : possibleValuesOfTile)
				{
					printf("%d ", possible);
				}
				printf("\n");
			}
		}
	}
}

bool Solver::isOpenTile(int value)
{
	return (value == -1);
}

bool Solver::checkBoxLineReduction(int row, int column) 
{
	int boxRow = row - (row % BOX_SIZE);
	int boxCol = column - (column % BOX_SIZE);
	unordered_set<int> possibleUnion;
	Tile otherTile,
		tile = this->board->getTile(row, column);
	bool foundSolution = false;

	for (int r = boxRow; r < boxRow + BOX_SIZE ; r++)
	{
		for (int c = boxCol; c < boxCol + BOX_SIZE; c++)
		{
			if (c == column && r == row)
				continue;

			otherTile = this->board->getTile(r, c);
			int actualTileValue = otherTile.getActualValue();
			
			if (isOpenTile(actualTileValue)) 
			{
				for (auto possible : otherTile.getPossibleValues())
					possibleUnion.insert(possible);
			}

		}
	}

	//for (auto possible : tile.getPossibleValues()) 
	//{
	//	if (possibleUnion.find(possible) == possibleUnion.end()) 
	//	{
	//		this->board->clearTilePossibleValues(tile.getRow(), tile.getColumn());
	//		this->board->addTilePossibleValue(possible, tile.getRow(), tile.getColumn());
	//		this->singleValueTiles.push_back(this->board->getTile(tile.getRow(), tile.getColumn()));
	//		foundSolution = true;
	//		break;
	//	}
	//}
	foundSolution = checkForValueMissing(possibleUnion, tile);
	return foundSolution;
}

void Solver::cancelRow(int value, int row, int column)
{

	for (int col = 0; col < BOARD_SIZE; col++)
	{
		if (col == column)
			continue;
		else
		{
			Tile tile = board->getTile(row, col);
			removeValue(tile, value);
		}
	}

}

void Solver::cancelColumn(int value, int rowCurrTile, int column)
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		if (row == rowCurrTile)
			continue;
		else
		{

			Tile tile = board->getTile(row, column);
			removeValue(tile, value);
		}
	}
}

void Solver::cancelBox(int value, int row, int column)
{
	int boxRow = row - (row % BOX_SIZE);
	int boxCol = column - (column % BOX_SIZE);

	for (int curRow = boxRow; curRow < boxRow + BOX_SIZE; curRow++) 
	{
		for (int curCol = boxCol; curCol < boxCol + BOX_SIZE; curCol++) 
		{
			if (curCol == column && curRow == row)
				continue;
			
			Tile tile = board->getTile(curRow, curCol);
			removeValue(tile, value);
			
		}
	}
}

void Solver::removeValue(Tile tile, int value)
{
	if (tile.getPossibleValues().size() > 1)
	{
		board->removePossibleValue(value, tile.getRow(), tile.getColumn());
		if (board->getTile(tile.getRow(), tile.getColumn()).getPossibleValues().size() == 1)
			singleValueTiles.push_back(board->getTile(tile.getRow(), tile.getColumn()));
	}
}
Board Solver::getBoard()
{
	return *board;
}

void Solver::reset(Board *board) {
	this->singleValueTiles.clear();
	this->board = NULL;
	this->board = board;
}

Solver::~Solver()
{
	//delete this->board;
	//this->board = NULL;
}
