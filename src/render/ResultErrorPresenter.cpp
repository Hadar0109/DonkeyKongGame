#include "ResultErrorPresenter.h"

#include <conio.h>
#include <iostream>

#include "utils.h"

void ResultErrorPresenter::show(const std::string& messageToScreen,
	const std::string& message,
	const std::string& filename,
	size_t iteration,
	int score,
	bool waitForKey)
{
	clearConsole();
	std::cout << "Screen " << filename << '\n';
	std::cout << messageToScreen << '\n';
	std::cout << message << '\n';
	std::cout << "Iteration: " << iteration << '\n';
	std::cout << "You scored: " << score << '\n' << '\n';

	if (waitForKey) 
	{
		std::cout << "Press any key to continue to next screens (if any)" << std::endl;
		_getch();
	}
}

