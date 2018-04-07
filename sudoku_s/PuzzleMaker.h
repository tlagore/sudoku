#pragma once

#include <random>
#include "Board.h"

#define DEFAULT_ITERATIONS 100

class PuzzleMaker
{
public:
	enum Difficulty { EASY, MEDIUM, HARD };

	PuzzleMaker();
	PuzzleMaker(int iterations);
	PuzzleMaker(int seed, int iterations);
	PuzzleMaker(const PuzzleMaker& rhs);

	Board* generateBoard(Difficulty diff);

	~PuzzleMaker();
private:
	std::default_random_engine _generator;
	int _iterations;
	int _seed;

	void jumblePuzzle(int values[BOARD_SIZE][BOARD_SIZE]);
	void swapRow(int row_a, int row_b, int values[BOARD_SIZE][BOARD_SIZE]);
	void swapCol(int col_a, int col_b, int values[BOARD_SIZE][BOARD_SIZE]);
};

