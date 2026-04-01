#pragma once
#include <iostream>
#include "gameConfig.h"

class Menu
{
	// Fixed ASCII layouts; each string is one console row.
	const char* menuBoard[(int)Boundaries::MAX_Y] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "                                                                                ",//0
		 "                                                                                ",//1
		 "                       ___   ___   _   _ _  _________   __                      ",//2
		 "                      |  _\\ / _ \\ | \\ | | |/ / ____\\ \\ / /                 ",//3
		 "                      | | |/ | | \\|  \\| |   /|  _|  \\ V /                    ",//4
		 "                      | |_|\\ |_| /| |\\  | . \\| |___  | |                     ",//5
		 "                      |___/ \\___/ |_| \\_|_||_|_____| | |                      ",//6
		 "                                                                                ",//7
		 "                             _  _____  _   _  ____                              ",//8
		 "                            | |/ / _ \\| \\ | |/ ___|                           ",//9
		 "                            | ' / | | |  \\| | |  _                             ",//10
		 "                            | . \\ |_| | |\\  | |_| |                           ",//11
		 "                            |_||_\\___/|_| \\_|\\____|                          ",//12
		 "                                                                                ",//13
		 "                                                                                ",//14
		 "                                                                                ",//15
		 "                                                                                ",//16
		 "                            (1) Start a new game                                ",//17
		 "                            (2) Play the game with features and colors          ",//18
		 "                            (3) Choose your screen                              ",//19
		 "                            (8) Present instructions and keys                   ",//20
		 "                            (9) EXIT                                            ",//21
		 "                                                                                ",//22
		 "                                                                                ",//23
		 """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""", // 0
	};

	const char* instructionsBoard[(int)Boundaries::MAX_Y] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"                                                                                ",//1
		"                            Welcome to Donkey Kong!                             ",//2
		"                                                                                ",//3
		"                                 Your mission:                                  ",//4
		"        Help Mario rescue Pauline from Donkey Kong while dodging dangers.       ",//5
		"                 Watch out for barrels and ghosts — avoid them!                 ",//6
		"           Grab the hammer and press 'p' to smash barrels and ghosts!           ",//7
		"                                                                                ",//8
		"                                    Controls:                                   ",//9
		"                                 UP:     w or W                                 ",//10
		"                                 DOWN:   x or X                                 ",//11
		"                                 RIGHT:  d or D                                 ",//12
		"                                 LEFT:   a or A                                 ",//13
		"                                 ATTACK: p or P                                 ",//14
		"                                                                                ",//15
		"                                    Scoring:                                    ",//16
		"                         Reaching Pauline:   100 points                         ",//17
		"                         Smashing a barrel:   10 points                         ",//18
		"                         Smashing a ghost:    20 points                         ",//19
		"                                                                                ",//20
		"                                  Good luck!!                                   ",//21
		"                                                                                ",//22
		"                         To pause the game, press ESC.                          ",//23
		"                   Press any key to return to the main menu.                    ",//24
		"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""",// 0
	};

	const char* pauseBoard[(int)Boundaries::MAX_Y] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "                                                                                ",//0
		 "                                                                                ",//1
		 "                                                                                ",//2
		 "                                                                                ",//3
		 "                                                                                ",//4
		 "                                                                                ",//5
		 "                                                                                ",//6
		 "                     ===================================                        ",//7
		 "                                Game Paused                                     ",//8
		 "                     ===================================                        ",//9
		 "                                                                                ",//10
		 "                          Press ESC again to resume                             ",//11
		 "                                                                                ",//12
		 "                     ===================================                        ",//13
		 "                                                                                ",//14
		 "                                                                                ",//15
		 "                                                                                ",//16
		 "                                                                                ",//17
		 "                                                                                ",//18
		 "                                                                                ",//19
		 "                                                                                ",//20
		 "                                                                                ",//21
		 "                                                                                ",//22
		 "                                                                                ",//23
		 """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""", // 0
	};

	void displayBoard(const char* const* board) const 
	{
		for (int i = 0; i < (int)Boundaries::MAX_Y - 1; i++) {
			std::cout << board[i] << '\n';
		}
		std::cout << board[(int)Boundaries::MAX_Y - 1];
	}

	void displayMainMenu() const 
	{
		displayBoard(menuBoard);
	}

	void displayInstructions() const 
	{
		displayBoard(instructionsBoard);
	}

	void displayPauseBoard() const 
	{
		displayBoard(pauseBoard);
	}

public:
	void displayMenu(MenuType menuType) const;
};
