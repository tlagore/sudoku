#pragma once

#define SQUARE_SIZE 3

class Square
{
public:
	Square() : _aValues{} {}
	Square(int values) : _aValues{ values } {}
	~Square();

	void printSquare();

private:	
	int _aValues[SQUARE_SIZE][SQUARE_SIZE];
};

