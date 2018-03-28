#pragma once

#include "Tile.h"

#define INTIAL_VALUE -1
#define BOARD_SIZE 9
class Board
{

public:
	Board();
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

