#include "menu.h"

void Menu::displayMenu(MenuType menuType) const 
{
	switch (menuType) 
	{
	case MenuType::MAIN_MENU:
		displayMainMenu();
		break;
	case MenuType::INSTRUCTIONS:
		displayInstructions();
		break;
	case MenuType::PAUSE_MENU:
		displayPauseBoard();
		break;
	default:
		return;
	}
}
