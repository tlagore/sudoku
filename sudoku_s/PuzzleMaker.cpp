#include "stdafx.h"

#include <random>
#include <chrono>

#include "PuzzleMaker.h"
#include "Solver.h"


/*
	Default constructor, 
*/
PuzzleMaker::PuzzleMaker()
{
	//seed random generator based on time
	this->_iterations = DEFAULT_ITERATIONS;
	this->_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

//overloaded constructor, takes in number of iterations to perform on the puzzle
PuzzleMaker::PuzzleMaker(int iterations) : _iterations{ iterations }
{
	//seed random generator based on time
	this->_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

//copy constructor
PuzzleMaker::PuzzleMaker(const PuzzleMaker & rhs) : _seed{rhs._seed}, _iterations{rhs._iterations}
{
	this->_generator.seed(_seed);
}

//overload constructor, takes in a specified seed and iterations
PuzzleMaker::PuzzleMaker(int seed, int iterations) : _seed{ seed }, _iterations{ iterations }
{
	this->_generator.seed(_seed);
}

/*
	generateBoard generates a randomized board based on a difficulty.

	Currently, generateBoard simply returns the solved board.

	TODO: Remove values based on the difficulty handed in.
*/
Board * PuzzleMaker::generateBoard(Difficulty diff)
{
	Board *board = nullptr;
	int defaultValues[BOARD_SIZE][BOARD_SIZE] =
	{
		{ 1,2,3,4,5,6,7,8,9 },
		{ 4,5,6,7,8,9,1,2,3 },
		{ 7,8,9,1,2,3,4,5,6 },

		{ 2,3,4,5,6,7,8,9,1 },
		{ 5,6,7,8,9,1,2,3,4 },
		{ 8,9,1,2,3,4,5,6,7 },

		{ 3,4,5,6,7,8,9,1,2 },
		{ 6,7,8,9,1,2,3,4,5 },
		{ 9,1,2,3,4,5,6,7,8 }
	};

	jumblePuzzle(defaultValues);

	board = new Board(defaultValues);

	if (!(board->isValidState())) 
	{
		printf("Something went wrong in puzzle maker, generated an invalid board...\n");
		delete board;
		board = nullptr;
	}
	else {
		removeTiles(board, diff);
	}

	return board;
}

/*
	removeTiles takes in a board and difficulty and randomly removes tiles.

	The number of tiles removed is based on the difficulty.

	removeTiles does not guarantee that the tiles removed will match the difficulty handed into the function,
	but does guarantee that it will not remove MORE than the passed in difficulty.

	Current difficulty is 
		easy = remove (up to) 45 tiles
		medium = remove (up to) 50 tiles
		hard = remove (up to) 55 tiles

	removeTiles will halt early if a full pass of the puzzle is done and no tile was able to be removed while keeping the puzzle
	in a solvable state. This is limited by the Solver classes ability to solve sudokus
*/
void PuzzleMaker::removeTiles(Board * board, Difficulty difficulty)
{
	int limit = DIFFICULTY_LIMT[difficulty];
	bool removedElement;
	int attemptedRemoveValue;
	int numSkipped;
	Solver solver;

	std::uniform_int_distribution<int> fifty_fifty(0, 1);
		
	do
	{
		removedElement = false;
		numSkipped = 0;

		for (int row = 0; row < BOARD_SIZE; row++)
		{
			for (int column = 0; column < BOARD_SIZE; column++)
			{
				if (board->getTile(row, column).getActualValue() != UNSOLVED) {
					//if we're relatively early in the removal process, randomize if we actually attempt to remove 
					//the tile or not. Don't skip if we've skipped more than skip cap and haven't removed an element yet
					if (limit > EARLY_STAGE_REMOVAL && (numSkipped < REMOVE_SKIP_CAP || removedElement)) {
						if (fifty_fifty(this->_generator) == 0) {
							numSkipped++;
							continue;
						}
					}

					//preserve tile value that we attempt to remove
					attemptedRemoveValue = board->getTile(row, column).getActualValue();
					board->setTileActualValue(UNSOLVED, row, column);
					solver.reset(board);

					//if our solver fails to solve the board with the tile removed, put the tile back
					if (!solver.solve())
						board->setTileActualValue(attemptedRemoveValue, row, column);
					else
					{
						//flag that we were able to remove something on this pass
						removedElement = true;
						limit--;
					}
				}

			}
		}
	} while (limit != 0 && removedElement);
	
}

/*

	jumblePuzzle takes in an array of values assumed to be a valid sudoku board.

	The default supplied valid sudoku board is used as a starting point, then the 
	method randomly swaps rows and columns within the same region based on the number of iterations provided
	in the constructor (default of 100 iterations - which seems a bit low).

	This takes advantage of the fact that for a valid sudoku board, swapping any row or column within the same region
	will still result in a valid sudoku board.
*/
void PuzzleMaker::jumblePuzzle(int values[BOARD_SIZE][BOARD_SIZE]) 
{	
	int row_a = -1,
		row_b = -1,
		col_a = -1,
		col_b = -1;
	int row_region = -1,
		col_region = -1;
	std::uniform_int_distribution<int> region_distribution(0, REGION_SIZE - 1);

	//random generator for generating a 50/50 chacne
	std::uniform_int_distribution<int> fifty_fifty(0, 1);

	for (int i = 0; i < _iterations; i++)
	{

		//dont care if we're swapping the same row/col region
		row_region = region_distribution(this->_generator);
		col_region = region_distribution(this->_generator);

		//(row_region * REGION_SIZE) gives us the start point of the actual region we've selected
		row_a = (row_region * REGION_SIZE) + region_distribution(this->_generator);
		//but spin till we find a different row than row_a
		while ((row_b = (row_region * REGION_SIZE) + region_distribution(this->_generator)) == row_a);

		col_a = (col_region * REGION_SIZE) + region_distribution(this->_generator);
		//sping till we find different column than col_a
		while ((col_b = (col_region * REGION_SIZE) + region_distribution(this->_generator)) == col_a);

		//50 50 chance of swapping row before column vs column before row
		if (fifty_fifty(this->_generator) == 0) {
			swapRow(row_a, row_b, values);
			swapCol(col_a, col_b, values);
		}
		else {
			swapCol(col_a, col_b, values);
			swapRow(row_a, row_b, values);
		}
	}
}

/*
	swapRow takes in two rows and an array of values and swaps the values of the two rows
*/
void PuzzleMaker::swapRow(int row_a, int row_b, int values[BOARD_SIZE][BOARD_SIZE]) 
{
	int temp;

	//don't swap the same rows && ensure rows are valid
	if (row_a == row_b || (row_a < 0 || row_a > BOARD_SIZE) || (row_b < 0 || row_b > BOARD_SIZE))
		return;

	for (int i = 0; i < BOARD_SIZE; i++) {
		temp = values[row_a][i];
		values[row_a][i] = values[row_b][i];
		values[row_b][i] = temp;
	}
}

/*
	swapCol takes in two columns and an array of values and swaps the values of the two columns
*/
void PuzzleMaker::swapCol(int col_a, int col_b, int values[BOARD_SIZE][BOARD_SIZE])
{
	int temp;

	//don't swap the same columns & ensure columns are valid
	if (col_a == col_b || (col_a < 0 || col_a > BOARD_SIZE) || (col_b < 0 || col_b > BOARD_SIZE))
		return;

	for (int i = 0; i < BOARD_SIZE; i++) {
		temp = values[i][col_a];
		values[i][col_a] = values[i][col_b];
		values[i][col_b] = temp;
	}
}


PuzzleMaker::~PuzzleMaker()
{
}
