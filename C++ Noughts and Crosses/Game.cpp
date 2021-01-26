#pragma region Inclusions
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <time.h>

#include "Game.h"
#pragma endregion

#pragma region Definitions
#define PLAYERCOLOUR 11
#define ENEMYCOLOUR 12
#define GRIDCOLOUR 15
#pragma endregion


#pragma region Class Management 
Game::Game()
{
	#pragma region Constructor
	// Prepare the grid for a fresh game
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 3; y++)
		{
			grid[x][y] = ' ';
		}
	}

	// Reset the selection
	selectionPos.X = 0;
	selectionPos.Y = 0;

	gridSelectionPos.X = 0;
	gridSelectionPos.Y = 0;


	// Set basic game state
	turn = CurrentTurn::Player;
	winner = Winner::Stalemate;

	gameRunning = true;
	gameWon = false;


	// Seed the random elements
	srand(time(NULL));


	// Draw the grid and begin the game
	drawGame();

	#pragma endregion 
}

Game::~Game()
{
	#pragma region Deconstructor

	// Moves the cursor below the grid
	COORD resetPos;

	resetPos.X = 0;
	resetPos.Y = 6;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), resetPos);

	#pragma region Winner Message

	switch (winner)
	{
	case Winner::PlayerWin:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | PLAYERCOLOUR));
		std::cout << "Game over - Player Wins!" << std::endl << std::endl;
		break;


	case Winner::CPUWin:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | ENEMYCOLOUR));
		std::cout << "Game over - CPU Wins!" << std::endl << std::endl;
		break;


	case Winner::Stalemate:
		std::cout << "Game over - Stalemate!" << std::endl << std::endl;
		break;


	default:
		std::cout << "Game over!" << std::endl << std::endl;
		break;

	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | GRIDCOLOUR));
	#pragma endregion

	#pragma endregion
}
#pragma endregion Constructor & Deconstructor


#pragma region Screen Management
void Game::drawGame()
{
	#pragma region Draw Grid
	// Prepare a fresh screen
	clearScreen();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | GRIDCOLOUR));


	// Draw game view
	for (size_t y = 0; y < 3; y++)
	{
		std::string lineOut;

		for (size_t x = 0; x < 3; x++)
		{
			// For each column on a given grid, 
			// add the value of that tile, 
			// if it's not the end then follow it with a |
			lineOut += grid[x][y];

			if (x != 2)
			{
				lineOut += '|';
			}
		}

		std::cout << lineOut << std::endl;

		if (y != 2)
		{
			std::cout << "-----" << std::endl;
		}
	}

	// Return the cursor to the player's last position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), selectionPos);


	// Check for end games
	checkWin('X', 'O');
	checkWin('O', 'X');

	stalemateCheck();
	#pragma endregion
}

void Game::clearScreen()
{
	#pragma region Clear Screen
	// Clear game view
	for (int x = 0; x < 120; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			COORD pos;

			pos.X = x;
			pos.Y = y;
			
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			std::cout << " ";
		}
	}


	// Return the cursor to origin
	COORD resetPos;

	resetPos.X = 0;
	resetPos.Y = 0;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), resetPos);
	#pragma endregion
}
#pragma endregion Draw Grid & Clear Screen


#pragma region Input
void Game::handleInput()
{
	int inputKey;

	inputKey = _getch();

	switch (inputKey)
	{

	// Enter
	case 13:	
		// If the player has selected an empty space on their turn, claim it 
		// Then move to the enemy turn
		if (turn == CurrentTurn::Player && grid[gridSelectionPos.X][gridSelectionPos.Y] == ' ')
		{
			grid[gridSelectionPos.X][gridSelectionPos.Y] = 'X';
			drawGame();

			if (gameRunning)
			{
				turn = CurrentTurn::CPU;
				enemyAI();
			}
		}
		break;


	#pragma region Arrows
	case 72:	// Up Arrow
		if (selectionPos.Y != 0)
		{
			selectionPos.Y -= 2;
			gridSelectionPos.Y--;
		}
		break;

	case 75:	// Left Arrow
		if (selectionPos.X != 0)
		{
			selectionPos.X -= 2;
			gridSelectionPos.X--;
		}
		break;

	case 77:	// Right Arrow
		if (selectionPos.X != 4)
		{
			selectionPos.X += 2;
			gridSelectionPos.X++;
		}
		break;

	case 80:	// Down Arrow
		if (selectionPos.Y != 4)
		{
			selectionPos.Y += 2;
			gridSelectionPos.Y++;
		}
		break;
	#pragma endregion Up, Down, Left, Right Movement

	default:
		break;
	}

	// Updates cursor position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), selectionPos);

}
#pragma endregion Player Input


#pragma region AI
void Game::enemyAI()
{

	bool selectionFound;
	selectionFound = false;

	COORD aiSelectionPoint;
	aiSelectionPoint.X = 0;
	aiSelectionPoint.Y = 0;


	#pragma region Win Check
	// Check for any rows where the AI is one move from winning
	std::vector<std::vector<COORD>> gridAnalysis = getRowsOfLength(2, 'O', 'X');

	for (int r = 0; r < gridAnalysis.size(); r++)
	{
		for (int t = 0; t < gridAnalysis[r].size(); t++)
		{
			if (grid[gridAnalysis[r][t].X][gridAnalysis[r][t].Y] == ' ')
			{
				aiSelectionPoint.X = gridAnalysis[r][t].X;
				aiSelectionPoint.Y = gridAnalysis[r][t].Y;

				selectionFound = true;
				break;
			}
		}

		if (selectionFound)
		{
			break;
		}
	}
	#pragma endregion

	#pragma region Block Player Check
	if (!selectionFound)
	{
		// Check for any rows where the Player is one move from winning
		gridAnalysis = getRowsOfLength(2, 'X', 'O');

		for (int r = 0; r < gridAnalysis.size(); r++)
		{
			for (int t = 0; t < gridAnalysis[r].size(); t++)
			{
				if (grid[gridAnalysis[r][t].X][gridAnalysis[r][t].Y] == ' ')
				{
					aiSelectionPoint.X = gridAnalysis[r][t].X;
					aiSelectionPoint.Y = gridAnalysis[r][t].Y;

					selectionFound = true;
					break;
				}
			}

			if (selectionFound)
			{
				break;
			}
		}
	}
	#pragma endregion

	#pragma region Continue Line
	if (!selectionFound)
	{
		// Check for any rows where the AI already has a presence
		gridAnalysis = getRowsOfLength(1, 'O', 'X');

		for (int r = 0; r < gridAnalysis.size(); r++)
		{
			for (int t = 0; t < gridAnalysis[r].size(); t++)
			{
				if (grid[gridAnalysis[r][t].X][gridAnalysis[r][t].Y] == ' ')
				{
					aiSelectionPoint.X = gridAnalysis[r][t].X;
					aiSelectionPoint.Y = gridAnalysis[r][t].Y;

					selectionFound = true;
					break;
				}
			}

			if (selectionFound)
			{
				break;
			}
		}
	}	
	#pragma endregion

	#pragma region Fallback
	// If all other cases aren't met, choose somewhere random
	while (!selectionFound)
	{

		aiSelectionPoint.X = rand() % 3;
		aiSelectionPoint.Y = rand() % 3;

		if (grid[aiSelectionPoint.X][aiSelectionPoint.Y] == ' ')
		{
			selectionFound = true;
		}
	}	
	#pragma endregion


	// Claim the selected tile
	grid[aiSelectionPoint.X][aiSelectionPoint.Y] = 'O';

	// Updates the board, and returns control to the player
	drawGame();
	turn = CurrentTurn::Player;
}
#pragma endregion Enemy AI Handling


#pragma region Game Management
void Game::checkWin(char charCheck, char hostileChar)
{
	
	if (gameRunning)
	{
		// If there are no complete rows, the game keeps running
		gameRunning = getRowsOfLength(3, charCheck, hostileChar).size() == 0;
	}

	if (!gameRunning && !gameWon)
	{
		switch (charCheck)
		{
		case 'X':
			winner = Winner::PlayerWin;
			break;

		case 'O':
			winner = Winner::CPUWin;
			break;

		default:
			winner = Winner::Stalemate;
			break;
		}

		gameWon = true;
	}
}

void Game::stalemateCheck()
{
	// Checks each tile in the grid
	// If the contain character is not ' ', add it to a vector
	// If that vector contains the whole grid, it's a stalemate
	
	if (!gameWon)
	{
		std::vector<bool> slotsFilled;
		for (size_t y = 0; y < 3; y++)
		{

			for (size_t x = 0; x < 3; x++)
			{
				if (grid[x][y] != ' ')
				{
					slotsFilled.push_back(true);
				}
			}

			if (slotsFilled.size() == 9)
			{
				winner = Winner::Stalemate;
				gameRunning = false;
			}
		}
	}
}
#pragma endregion End-Game Checks


#pragma region Utilities
bool Game::isGameRunning()
{
	return gameRunning;
}


std::vector<std::vector<COORD>> Game::getRowsOfLength(int length, char teamChar, char enemyChar)
{
	std::vector<std::vector<COORD>> rows;


	#pragma region H. Check
	for (int y = 0; y < 3; y++)
	{
		std::vector<COORD> rowData;
		int concecutiveChars;
		concecutiveChars = 0;
		bool endReached;
		endReached = false;

		for (int x = 0; x < 3; x++)
		{
			if ((grid[x][y] == teamChar))
			{
				concecutiveChars++;
			}

			if (grid[x][y] != enemyChar)
			{
				COORD currentTileID;
				currentTileID.X = x;
				currentTileID.Y = y;

				rowData.push_back(currentTileID);
			}

			if (concecutiveChars == length)
			{
				endReached = true;
			}
		}

		if (endReached)
		{
			rows.push_back(rowData);
		}
	}
	#pragma endregion Horizontal Check

	#pragma region V. Check
	for (int x = 0; x < 3; x++)
	{
		std::vector<COORD> rowData;
		int concecutiveChars;
		concecutiveChars = 0;
		bool endReached;
		endReached = false;

		for (int y = 0; y < 3; y++)
		{
			if (grid[x][y] == teamChar)
			{
				concecutiveChars++;
			}

			if (grid[x][y] != enemyChar)
			{
				COORD currentTileID;
				currentTileID.X = x;
				currentTileID.Y = y;

				rowData.push_back(currentTileID);
			}

			if (concecutiveChars == length)
			{
				endReached = true;
			}
		}

		if (endReached)
		{
			rows.push_back(rowData);
		}
	}
	#pragma endregion Vertical Check

	#pragma region Diag. Check
	std::vector<COORD> diagonalLeftData;

	int concecutiveChars;
	concecutiveChars = 0;
	bool endReached;
	endReached = false;

	for (int i = 0; i < 3; i++)
	{
		if (grid[i][i] == teamChar)
		{
			concecutiveChars++;
		}

		if (grid[i][i] != enemyChar)
		{
			COORD currentTileID;
			currentTileID.X = i;
			currentTileID.Y = i;

			diagonalLeftData.push_back(currentTileID);
		}

		if (concecutiveChars == length)
		{
			endReached = true;
		}
	}

	if (endReached)
	{
		rows.push_back(diagonalLeftData);
	}

	concecutiveChars = 0;
	#pragma endregion Diagonal Check - Left

	#pragma region Diag. Check

	std::vector<COORD> diagonalRightData;
	endReached = false;


	for (int i = 0; i < 3; i++)
	{
		if (grid[2 - i][i] == teamChar)
		{
			concecutiveChars++;
		}

		if (grid[2 - i][i] != enemyChar)
		{
			COORD currentTileID;
			currentTileID.X = 2 - i;
			currentTileID.Y = i;

			diagonalRightData.push_back(currentTileID);
		}

		if (concecutiveChars == length)
		{
			endReached = true;
		}
	}

	if (endReached)
	{
		rows.push_back(diagonalRightData);
	}
	#pragma endregion Diagonal Check - Right


	return rows;
}
#pragma endregion Misc. Utilities
