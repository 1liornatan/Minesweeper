//ID - 208631572
//NAME - LIOR NATAN
//Project Minesweeper Part B

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAINSIZE 22 // Max Game Board Size
#define BOMB 9 // mark 9 as BOMB
typedef struct BombSquare {
	bool Visible; // Square's visiblity
	bool Flag;
	int Value; // Square's Value
} BombSquare;

typedef struct BoardSize {
	int rowSize;
	int colSize;
	int gameBombs;
}BoardSize;

// Functions Declarations
void CalculateNumbers(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // Adds value to Squares around bomb
void PrintLose(); // Prints lose message
void PrintWon(); // Prints win message
void PlantBombs(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // plants bomb randomly on board
BoardSize ModeMenu(); // Game selection menu
bool isValid(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // Checks if Square is valid
bool isEmptySquare(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // square is empty
bool isSafeSquare(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // Square is not a bomb and in game size
void StartGame(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // Starts the game
void ClearField(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // clears board
void PrintTable(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // Print game board
void openEmptyField(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int row, int col);
int openXCells(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int cells); // opens X cells and returns how many squares were revealed
int calculateHidden(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails); // calculates how many hidden squares left
void openSquare(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int row, int line, int HiddenSquares); // open a square with row and line as indexs

// Main
void main() {
	BombSquare DataTable[MAINSIZE][MAINSIZE]; // Creates Board
	BoardSize GameDetails = ModeMenu(); // print game selection menu and return game size
	printf("Welcome to Minesweeper!\n\n");
	if (GameDetails.rowSize != 0 && GameDetails.colSize != 0) // checks if exit game not selected
	{
		ClearField(DataTable, GameDetails); // Clears Board
		PlantBombs(DataTable, GameDetails); // Places bombs randomly around board
		PrintTable(DataTable, GameDetails); // print the game board
		StartGame(DataTable, GameDetails, GameDetails.colSize * GameDetails.rowSize); // Starts The Game
	}
	else
		printf("Exiting Game.");
}

void PrintTable(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // Board Print Function
{
	system("cls"); // clearing screen
	printf("\n   ");
	for (int i = 0; i < GameDetails.colSize; i++) // printing rows numbers
	{
		printf("  %d", i);
		if (i < 10)
			printf(" ");
	}
	printf("\n   _");
	for (int i = 0; i < GameDetails.colSize; i++)
		printf("____");
	printf("\n");
	for (int i = 0; i < GameDetails.rowSize; i++)
	{
		printf("%d ", i); // printing lines numbers
		if (i < 10)
			printf(" ");
		for (int j = 0; j < GameDetails.colSize; j++)
		{
			if (DataTable[i][j].Visible == true)
			{
				if (DataTable[i][j].Value == BOMB)
					printf("| * ");
				else
					if (DataTable[i][j].Value == 0)
						printf("|   ");
					else
						printf("| %d ", DataTable[i][j].Value);
			}
			else if(DataTable[i][j].Flag == true)
				printf("| F ");
			else
				printf("| X ");
		}
		printf("|\n");
	}
	printf("   _");
	for (int i = 0; i < GameDetails.colSize; i++)
		printf("____");
	printf("\n\n");
}

void PrintLose() // lost game
{
	printf("You Hit a Bomb!\nGame is over.\n");
}

void PrintWon() // won game
{
	printf("Congratulations, You won!\n");
}

BoardSize ModeMenu() // printing Game Mode Selection menu
{
	BoardSize GameDetails;
	int gamemode;
	printf("Please choose board size:\n1.8x8\n2.12x12\n3.15x15\n4.Custom size\n\n0.Exit\n");
	scanf("%d", &gamemode);
	switch (gamemode) {
	case 0:
		GameDetails.colSize = 0;
		GameDetails.rowSize = 0;
		break;
	case 1:
		GameDetails.colSize = 8;
		GameDetails.rowSize = 8;
		break;
	case 2:
		GameDetails.colSize = 12;
		GameDetails.rowSize = 12;
		break;
	case 3:
		GameDetails.colSize = 15;
		GameDetails.rowSize = 15;
		break;
	case 4:
		printf("Choose row size and then column size (1-22):\n");
		scanf("%d %d", &GameDetails.rowSize, &GameDetails.colSize);
		if (GameDetails.rowSize < 0 || GameDetails.rowSize > 23 || GameDetails.colSize < 0 || GameDetails.colSize > 23)
		{
			printf("Invalid choise.\n");
			return ModeMenu();
		}
		break;
	default:
		printf("Invalid choice.\n");
		return ModeMenu();
	}
	double rowSize = GameDetails.rowSize * 1.0;
	double colSize = GameDetails.colSize * 1.0;
	GameDetails.gameBombs = (int)(sqrt(rowSize * colSize)); // Calculates How Many Bombs in game
	return GameDetails;
}


void StartGame(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int HiddenSquares) { // starting game function
	printf("Choose a row and then a line:\n");
	int row, line;
	char choice;
	scanf("%d %d", &row, &line);
	if (row == -1 && line > 0) // -1 Option for revealing X bombs
	{
		HiddenSquares -= openXCells(DataTable, GameDetails, line);
		PrintTable(DataTable, GameDetails); // print updates board
		if (HiddenSquares <= GameDetails.gameBombs) // game won
			PrintWon();
		else
			StartGame(DataTable, GameDetails, HiddenSquares);
	}
	else if (row >= 0 && line >= 0 && row < GameDetails.rowSize && line < GameDetails.colSize) // Square selection
	{
		scanf(" %c", &choice);
		if (DataTable[row][line].Visible == false)
		{
			if (choice == 'o' || choice == 'O') // Square open choice
				openSquare(DataTable, GameDetails, row, line, HiddenSquares);
			else if (choice == 'f' || choice == 'F') // Flag choice
			{
				DataTable[row][line].Flag = true;
				PrintTable(DataTable, GameDetails, HiddenSquares);
				StartGame(DataTable, GameDetails, HiddenSquares);
			}
			else
			{
				printf("Please enter a valid choice!\n");
				StartGame(DataTable, GameDetails, HiddenSquares);
			}
		}
		else
		{
			printf("Square already revealed.\n");
			StartGame(DataTable, GameDetails, HiddenSquares);
		}
	}
	else
	{
		printf("Invalid Choice.\n");
		StartGame(DataTable, GameDetails, HiddenSquares);
	}
}

bool isValid(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails)
{
	// Returns true if row number and column number is in range

	if ((row >= 0) && (row < GameDetails.rowSize) && (col >= 0) && (col < GameDetails.colSize))
			return true;
	return false;

}

bool isSafeSquare(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // Square is not bomb and in game size
{
	if (isValid(row, col, DataTable, GameDetails))
		return (DataTable[row][col].Value != BOMB);
	return false;

}
bool isEmptySquare(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // checks if square is empty and hidden
{
	if (isValid(row, col, DataTable, GameDetails))
		if (DataTable[row][col].Value == 0 && DataTable[row][col].Visible == false)
			return true;
	return false;
}
void CalculateNumbers(int row, int col, BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // Add Values to Squares around bomb
{
	if (isSafeSquare(row + 1, col + 1, DataTable, GameDetails) == true) // North East
		DataTable[row + 1][col + 1].Value++;
	if (isSafeSquare(row + 1, col, DataTable, GameDetails) == true) // North
		DataTable[row + 1][col].Value++;
	if (isSafeSquare(row + 1, col - 1, DataTable, GameDetails) == true) // North West
		DataTable[row + 1][col - 1].Value++;
	if (isSafeSquare(row, col + 1, DataTable, GameDetails) == true) // East
		DataTable[row][col + 1].Value++;
	if (isSafeSquare(row, col - 1, DataTable, GameDetails) == true) // West
		DataTable[row][col - 1].Value++;
	if (isSafeSquare(row - 1, col + 1, DataTable, GameDetails) == true) // South East
		DataTable[row - 1][col + 1].Value++;
	if (isSafeSquare(row - 1, col, DataTable, GameDetails) == true) // South
		DataTable[row - 1][col].Value++;
	if (isSafeSquare(row - 1, col - 1, DataTable, GameDetails) == true) // South West
		DataTable[row - 1][col - 1].Value++;
}

void PlantBombs(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // randomly put squares around board based on GAMEDIFFICULTY
{
	int BombsToPlant = GameDetails.gameBombs;
	int row, col;
	srand(time(NULL));
	while (BombsToPlant > 0)
	{
		row = rand() % GameDetails.rowSize;
		col = rand() % GameDetails.colSize;
		if (DataTable[row][col].Value != BOMB)
		{
			DataTable[row][col].Value = BOMB;
			BombsToPlant--;
			CalculateNumbers(row, col, DataTable, GameDetails);
		}
	}
}

void ClearField(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // Prepare board
{
	for (int i = 0; i < GameDetails.rowSize; i++)
		for (int j = 0; j < GameDetails.colSize; j++)
		{
			DataTable[i][j].Value = 0;
			DataTable[i][j].Visible = false;
			DataTable[i][j].Flag = false;
		}
}

void openEmptyField(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int row, int col) // Opens all fields around an empty square
{
	DataTable[row][col].Visible = true;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (isSafeSquare(row - i, col - j, DataTable, GameDetails) && i != j) // square is not a bomb
			{
				if (isEmptySquare(row - i, col - j, DataTable, GameDetails) == false) // number square
					DataTable[row - i][col - j].Visible = true;
				else
					openEmptyField(DataTable, GameDetails, row - i, col - j); // empty square
			}
		}
	}
}

int openXCells(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int cells) // opens X cells and returns Hidden Squares amount
{
	int rowCounter = 0;
	int colCounter = 0;
	int RevealedSquares = 0;
	while (cells > 0 && rowCounter != GameDetails.rowSize && colCounter != GameDetails.colSize) // Reveals Chosen Number of times non bombs squares
	{
		if (DataTable[rowCounter][colCounter].Value != BOMB && DataTable[rowCounter][colCounter].Visible == false)
		{
			DataTable[rowCounter][colCounter].Visible = true;
			RevealedSquares++;
			cells--;
		}
		colCounter++;
		if (colCounter == GameDetails.colSize) // Checks if next square is in the next row
		{
			if (rowCounter == GameDetails.rowSize)
				cells = 0;
			else
			{
				rowCounter++;
				colCounter = 0;
			}
		}
	}
	return RevealedSquares;
}

int calculateHidden(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails) // calculates how many hidden squares left
{
	int HiddenSquares = 0;
	for (int i = 0; i < GameDetails.rowSize; i++)
		for (int j = 0; j < GameDetails.colSize; j++)
			if (DataTable[i][j].Visible == false)
				HiddenSquares++;
	return HiddenSquares;
}

void openSquare(BombSquare DataTable[][MAINSIZE], BoardSize GameDetails, int row, int line, int HiddenSquares) // open a square with row and line as indexs
{
	bool gamelost = false;
	switch (DataTable[row][line].Value)
	{
		case BOMB:
			gamelost = true;
			break;
		case 0:
			openEmptyField(DataTable, GameDetails, row, line);
			HiddenSquares = calculateHidden(DataTable, GameDetails);
			break;
		default:
			DataTable[row][line].Visible = true; // reveal square and update hidden squares left
			HiddenSquares--;
			break;
	}
	if (gamelost) // game lost
	{
		for (int i = 0; i < GameDetails.rowSize; i++)
			for (int j = 0; j < GameDetails.colSize; j++)
				DataTable[i][j].Visible = true;
		PrintTable(DataTable, GameDetails);
		PrintLose();
	}
	else
	{
		PrintTable(DataTable, GameDetails); // print updated board
		if (HiddenSquares <= GameDetails.gameBombs) // game won
			PrintWon();
		else // continue game
			StartGame(DataTable, GameDetails, HiddenSquares);
	}
}