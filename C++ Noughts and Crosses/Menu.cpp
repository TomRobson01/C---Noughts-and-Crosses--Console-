#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>

#include "Menu.h"


#pragma region Class Management
Menu::Menu(bool* _gameLoopRef)
{
	setIsMenuRunning(true);
	appLoopRef = _gameLoopRef;

	// Generate buttons for the menu
	COORD buttonCursorPos;

	buttonCursorPos.Y = 9;
	buttonCursorPos.X = 7;

	generatedButtons.push_back(new Menu::Option(this, buttonCursorPos, "Replay", true));

	buttonCursorPos.Y = 10;
	buttonCursorPos.X = 7;

	generatedButtons.push_back(new Menu::Option(this, buttonCursorPos, "Quit", false));

	// Draw the menu
	drawButtons();
}

Menu::~Menu()
{
	generatedButtons.clear();
}
#pragma endregion Constructor/Desconstructor


#pragma region Screen Management
// Draws each button and selectes the first
void Menu::drawButtons()
{
	for (int i = 0; i < generatedButtons.size(); i++)
	{
		COORD pos;

		pos.X = 0;
		pos.Y = generatedButtons[i]->cursorPoint.Y;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

		std::cout << generatedButtons[i]->text;
	}

	selectedOptionIndex = 0;
	hightlightOption();
}

void Menu::clearScreen()
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
#pragma endregion

#pragma region Input Management
void Menu::handleInput()
{
	int inputKey;

	inputKey = _getch();

	switch (inputKey)
	{

		
	case 13:	// Enter
		selectedOption->action();
		break;


	#pragma region Arrows
	case 72:	// Up Arrow
		if (selectedOptionIndex > 0)
		{
			selectedOptionIndex--;
			hightlightOption();
		}
		break;

	case 80:	// Down Arrow
		if (selectedOptionIndex < generatedButtons.size() - 1)
		{
			selectedOptionIndex++;
			hightlightOption();
		}
		break;
	#pragma endregion Up, Down, Left, Right Movement

	default:
		break;
	}
}
#pragma endregion

#pragma region  Utilities
void Menu::hightlightOption()
{
	selectedOption = generatedButtons[selectedOptionIndex];

	// Updates cursor position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), selectedOption->cursorPoint);
}
#pragma endregion

#pragma region Getters/Setters
bool Menu::getIsMenuRunning()
{
	return isRunning;
}

void Menu::setIsMenuRunning(bool target)
{
	isRunning = target;
}
#pragma endregion

