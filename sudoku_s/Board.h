#pragma once

#include "Tile.h"

#define BOARD_SIZE 9
class Board
{

public:
	Board();
	~Board();

private:
	Tile _aBoard[BOARD_SIZE][BOARD_SIZE];
};

