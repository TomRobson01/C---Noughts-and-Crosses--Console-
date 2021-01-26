#include <iostream>
#include <conio.h>
#include <windows.h>

#include "Game.h"
#include "Menu.h"


int main()
{
	bool running;

	running = true;
	
	while (running)
	{
		#pragma region Game Management
		// Starts a new game
		Game* currentGame = new Game();

		// Main gameplay loop
		while (currentGame->isGameRunning())
		{
			if (_kbhit)
			{
				currentGame->handleInput();
			}
		}
		#pragma endregion

		delete(currentGame);	// Ends the game

		#pragma region End Menu
		// Main menu loop
		Menu* endMenu = new Menu(&running);

		while (endMenu->getIsMenuRunning())
		{
			if (_kbhit)
			{
				endMenu->handleInput();
			}
		}
		#pragma endregion
	}

	return 0;
}