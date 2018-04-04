#include "stdafx.h"
#include "Solver.h"

Solver::Solver()
{
}

Solver::Solver(Board *board)
{
	this->board = board;
	this->numSolved = 0;
}

/*
	generateCandidateValues loops through the entire puzzle, generating all candidate values for each tile
*/
void Solver::generateCandidateValues()
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
	generateTileValues takes in a specific row and column and generates the candidate values for this Tile.

	It does so by assuming that the tile has all candidate values (1-9 for a 9x9 sudoku) and then iteratively 
	removes candidate values by checking the other values in the tiles respective row, column, and region
*/
void Solver::generateTileValues(int row, int column)
{
	unordered_set<int> candidateValues = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	removeRowValues(row, column, &candidateValues);
	removeColValues(row, column, &candidateValues);
	removeRegionValues(row, column, &candidateValues);

	board->setTileCandidateValues(row, column, candidateValues);

	//if the candidateValues for this tile has only 1 value, it is the solution to the tile - add it to the singleValueTiles vector
	if (candidateValues.size() == 1)
	{
		//Tile tile = this->board->getTile(row, column);
		solveTile(this->board->getTile(row, column));
		//this->singleValueTiles.push_back(this->board->getTile(row, column));
	}

}

/*
	Removes row values as candidate solutions to a tile by iterating through each column for the row, skipping its own Tile,
	If a value is encountered is in the supplied candidateValue unordered_set, then it is removed as a candidate value
*/
void Solver::removeRowValues(int row, int column, unordered_set<int>*candidateValues)
{
	int curValue;

	for (int c = 0; c < BOARD_SIZE; c++) {
		//skip the tile that we are checking values for
		if (column == c)
			continue;

		curValue = board->getTile(row, c).getActualValue();
		if (candidateValues->find(curValue) != candidateValues->end()) {
			candidateValues->erase(curValue);
		}
	}
}

/*
	Removes column values as candidate solutions to a tile by iterating through each row for the row, skipping its own Tile,
	If a value is encountered is in the supplied candidateValue unordered_set, then it is removed as a candidate value

	Inputs: (int) row - row for the tile being checked
			(int) column - column for the tile being checked
			(unordered_set<int>*) candidateValues - current possible candidates for this tile
*/
void Solver::removeColValues(int row, int column, unordered_set<int>* candidateValues)
{
	int curValue;

	for (int r = 0; r < BOARD_SIZE; r++) {
		//skip th e tile that we are checking vlaues for
		if (row == r)
			continue;

		curValue = board->getTile(r, column).getActualValue();
		//if value is found in column that is currently in candidate list, remove from candidates
		if (candidateValues->find(curValue) != candidateValues->end()) {
			candidateValues->erase(curValue);
		}
	}
}

/*
	Removes region values as candidate solutions to a tile by iterating through the region that the tile belongs to,
	If a value is encountered is in the supplied candidateValue unordered_set, then it is removed as a candidate value

	Inputs: (int) row - row for the tile being checked
			(int) column - column for the tile being checked
			(unordered_set<int>*) candidateValues - current possible candidates for this tile

	Outputs: None
*/
void Solver::removeRegionValues(int row, int column, unordered_set<int>* candidateValues)
{
	//find what the row/column is the start of the region
	int regionRow = row - (row % BOX_SIZE);
	int regionCol = column - (column % BOX_SIZE);
	int tileValue;

	//loop from region start row/column to the end of the region row/column
	for (int curRow = regionRow; curRow < regionRow + BOX_SIZE; curRow++) {
		for (int curCol = regionCol; curCol < regionCol + BOX_SIZE; curCol++) {
			//skip the Tile we are checking for
			if (curCol == column && curRow == row)
				continue;

			tileValue = this->board->getTile(curRow, curCol).getActualValue();
			if (candidateValues->find(tileValue) != candidateValues->end()) {
				candidateValues->erase(tileValue);
			}
		}
	}
}

/*
	solveTile takes in a specified tile and solves the tile.
	
	It checks to ensure that the tile has only one candidate value, then sets the value to its only candidate value.

	It then cancels in the row, column, and region for which the tile belongs, removing all candidate values that are equal 
	to the value set in this tile.

	Inputs: (Tile) tile - the tile for which the actual value is being set
	Outputs: None
*/
void Solver::solveTile(Tile tile) {
	// ensure we are being asked to solve a tile that has only one candidate value

	if (tile.getCandidateValues().size() == 1) {

		
		int value = *tile.getCandidateValues().begin();
		int row = tile.getRow();
		int col = tile.getColumn();

		this->board->setTileActualValue(value, row, col);
		this->numSolved++;


		cancelRow(value, row, col);
		cancelColumn(value, row, col);
		cancelRegion(value, row, col);
	}
}

/*
	solve() runs a series of checks to attempt to solve the board.

	First the singleValueTiles vector is checked to see if there are any tiles that have only one candidate solution.
	Each Tile in the singleValueTiles vector has its value set to its only candidate value, and then that value is removed
	from other Tile values in the same row/column/region as the Tile that was set.

	When the singleValueTiles set is exhausted, a check is performed to see if the board is solved
	If the board is not solved, more advanced checks are performed to see if further cancellations can be made.

*/
bool Solver::solve()
{
	generateCandidateValues();
	
	bool isSolved = false;

	while (!this->board->isSolved()) 
	{
		//if performAdvancedSolve doesn't find a new Tile value then our sudoku solver can't handle this puzzle, exit solve loop
		if (!performSolve())
		{
			//printCandidateValues();
			break;
		}
	}

	isSolved = this->board->isSolved();
	return isSolved;
}


/*
	cancelRow takes in a value and cancels that value as a candidate value from all Tiles in the supplied row
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
	cancelColumn takes in a value and cancels that value as a candidate value from all Tiles in the supplied column
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
	cancelRegion takes in a value and cancels that value as a candidate value from all Tiles in the region to which the Tile belongs
*/
void Solver::cancelRegion(int value, int row, int column)
{
	int regionRow = row - (row % BOX_SIZE);
	int regionCol = column - (column % BOX_SIZE);

	for (int curRow = regionRow; curRow < regionRow + BOX_SIZE; curRow++)
	{
		for (int curCol = regionCol; curCol < regionCol + BOX_SIZE; curCol++)
		{
			if (curCol == column && curRow == row)
				continue;

			Tile tile = board->getTile(curRow, curCol);
			removeValue(tile, value);

		}
	}
}

/*
	performSolve performs a series of checks that go above simple cross-hatching (eliminating candidate values by checking 
	the values of interesecting rows/columns and values within the region itself) to generate new solutions for the puzzle.

	These methods are:
		- Region line reduction
			- A form of intersection removal in which candidates which must belong to 
			  a line can be ruled out as candidates in a block (or region) that intersects the line in question.

		- row union / column union
			- This checks if any particular candidate value in a Tile is the not found in the union of the candidate
			  values of the other tiles corresponding to that tiles row or column. This indicates that it is the 
			  only candidate Tile for the value for that row/column
			- Same as region line reduction except for column & row

		- check unsolved cancel
			- Checks to see if a cancellation can be made by seeing that a candidate value appears in only one row or column in a specific region, 
			  therefore the value can be removed as a candidate value from all other regiones in the specified row or column

		- IN PROGRESS FUNCTIONS:
		- X wing cancel 
			- refers to the instance where a candidate value can be found in one of two rows (or columns)
			  in two adjacent regiones, and the same candidate value can be found in 2 or 3 squares of the 3rd adjacent region.
			  this indicates that the candidate MUST appear in the two columns of the first two regiones, and can be cancelled
			  from the 3rd region in these two columns:
			  Example: (-'s indicate an ignored region, 3's are considered to be candidates in an unsolved region)
			  
			  Region:	1				2				3
					3	-	-	|	-	-	3	|	-	-	3 <- these two 3s can be cancelled
					-	-	3	|	-	-	3	|	-	-	3 <- because the 3 must belong in row 1 & 2 for region 1 & 2
					-	-	-	|	-	-	-	|	-	-	3 <- 3 must go in this tile

		- Y wing cancel
			- Occurs when a Tile (the pivot) has exactly 2 values and is intersected by two conjugate pair Tiles. These are called pincers
				(definition: conjugate pair): a tile that contains a candidate that must go in one of the two tiles
				
				If these two intersecting Tiles share a value, that value can be eliminated from THEIR intersection.
				Example:

			Region    
					0	  1		 2     3  	4   5 
			0	  (2,1)	  4	     5   | 8  (3,2) 9
			1		6   (3,1)  (8,9) | 5  (2,3) 7
			2	(7,8,9)(7,8,9)(7,8,9)| 1    6   4

				(0,0) is the pivot, having pincers at (1,1), and (0, 4). Notice they share a conjugate pair value.
				In the case of (1,1), (1) is shared with (0,0). in the case of (0,4), (2), is shared with (0,0).

				The shared value between (1,1) and (0,4) is (3), which can then be removed from their intersection (1,4)
				solving the square to (2).

		- Naked tuple cancel
			- when a tuple of tiles contains the same "tuple" candidate solutions)
				for example if 3 tiles share the candidate values 1, 3, 7, then 1, 3, and 7 must belong to those tiles.. cancel
				1, 3, and 7 from row OR column OR region, depending where the tuple was found.

		- Hidden tuple cancel
			- when a tuple of tiles contains the same "tuple" candidate solutions but also have additional values present.
				This is a much harder check, if 4 and 5 are only found in 2 tiles, but have many other "candidates", 4 and 5 must 
				belong to these two tiles, cancel all possible other values in these 2 tiles except for 4 and 5.
				can also be extended to a larger tuple.

		Inputs: None
		Outputs: (bool) foundSolution - indicates whether or not the puzzle has been progressed via the call to performSolve
					if there is no progression on a call to performSolve, our algorithm is insufficient to solve the puzzle
*/
bool Solver::performSolve() 
{
	bool foundSolution = false;

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			Tile tile = this->board->getTile(row, column);
			int tileValue = tile.getActualValue();
			int candidateSize = tile.getCandidateValues().size();
		
			if (isOpenTile(tileValue) && candidateSize > 1)
			{
				
				foundSolution = checkRegionLineReduction(row, column) || foundSolution;
				foundSolution = checkRowUnion(row, column) || foundSolution;

				foundSolution = checkColumnUnion(row, column) || foundSolution;

				//TODO: implement check for XWingCancel
				//foundSolution = checkXWingCancel(row, column);

				//TODO: implement check for YWingCancel
				//if (candidateSize == 2)
					//foundSolution = checkXWingCancel(row, column);

				//TODO: implement check for Naked tuple
				//foundSolution = checkNakedTuple(row, column);

				//TODO: implement check for Check Hidden Tuple
				//foundSolution = checkHiddenTuple(row, column);
			}

			//checked once per region, as checkPointedTuple will find all pointed tuples in a region on call
			if (row % BOX_SIZE == 0 && column % BOX_SIZE == 0)
				foundSolution = checkPointedTuple(row, column) || foundSolution;
		}
	}

	return foundSolution;
}

/*
	checkRegionLineReduction performs a "Region line reduction" cancellation of a specific tile.
	A region line reduction is defined as: "A form of intersection removal in which candidates which must belong to
	a line can be ruled out as candidates in a block (or region) that intersects the line in question."

	checkRegionLineReduction is performed by generating a union of all candidate values of the other tiles within the same
	square as the tile being checked. Then the candidate values of the tile being checked are checked against the union
	of the other tiles. If a candidate value is not found to be in the union, a region line reduction has been detected,
	solve the tile.

	Inputs: (int) row - row of tile being checked
	(int) column - column of tile being checked

	Returns: (bool) foundSolution - indicates if a tile has been solved as a result of checkRegionLineReduction or not
*/
bool Solver::checkRegionLineReduction(int row, int column)
{
	int regionRow = row - (row % BOX_SIZE);
	int regionCol = column - (column % BOX_SIZE);
	unordered_set<int> candidateUnion;
	Tile otherTile,
		tile = this->board->getTile(row, column);
	bool foundSolution = false;

	for (int r = regionRow; r < regionRow + BOX_SIZE; r++)
	{
		for (int c = regionCol; c < regionCol + BOX_SIZE; c++)
		{
			if (c == column && r == row)
				continue;

			otherTile = this->board->getTile(r, c);
			int actualTileValue = otherTile.getActualValue();

			if (isOpenTile(actualTileValue))
			{
				for (auto candidate : otherTile.getCandidateValues())
					candidateUnion.insert(candidate);
			}

		}
	}

	foundSolution = checkForValueMissing(candidateUnion, tile);

	return foundSolution;
}

/*
checkRowUnion checks an indivdual tile's candidate values against the union of the rest of the row's candidate values

If a candidate value of a tile is not found to be in the union of the rest of the row, it is the only place that a value
can go. Solve it.

Inputs: (int) row - row of tile being checked
		(int) column - column of tile being checked

Returns: (bool) foundSolution - indicates if a tile has been solved as a result of checkRowUnion or not
*/
bool Solver::checkRowUnion(int currRow, int currColumn)
{
	bool foundSolution = false;
	Tile otherTile,
		tile = this->board->getTile(currRow, currColumn);
	int actualTileValue;
	unordered_set<int> candidateRowUnionValues;
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		otherTile = this->board->getTile(currRow, col);
		actualTileValue = otherTile.getActualValue();
		if (col == currColumn)
			continue;

		if (isOpenTile(actualTileValue))
		{
			for (auto candidate : otherTile.getCandidateValues())
			{
				candidateRowUnionValues.insert(candidate);
			}
		}
	}

	foundSolution = checkForValueMissing(candidateRowUnionValues, tile);

	return foundSolution;
}


/*
	checkColumnUnion checks an indivdual tiles candidate values against the union of the rest of the column's candidate values

	If a candidate value of a tile is not found to be in the union of the rest of the column, it is the only place that a value 
	can go. Solve it.

	Inputs: (int) row - row of tile being checked
			(int) column - column of tile being checked

	Returns: (bool) foundSolution - indicates if a tile has been solved as a result of checkColumnUnion or not
*/
bool Solver::checkColumnUnion(int currRow, int currColumn)
{
	bool foundSolution = false;
	Tile otherTile,
		tile = this->board->getTile(currRow, currColumn);
	int actualTileValue;
	unordered_set<int> candidateRowUnionValues;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		otherTile = this->board->getTile(row, currColumn);
		actualTileValue = otherTile.getActualValue();
		if (row == currRow)
			continue;

		if (isOpenTile(actualTileValue))
		{
			for (auto candidate : otherTile.getCandidateValues())
			{
				candidateRowUnionValues.insert(candidate);
			}
		}
	}

	foundSolution = checkForValueMissing(candidateRowUnionValues, tile);

	return foundSolution;
}


/*
	checkPointedTuple checks to see if a region contains a value that appears only in one row or column of a region. 
	If it does, then the value must belong in that row or column of that region and can be used to cancel this 
	candidate value from the rest of the row or column. 

	Method:
		- Create a mapping of { row:	all candidate values for this row of a particular region }
						and	  { column: all candidate values for this column of particular region }
		- check each value in each mapping to see if appears in the other row candidate values.
		- if the value does not belong to the other mappings, it *must* be found in this row or column of this region 
		- cancel the particular value from all other tiles in same row or column of the other regiones
	
	inputs: (int) curRow - the row of the region we are checking
			(int) curCol - the column of the region we are checking

	outputs: (bool) foundSolution - indicates if we found a cancellation or not
*/

bool Solver::checkPointedTuple(int curRow, int curCol)
{
	bool foundSolution = false;

	unordered_map<int, unordered_set<int>> rowCandidates;
	unordered_map<int, unordered_set<int>> colCandidates;
	//find start of region row
	int regionRow = curRow - (curRow % BOX_SIZE);
	//find start of region column
	int regionCol = curCol - (curCol % BOX_SIZE);
	bool performedCancellation = false;


	for (int curRegionRow = regionRow; curRegionRow < regionRow + BOX_SIZE; curRegionRow++)
	{
		for (int curRegionCol = regionCol; curRegionCol < regionCol + BOX_SIZE; curRegionCol++)
		{
			// for each candidate value in this tile add it to the corresponding row/column set of candidate values
			for (auto candidate : this->board->getTile(curRegionRow, curRegionCol).getCandidateValues())
			{
				rowCandidates[curRegionRow].insert(candidate);
				colCandidates[curRegionCol].insert(candidate);
			}
		}
	}

	//check to see if the rows of the region or the columns of a region have a unique value in a particular column or row
	//and set cancellation flag based on this
	performedCancellation = checkSetsContainsUnique(&rowCandidates, curRow, curCol, "row")
		|| checkSetsContainsUnique(&colCandidates, curRow, curCol, "column");

	return performedCancellation;
}


/*
	This is only a little hacky because, in an attempt to reduce duplicated code, the function takes in a string indicating
	whether or not it is checking sets of rows or sets of columns, then uses this string to perform the appropriate cancellation.

	It would be better if checkSetsContainUnique was unaware of the column/row concept but can't think of a way to handle this without
	duplicating code or a flag.
*/
bool Solver::checkSetsContainsUnique(const unordered_map<int, unordered_set<int>> *pSets, int curRow, int curCol, string type) {

	bool performedCancellation = false;
	//check if any value in a row candidate is not found in the other rows

	vector <unordered_set<int>> vOtherSets;
	unordered_map<int, unordered_set<int>> sets = *pSets;

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

		//for each candidate value in our test set
		for (auto candidate : keyValue.second)
		{
			bool notInOtherSets = true;
			for (auto set : vOtherSets) {
				notInOtherSets = !setContains(set, candidate) && notInOtherSets;

				//if in any of the sets, notInOtherSets = false
				if (!notInOtherSets)
					break;
			}
			if (notInOtherSets)
			{
				if (type == "row")
					performedCancellation = cancelRowSkipSameRegion(candidate, keyValue.first, curCol) || performedCancellation;
				else if (type == "column") {
					performedCancellation = cancelColumnSkipSameRegion(candidate, curRow, keyValue.first) || performedCancellation;
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
	for (auto candidate : currTile.getCandidateValues())
	{
		int rowOrCol = checkForValueInRegion(candidate, currRow, currColumn);
		switch(rowOrCol)	
		{
			case 0:
				foundSolution = cancelRowSkipSameRegion(candidate, currRow, currColumn) || foundSolution;
				break;
			case 1:
				foundSolution = cancelColumnSkipSameRegion(candidate, currRow, currColumn) || foundSolution;
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
2. initializing the otherTile, otherTileActualValue and candidateValuesOfOtherTile in the loops.
	Although I think its hacky it seems neccessary to make it work not sure why.
*/
int Solver::checkForValueInRegion(int currCandidate, int currRow, int currColumn)
{
	int inRowFlag = 0;
	int inColumnFlag = 1;
	int inRegionFlag = -1;

	int validCancelFlag = -1;
	int inRowCounter = 0, inColumnCounter = 0;
	int regionRow = currRow - (currRow % BOX_SIZE);
	int regionCol = currColumn - (currColumn % BOX_SIZE);

	for (int row = regionRow; row < regionRow + BOX_SIZE; row++)
	{
		for (int col = regionCol; col < regionCol + BOX_SIZE; col++)
		{
			Tile otherTile = this->board->getTile(row, col);
			int otherTileActualValue = otherTile.getActualValue();
			unordered_set<int> candidateValuesOfOtherTile = otherTile.getCandidateValues();

			if (isOpenTile(otherTileActualValue))
			{
				if (setContains(candidateValuesOfOtherTile, currCandidate))
				{
					if (col == currColumn && row == currRow)
						continue;
					if (col == currColumn)
						inColumnCounter++;
					else if (row == currRow)
						inRowCounter++;
					else
					{
						validCancelFlag = inRegionFlag;
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
/*
cancelRowSkipSameRegion cancels a candidate value from the column, but skips the region for which the tile belongs

inputs : (int)candidateValue - the value to cancel from the column
		 (int) currRow - the row of the tile region to skip
		 (int) currColumn - the column of the tile region to skip
*/
bool Solver::cancelRowSkipSameRegion(int candidateValue, int currRow, int currColumn)
{
	int regionCol = currColumn - (currColumn % BOX_SIZE);
	bool removedValue = false;

	for (int col = 0; col < BOARD_SIZE; col++)

	{
		if (col >= regionCol && col < regionCol + BOX_SIZE)//isInRegionRow(regionCol, col))
			continue;
		else
		{
			Tile tile = board->getTile(currRow, col);
			if(isOpenTile(tile.getActualValue()))
				removedValue = removeValue(tile, candidateValue) || removedValue;
		}
	}

	return removedValue;
}

/*
	cancelColumnSkipSameRegion cancels a candidate value from the column, but skips the region for which the tile belongs

	inputs: (int) candidateValue - the value to cancel from the column
			(int) currRow - the row of the tile region to skip
			(int) currColumn - the column of the tile region to skip
*/
bool Solver::cancelColumnSkipSameRegion(int candidateValue, int currRow, int currColumn)
{
	int regionRow = currRow - (currRow % BOX_SIZE);
	bool removedValue = false;

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		if (row >= regionRow && row < regionRow + BOX_SIZE)
			continue;
		else
		{
			Tile tile = board->getTile(row, currColumn);
			removedValue =  removeValue(tile, candidateValue) || removedValue;
		}
	}

	return removedValue;
}

/*
	checkForValueMissing checks to see if the supplied tile maintains a candidate value that does not appear in the 
		supplied set.

	If the value is missing, the tile is solved for this value.

	Inputs: (unordered_set<int>) candidateValues - a set of candidate values
			(Tile) tile - the tile for which we are comparing candidate Values

	Outputs: (bool) true if the tile contained a value that was missing from the list
					false if the tile did not contain a value that was missing from the list
*/
bool Solver::checkForValueMissing(unordered_set<int> candidateUnionValues, Tile tile)
{
	if (tile.getCandidateValues().size() > 1) {
		for (auto candidate : tile.getCandidateValues())
		{
			if (!setContains(candidateUnionValues, candidate))
			{
				this->board->clearTileCandidateValues(tile.getRow(), tile.getColumn());
				this->board->addTileCandidateValue(candidate, tile.getRow(), tile.getColumn());
				solveTile(this->board->getTile(tile.getRow(), tile.getColumn()));
				return true;
			}
		}
	}
	return false;
}

bool Solver::isOpenTile(int value)
{
	return (value == -1);
}

bool Solver::removeValue(Tile tile, int value)
{
	bool removedValue = false;
	if (board->getTile(tile.getRow(), tile.getColumn()).getCandidateValues().size() > 1)
	{
		removedValue = board->removeCandidateValue(value, tile.getRow(), tile.getColumn());
		if (board->getTile(tile.getRow(), tile.getColumn()).getCandidateValues().size() == 1)
			solveTile(board->getTile(tile.getRow(), tile.getColumn()));
			//singleValueTiles.push_back(board->getTile(tile.getRow(), tile.getColumn()));
	}

	return removedValue;
}
Board* Solver::getBoard()
{
	return board;
}

void Solver::reset(Board *otherBoard) {
	this->numSolved = 0;
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
