#include "stdafx.h"
#include "PuzzleIO.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

#define ZERO '0'

using namespace std;

PuzzleIO::PuzzleIO()
{
}

PuzzleIO::~PuzzleIO()
{
}

void PuzzleIO::readFromFile(string fileName)
{
	string line;
	int value;
	
	int puzzle[PUZZLE_SIZE][PUZZLE_SIZE];

	if (fileExists(fileName)) {
		std::ifstream file(fileName);
		if (file.is_open()) {
			while (getline(file, line)) {
				if (line.length() == PUZZLE_SIZE * PUZZLE_SIZE) {
					generateBoardArray(line, puzzle);
					for (int r = 0; r < PUZZLE_SIZE; r++) {
						for (int c = 0; c < PUZZLE_SIZE; c++) {
							printf("%2d ", puzzle[r][c]);
						}
						cout << endl;
					}
				}
				else if (line.length() == PUZZLE_SIZE) {
					printf("multi line puzzle...\n");
				}
				else {
					printf("WEIRD");
				}
			}
		}
	}
	else {
		throw std::invalid_argument("File does not exist.");
	}
}

bool PuzzleIO::fileExists(string fileName)
{
	std::ifstream file(fileName);
	if (file.good()) {
		file.close();
		return true;
	}

	file.close();
	return false;
}

/*
	Takes in a sudoku board in string format. Currently assumed to be exaclty 81 valid integer characters in length.
	Converts the string into integers, then inserts them into a supplied 9x9 array
*/
void PuzzleIO::generateBoardArray(string line, int puzzle[PUZZLE_SIZE][PUZZLE_SIZE])
{
	int row = 0;
	int column = 0;
	int value;

	for (auto ch : line) {
		value = (int)(ch - ZERO);
		if (value == 0)
			value = -1;

		puzzle[row][column] = value;
		column++;
		if (column == 9) {
			column = 0;
			row++;
		}
	}
}
