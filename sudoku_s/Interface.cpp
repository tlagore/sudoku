#include "stdafx.h"
#include "Interface.h"


Interface::Interface()
{
	this->boards = pio.readFromFile("..\\sudoku.txt");
	this->solver = Solver(boards[0]);
}

Interface::Interface(string fromFile)
{
	boards = pio.readFromFile(fromFile);
}

void Interface::printDifferentBoardPtrs()
{

	for (std::vector<Board*>::iterator it = this->boards.begin(); it != this->boards.end(); ++it) {
		printf("board pointer: %p\n ", *it);
	}
}

bool Interface::mainMenu()
{
	if (boards.size() < 1)
	{
		printf("There are no boards to solve");
		return !isOpen;
	}

	int choice = 0;
	printf("Welcome to Sudoku Solver \n");
	printf("What would you like to do: \n");
	printf("1) Solve Specific Board\n");
	printf("2) Solve all boards\n");
	printf("3) Exit\n");
	cin >> choice;
	cin.clear();

	switch (choice)
	{
		case 1: 
			solveSpecificBoardMenu();
			isOpen = false;
			break;
		case 2:
			solveAllBoards();
			isOpen = false;
			break;
		case 3:
			isOpen = false;
		default:
			printf("Invalid selecion try again");
			break;
	}


	return isOpen;
}

void Interface::solveSpecificBoardMenu()
{
	int boardNum = 0;
	bool isValidInput = false;

	while (!isValidInput)
	{
		printf("Total number of boards: %d \n", boards.size());
		printf("Enter a number between 1 and %d \n", boards.size());
		cin >> boardNum;
		cin.clear();

		if (boardNum <= boards.size() && boardNum >= 1)
		{
			boardNum--;
			isValidInput = true;
		}
	}

	solveSpecificBoard(boardNum);

}

void Interface::solveSpecificBoard(int boardNum)
{
	bool isSolved = false;
	bool validChoice = false;
	this->solver.reset(boards[boardNum]);
	isSolved = this->solver.solve();

	if (isSolved)
	{
		printf("Board Was Solved\n");
		this->solver.getBoard().printBoard();
		printf("Number of solved tiles: %d \n",this->solver.getNumberSolved());
	}
	else
	{
		printf("Failed to solve board\n");
		this->solver.getBoard().printBoard();
		printf("Number of solved tiles: %d \n", this->solver.getNumberSolved());
		
		char choice;
		printf("Print remaining possible values? (y or n)\n");

		while (!validChoice)
		{
			cin >> choice;
			cin.clear();

			choice = tolower(choice);
			switch (choice)
			{
			case 'y':
				//solver.printPossibleValues();
				printf("Add ability to print possible values here \n");
				validChoice = true;
				break;
			case 'n':
				printf("See ya ....\n");
				validChoice = true;
				break;
			default:
				printf("Invalid Choice. Enter y or n \n");
				break;
			}
		}
	}
}

void Interface::solveAllBoards()
{

	bool isSolved = false;
	this->solver.reset(boards[0]);
	isSolved = solver.solve();
	int failedBoards = 0;
	vector<int> boardsFailed;
	vector<int> boardsPassed;
	vector<int> numberSolvedTiles;

	printf("Solving.....\n\n");
	for (int boardNum = 1; boardNum < boards.size(); boardNum++)
	{
		if (isSolved)
		{
			boardsPassed.push_back(boardNum);
		}
		else
		{
			boardsFailed.push_back(boardNum);
			numberSolvedTiles.push_back(solver.getNumberSolved());
		}
		solver.reset(boards[boardNum]);
		isSolved = solver.solve();
	}

	printf("Number of Boards failed: %d\n\n", boardsFailed.size());
	printf("Boards that Failed: \n\n");
	for (int numBoardsFailed = 0; numBoardsFailed < boardsFailed.size(); numBoardsFailed++)
	{
		printf("Board number: %d \n", boardsFailed[numBoardsFailed]);
		printf("Number of Solved Tiles: %d \n\n", numberSolvedTiles[numBoardsFailed]);
	}

}

Interface::~Interface()
{
}
