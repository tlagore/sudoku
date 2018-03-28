#pragma once

#include "Tile.h"

#define INTIAL_VALUE -1
#define BOARD_SIZE 9
#define BOX_SIZE 3
class Board
{

public:
	Board();
	Board(int inputForNewBoard[BOARD_SIZE][BOARD_SIZE]);
	~Board();
	Tile getTile(int row, int column);
	void setTileActualValue(int value, int row, int column);
	bool isValidState();
	bool isValidRow(Tile tile);
	bool isValidColumn(Tile tile);
	bool isValidBox(Tile tile);

private:
	Tile* board[BOARD_SIZE][BOARD_SIZE];
};

