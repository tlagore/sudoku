#pragma once

#include "Tile.h"
#include <iostream>
#define INTIAL_VALUE -1
#define BOARD_SIZE 9
#define REGION_SIZE 3
#define ZERO '0'
class Board
{

public:
	Board();
	Board(const Board &rhs);
	Board(int inputForNewBoard[BOARD_SIZE][BOARD_SIZE]);
	~Board();
	Tile getTile(int row, int column);
	void setTileActualValue(int value, int row, int column);
	void setTileCandidateValues(int row, int col, unordered_set<int>);
	void addTileCandidateValue(int value, int row, int col);
	void clearTileCandidateValues(int row, int col);
	int getTilesLeftToSolve();
	bool isValidState();
	bool isValidRow(Tile tile);
	bool isValidColumn(Tile tile);
	bool isValidRegion(Tile tile);
	void printCandidateValues();
	bool removeCandidateValue(int value, int row, int column);
	bool isSolved();
	void printBoard();

private:
	Tile* board[BOARD_SIZE][BOARD_SIZE];
	int tilesToSolve;
};

