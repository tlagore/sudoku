#include "stdafx.h"
#include "Square.h"



void Square::printSquare() {
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		for (int j = 0; j < SQUARE_SIZE - 1; j++) {
			printf("%d, ", _aValues[i][j]);
		}
		printf("%d\n", _aValues[i][SQUARE_SIZE - 1]);
	}
}

Square::~Square()
{
}
