#include <string>
#include <vector>
#include <windows.h>

#pragma once
class Game
{
public:

	Game();
	~Game();

	void drawGame();					// Draws the current board state

	void clearScreen();					// Completely wipes the console

	void handleInput();					// Handles player input

	void enemyAI();						// Handles enemy AI

	bool isGameRunning();				// Gets the value of 'gameRunning'

	void stalemateCheck();				// Checks if the game is in a stalemate (nobody can make a move)


	void checkWin(char charCheck, char hostileChar);		// Checks if a given player has won

	std::vector<std::vector<COORD>> getRowsOfLength(int length, char teamChar, char enemyChar);		// Returns all rows containing a given character x number of times


private:

	enum class CurrentTurn
	{
		None,
		Player,
		CPU
	};

	enum class Winner
	{
		PlayerWin,
		CPUWin,
		Stalemate
	};


	char grid[3][3];


	COORD selectionPos;			// Where the cursor will appear on the screen
	COORD gridSelectionPos;		// The currently selected index in the grid

	CurrentTurn turn;
	Winner winner;

	bool gameRunning;
	bool gameWon;

};

