#pragma once

#include <random>
#include "Board.h"

#define DEFAULT_ITERATIONS 100
#define REMOVE_SKIP_CAP 25
#define EARLY_STAGE_REMOVAL 35

class PuzzleMaker
{
public:
	enum Difficulty { EASY = 0, MEDIUM = 1, HARD = 2 };

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

	const int DIFFICULTY_LIMT [3]{ 45, 50, 55 };

	void removeTiles(Board * board, Difficulty difficulty);
	void jumblePuzzle(int values[BOARD_SIZE][BOARD_SIZE]);
	void swapRow(int row_a, int row_b, int values[BOARD_SIZE][BOARD_SIZE]);
	void swapCol(int col_a, int col_b, int values[BOARD_SIZE][BOARD_SIZE]);
};

