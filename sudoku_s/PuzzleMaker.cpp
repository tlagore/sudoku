#include "stdafx.h"

#include <random>
#include <chrono>

#include "PuzzleMaker.h"


PuzzleMaker::PuzzleMaker()
{
	//seed random generator based on time
	this->_iterations = DEFAULT_ITERATIONS;
	this->_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

PuzzleMaker::PuzzleMaker(int iterations) : _iterations{ iterations }
{
	//seed random generator based on time
	this->_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

PuzzleMaker::PuzzleMaker(const PuzzleMaker & rhs) : _seed{rhs._seed}, _iterations{rhs._iterations}
{
}

PuzzleMaker::PuzzleMaker(int seed, int iterations) : _seed{ seed }, _iterations{ iterations }
{
}


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
		board->printBoard();
	}

	return board;
}

void PuzzleMaker::jumblePuzzle(int values[BOARD_SIZE][BOARD_SIZE]) 
{	
	int row_a = -1,
		row_b = -1,
		col_a = -1,
		col_b = -1;
	std::uniform_int_distribution<int> distribution(0, BOARD_SIZE - 1);

	for (int i = 0; i < _iterations; i++)
	{
		row_a = distribution(this->_generator);
		//spin till we find a different row than row_a
		while ((row_b = distribution(this->_generator)) == row_a);

		col_a = distribution(this->_generator);
		//sping till we find different column than col_a
		while ((col_b = distribution(this->_generator)) == col_a);


		//50 50 chance of swapping row before column vs column before row
		if (distribution(this->_generator) % 2 == 0) {
			swapRow(row_a, row_b, values);
			swapCol(col_a, col_b, values);
		}
		else {
			swapCol(col_a, col_b, values);
			swapRow(row_a, row_b, values);
		}
	}
}

void PuzzleMaker::swapRow(int row_a, int row_b, int values[BOARD_SIZE][BOARD_SIZE]) 
{
	int temp;

	//don't swap the same rows
	if (row_a == row_b)
		return;

	for (int i = 0; i < BOARD_SIZE; i++) {
		temp = values[row_a][i];
		values[row_a][i] = values[row_b][i];
		values[row_b][i] = temp;
	}
}

void PuzzleMaker::swapCol(int col_a, int col_b, int values[BOARD_SIZE][BOARD_SIZE])
{
	int temp;

	//don't swap the same rows
	if (col_a == col_b)
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
