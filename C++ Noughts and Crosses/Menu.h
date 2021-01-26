#include <string>
#include <vector>
#include <windows.h>

#pragma once
class Menu
{
public:
	// All data relating to a given button in the menu
	struct Option
	{
		#pragma region Struct Management
		Option(Menu* _parent, COORD _cursorPoint, std::string _text, bool _replayGame)
		{
			parent		= _parent;
			cursorPoint = _cursorPoint;
			text		= _text;
			replayGame  = _replayGame;
		}

		~Option()
		{

		}
		#pragma endregion Constructor/Desconstructor

		#pragma region Functions
		void action()
		{
			if (replayGame)
			{
				// Restart game
				parent->setIsMenuRunning(false);
			}
			else
			{
				// End of application
				std::cout << std::endl;

				*parent->appLoopRef = false;

				parent->setIsMenuRunning(false);
			}
		}
		#pragma endregion

		Menu* parent;

		COORD cursorPoint;

		std::string text;

		bool replayGame;

	};


	Menu(bool* _gameLoopRef);
	~Menu();


	void drawButtons();						// Draws all buttons to the console

	void clearScreen();						// Completely clears the console

	void handleInput();						// Handles menu input

	void hightlightOption();				// Highlights the currently selected option


	#pragma region Getters/Setters
	bool getIsMenuRunning();
	void setIsMenuRunning(bool target);
	#pragma endregion

private:

	std::vector<Option*> generatedButtons;

	Option* selectedOption; 
	int selectedOptionIndex;

	bool isRunning;

	bool* appLoopRef;	// A pointer to the variable that handles the main program loop - used to quit the program
};

