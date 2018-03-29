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
			generateTileValues(row, column);
		}
	}
}

void Solver::generateTileValues(int row, int column)
{
	//Tile tile = board->getTile(row, column);
	unordered_set<int> possibleValues = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	
	removeRowValues(row, column, &possibleValues);
	removeColValues(row, column, &possibleValues);
	removeBoxValues(row, column, &possibleValues);

	board->setTilePossibleValues(row, column, possibleValues);
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
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			Tile curTile = this->board->getTile(row, column);
			
			if (curTile.getActualValue() == -1 && curTile.getPossibleValues().size() == 1) {
				curTile.setActualValue(*curTile.getPossibleValues().begin());
				//cancel row
				//cancel column
				//cancel box
			}
		}
	}
}


Board Solver::getBoard()
{
	return *board;
}

Solver::~Solver()
{
	//delete this->board;
	//this->board = NULL;
}
