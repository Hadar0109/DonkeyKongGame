#pragma once

#include <string>

#include "gameConfig.h"

class ResultErrorPresenter 
{
public:
	static void show(const std::string& messageToScreen,
		const std::string& message,
		const std::string& filename,
		size_t iteration,
		int score,
		bool waitForKey);
};

