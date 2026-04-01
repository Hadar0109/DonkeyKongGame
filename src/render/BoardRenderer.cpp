#include <iostream>

#include "BoardRenderer.h"
#include "board.h"
#include "utils.h"

void BoardRenderer::print(const Board& board) 
{
	clearConsole();
	std::cout << board.getPrintableBuffer();
}

