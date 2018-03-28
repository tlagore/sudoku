#pragma once

#include "Square.h"

#define BOARD_SIZE 3
class Board
{

public:
	Board();
	~Board();

private:
	Square _aBoard[BOARD_SIZE][BOARD_SIZE];
};

