#include "stdafx.h"
#include "Solver.h"

Solver::Solver()
{
}

Solver::Solver(Board *board)
{
	this->board = board;
}

/*
	generatePossibleValues loops through the entire puzzle, generating all possible values for each tile
*/
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

/*
	generateTileValues takes in a specific row and column and generates the possible values for this Tile.

	It does so by assuming that the tile has all possible values (1-9 for a 9x9 sudoku) and then iteratively 
	removes possible values by checking the other values in the tiles respective row, column, and box
*/
void Solver::generateTileValues(int row, int column)
{
	unordered_set<int> possibleValues = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	removeRowValues(row, column, &possibleValues);
	removeColValues(row, column, &possibleValues);
	removeBoxValues(row, column, &possibleValues);

	board->setTilePossibleValues(row, column, possibleValues);

	//if the possibleValues for this tile has only 1 value, it is the solution to the tile - add it to the singleValueTiles vector
	if (possibleValues.size() == 1)
	{
		Tile tile = this->board->getTile(row, column);
		this->singleValueTiles.push_back(this->board->getTile(row, column));
	}

}

/*
	Removes row values as possible solutions to a tile by iterating through each column for the row, skipping its own Tile,
	If a value is encountered is in the supplied possibleValue unordered_set, then it is removed as a possible value
*/
void Solver::removeRowValues(int row, int column, unordered_set<int>*possibleValues)
{
	int curValue;

	for (int c = 0; c < BOARD_SIZE; c++) {
		//skip the tile that we are checking values for
		if (column == c)
			continue;

		curValue = board->getTile(row, c).getActualValue();
		if (possibleValues->find(curValue) != possibleValues->end()) {
			possibleValues->erase(curValue);
		}
	}
}

/*
	Removes column values as possible solutions to a tile by iterating through each row for the row, skipping its own Tile,
	If a value is encountered is in the supplied possibleValue unordered_set, then it is removed as a possible value
*/
void Solver::removeColValues(int row, int column, unordered_set<int>* possibleValues)
{
	int curValue;

	for (int r = 0; r < BOARD_SIZE; r++) {
		//skip th e tile that we are checking vlaues for
		if (row == r)
			continue;

		curValue = board->getTile(r, column).getActualValue();
		if (possibleValues->find(curValue) != possibleValues->end()) {
			possibleValues->erase(curValue);
		}
	}
}

/*
	Removes box values as possible solutions to a tile by iterating through the box that the tile belongs to,
	If a value is encountered is in the supplied possibleValue unordered_set, then it is removed as a possible value
*/
void Solver::removeBoxValues(int row, int column, unordered_set<int>* possibleValues)
{
	//find what the row/column is the start of the box
	int boxRow = row - (row % BOX_SIZE);
	int boxCol = column - (column % BOX_SIZE);
	int tileValue;

	//loop from box start row/column to the end of the box row/column
	for (int curRow = boxRow; curRow < boxRow + BOX_SIZE; curRow++) {
		for (int curCol = boxCol; curCol < boxCol + BOX_SIZE; curCol++) {
			//skip the Tile we are checking for
			if (curCol == column && curRow == row)
				continue;

			tileValue = this->board->getTile(curRow, curCol).getActualValue();
			if (possibleValues->find(tileValue) != possibleValues->end()) {
				possibleValues->erase(tileValue);
			}
		}
	}
}

/*
	solve() runs a series of checks to attempt to solve the board.

	First the singleValueTiles vector is checked to see if there are any tiles that have only one possible solution.
	Each Tile in the singleValueTiles vector has its value set to its only possible value, and then that value is removed
	from other Tile values in the same row/column/box as the Tile that was set.

	When the singleValueTiles set is exhausted, a check is performed to see if the board is solved
	If the board is not solved, more advanced checks are performed to see if further cancellations can be made.

*/
bool Solver::solve()
{
	generatePossibleValues();
	
	bool isSolved = false;

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
			//if performAdvancedSolve doesn't find a new Tile value then our sudoku solver can't handle this puzzle, exit solve loop
			if (!performAdvancedSolve())
			{
				//printPossibleValues();
				break;
			}
		}
	}

	isSolved = this->board->isSolved();
	return isSolved;
}


/*
	cancelRow takes in a value and cancels that value as a possible value from all Tiles in the supplied row
*/
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

/*
	cancelColumn takes in a value and cancels that value as a possible value from all Tiles in the supplied column
*/
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

/*
	cancelBox takes in a value and cancels that value as a possible value from all Tiles in the box to which the Tile belongs
*/
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

/*
	performAdvancedSolve performs a series of checks that go above simple cross-hatching (eliminating possible values by checking 
	the values of interesecting rows/columns and values within the box itself) to generate new solutions for the puzzle.

	These methods are:
		- Box line reduction
			- A form of intersection removal in which candidates which must belong to 
			  a line can be ruled out as candidates in a block (or box) that intersects the line in question.

		- row union / column union
			- This checks if any particular possible value in a Tile is the not found in the union of the possible
			  values of the other tiles corresponding to that tiles row or column. This indicates that it is the 
			  only possible Tile for the value for that row/column
			- Same as box line reduction except for column & row

		- check unsolved cancel
			- in progress..
			- Checks to see if a cancellation can be made by seeing that a possible value appears in only one row or column in a specific box, 
			  therefore the value can be removed as a possible value from all other boxes in the specified row or column
*/
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
				//foundSolution = checkUnsolvedCancel(row, column) || foundSolution;

				//only needs to be checked once per box
				//if (row % BOX_SIZE == 0 && column % BOX_SIZE == 0)
				//foundSolution = checkUnsolvedCancel(row, column) || foundSolution;
				
				foundSolution = checkUnsolvedCancel2(row, column) || foundSolution;
			}
		}
	}

	return foundSolution;
}

/*
	- A form of intersection removal in which candidates which must belong to
	a line can be ruled out as candidates in a block (or box) that intersects the line in question.
*/
bool Solver::checkBoxLineReduction(int row, int column)
{
	int boxRow = row - (row % BOX_SIZE);
	int boxCol = column - (column % BOX_SIZE);
	unordered_set<int> possibleUnion;
	Tile otherTile,
		tile = this->board->getTile(row, column);
	bool foundSolution = false;

	for (int r = boxRow; r < boxRow + BOX_SIZE; r++)
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

	foundSolution = checkForValueMissing(possibleUnion, tile);

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


/*
	Attempt #2 at checkUnsolvedCancel

	Idea:
		- Create a mapping of row: all possible values for a particular box.
		- check each value in each mapping to see if appears in the other row possible values.
		- if the value does not belong to the other mappings, it *must* be found in this row of this box 
		- cancel the particular value from all other tiles in same row the other boxes
	
	AND YEAH I USED 1 R IN CUR. GIT SUM.	
*/

bool Solver::checkUnsolvedCancel2(int curRow, int curCol)
{
	bool foundSolution = false;

	unordered_map<int, unordered_set<int>> rowPossibles;
	unordered_map<int, unordered_set<int>> colPossibles;
	//find start of box row
	int boxRow = curRow - (curRow % BOX_SIZE);
	//find start of box column
	int boxCol = curCol - (curCol % BOX_SIZE);
	bool performedCancellation = false;


	for (int curBoxRow = boxRow; curBoxRow < boxRow + BOX_SIZE; curBoxRow++)
	{
		for (int curBoxCol = boxCol; curBoxCol < boxCol + BOX_SIZE; curBoxCol++)
		{
			// for each possible value in this tile add it to the corresponding row/column set of possible values
			for (auto possible : this->board->getTile(curBoxRow, curBoxCol).getPossibleValues())
			{
				rowPossibles[curBoxRow].insert(possible);
				colPossibles[curBoxCol].insert(possible);
			}
		}
	}

	//check to see if the rows of the box or the columns of a box have a unique value in a particular column or row
	//and set cancellation flag based on this
	performedCancellation = checkSetsContainsUnique(rowPossibles, curRow, curCol, "row")
		|| checkSetsContainsUnique(colPossibles, curRow, curCol, "column");

	return performedCancellation;
}


/*
	This is only a little hacky because, in an attempt to reduce duplicated code, the function takes in a string indicating
	whether or not it is checking sets of rows or sets of columns, then uses this string to perform the appropriate cancellation.

	It would be better if checkSetsContainUnique was unaware of the column/row concept but can't think of a way to handle this without
	duplicating code or a flag.
*/
bool Solver::checkSetsContainsUnique(unordered_map<int, unordered_set<int>> sets, int curRow, int curCol, string type) {

	bool performedCancellation = false;
	//check if any value in a row possible is not found in the other rows

	vector <unordered_set<int>> vOtherSets;

	//for each set
	for (auto keyValue : sets)
	{
		//generate a list of the other sets
		for (auto otherSets : sets)
		{
			if (otherSets.first == keyValue.first)
				continue;

			vOtherSets.push_back(otherSets.second);
		}

		//for each possible value in our test set
		for (auto possible : keyValue.second)
		{
			bool notInOtherSets = true;
			for (auto set : vOtherSets) {
				notInOtherSets = !setContains(set, possible) && notInOtherSets;

				//if in any of the sets, notInOtherSets = false
				if (!notInOtherSets)
					break;
			}
			if (notInOtherSets)
			{
				if (type == "row")
					performedCancellation = cancelRowSkipSameBox(possible, keyValue.first, curCol) || performedCancellation;
				else if (type == "column") {
					performedCancellation = cancelColumnSkipSameBox(possible, curRow, keyValue.first) || performedCancellation;
				}
			}
		}
		vOtherSets.clear();
	}
	return performedCancellation;
}

/*
	simple wrapper to clarify the logic behind checking if a value exists in an unordered_set
*/
bool Solver::setContains(unordered_set<int> set, int value) 
{
	return (set.find(value) != set.end());
}

/*TODO Hacky Reason
	1.) using a switch statement.  Is this a neccessary case for using a switch statement?
	Is there a better way of doing this?
*/
bool Solver::checkUnsolvedCancel(int currRow, int currColumn)
{
	bool foundSolution = false;
	Tile currTile = this->board->getTile(currRow, currColumn);
	for (auto possible : currTile.getPossibleValues())
	{
		int rowOrCol = checkForValueInBox(possible, currRow, currColumn);
		switch(rowOrCol)	
		{
			case 0:
				cancelRowSkipSameBox(possible, currRow, currColumn);
				if(this->singleValueTiles.size() >= 1)
					foundSolution = true;
				break;
			case 1:
				cancelColumnSkipSameBox(possible, currRow, currColumn);
				if (this->singleValueTiles.size() >= 1)
					foundSolution = true;
				break;
			default:
				break;
		}
	}

	return foundSolution;
}

/*TODO make this less hacky of a way of doing it for now I just want to test if this logic works
Hacky being
1. Using Flags and returning flags
2. initializing the otherTile, otherTileActualValue and possibleValuesOfOtherTile in the loops.
	Although I think its hacky it seems neccessary to make it work not sure why.
*/
int Solver::checkForValueInBox(int currPossible, int currRow, int currColumn)
{
	int inRowFlag = 0;
	int inColumnFlag = 1;
	int inBoxFlag = -1;

	int validCancelFlag = -1;
	int inRowCounter = 0, inColumnCounter = 0;
	int boxRow = currRow - (currRow % BOX_SIZE);
	int boxCol = currColumn - (currColumn % BOX_SIZE);

	for (int row = boxRow; row < boxRow + BOX_SIZE; row++)
	{
		for (int col = boxCol; col < boxCol + BOX_SIZE; col++)
		{
			Tile otherTile = this->board->getTile(row, col);
			int otherTileActualValue = otherTile.getActualValue();
			unordered_set<int> possibleValuesOfOtherTile = otherTile.getPossibleValues();

			if (isOpenTile(otherTileActualValue))
			{
				if (isInPossibleValues(possibleValuesOfOtherTile, currPossible))
				{
					if (col == currColumn && row == currRow)
						continue;
					if (col == currColumn)
						inColumnCounter++;
					else if (row == currRow)
						inRowCounter++;
					else
					{
						validCancelFlag = inBoxFlag;
						return validCancelFlag;
					}
				}
			}
		}
	}

	if (inRowCounter == 0 && inColumnCounter > 0)
		validCancelFlag = inColumnFlag;

	if (inRowCounter > 0 && inColumnCounter == 0)
		validCancelFlag = inRowFlag;

	return validCancelFlag;
}

bool Solver::isInPossibleValues(unordered_set<int> possibleValues, int possible)
{
	return (possibleValues.find(possible) != possibleValues.end());
}

bool Solver::cancelRowSkipSameBox(int possibleValue, int currRow, int currColumn)
{
	int boxCol = currColumn - (currColumn % BOX_SIZE);
	bool removedValue = false;

	for (int col = 0; col < BOARD_SIZE; col++)
	{
		if (col >= boxCol && col < boxCol + BOX_SIZE)//isInBoxRow(boxCol, col))
			continue;
		else
		{
			Tile tile = board->getTile(currRow, col);
			if(isOpenTile(tile.getActualValue()))
				removedValue = removeValue(tile, possibleValue) || removedValue;
		}
	}

	return removedValue;
}

bool Solver::cancelColumnSkipSameBox(int possibleValue, int currRow, int currColumn)
{
	int boxRow = currRow - (currRow % BOX_SIZE);
	bool removedValue = false;

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		if (row >= boxRow && row < boxRow + BOX_SIZE)
			continue;
		else
		{
			Tile tile = board->getTile(row, currColumn);
			removedValue =  removeValue(tile, possibleValue) || removedValue;
		}
	}

	return removedValue;
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

bool Solver::removeValue(Tile tile, int value)
{
	bool removedValue = false;
	if (tile.getPossibleValues().size() > 1)
	{
		removedValue = board->removePossibleValue(value, tile.getRow(), tile.getColumn());
		if (board->getTile(tile.getRow(), tile.getColumn()).getPossibleValues().size() == 1)
			singleValueTiles.push_back(board->getTile(tile.getRow(), tile.getColumn()));
	}

	return removedValue;
}
Board Solver::getBoard()
{
	return *board;
}

void Solver::reset(Board *otherBoard) {
	this->numSolved = 0;
	this->singleValueTiles.clear();
	//delete this->board;
	this->board = NULL;
	this->board = otherBoard;
}

int Solver::getNumberSolved()
{
	return numSolved;
}

Solver::~Solver()
{
	//delete this->board;
	//this->board = NULL;
}
