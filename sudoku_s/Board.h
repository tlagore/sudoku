#pragma once

#include "Tile.h"
#include <iostream>
#define INTIAL_VALUE -1
#define BOARD_SIZE 9
#define BOX_SIZE 3
class Board
{

public:
	Board();
	Board(const Board &rhs);
	Board(int inputForNewBoard[BOARD_SIZE][BOARD_SIZE]);
	~Board();
	Tile getTile(int row, int column);
	void setTileActualValue(int value, int row, int column);
	void setTilePossibleValues(int row, int col, unordered_set<int>);
	bool isValidState();
	bool isValidRow(Tile tile);
	bool isValidColumn(Tile tile);
	bool isValidBox(Tile tile);
	void printBoard();

private:
	Tile* board[BOARD_SIZE][BOARD_SIZE];
};

