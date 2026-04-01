#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "hudRenderer.h"
#include "utils.h"

void HudRenderer::renderLegend(const Point& iconPos, int lives, int score, bool colorModeEnabled) const 
{
	if (colorModeEnabled) 
	{
		for (int i = iconPos.getX() + 2; i < iconPos.getX() + lives + 2; i++) 
		{
			drawHeartIcon(i, iconPos.getY());
		}
	}
	else 
	{
		gotoxy(iconPos.getX() + 2, iconPos.getY());
		std::cout << lives;
	}

	gotoxy(iconPos.getX(), iconPos.getY() + 1);
	std::cout << "SCORE:";
	renderScore(iconPos, score);
}

void HudRenderer::renderScore(const Point& iconPos, int score) const
{
	gotoxy(iconPos.getX() + 7, iconPos.getY() + 1);
	std::cout << score;
}

void HudRenderer::renderArmedStatus(const Point& legendPos) const
{
	gotoxy(legendPos.getX(), legendPos.getY() + 2);
	std::cout << "MARIO IS ARMED";
}

void HudRenderer::drawHeartIcon(int x, int y) const 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x) , static_cast<SHORT>(y) };
	SetConsoleCursorPosition(hConsole, pos);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	WORD originalAttributes = csbi.wAttributes;

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	_setmode(_fileno(stdout), _O_U16TEXT);
	std::wcout << L"\u2665";
	_setmode(_fileno(stdout), _O_TEXT);
	SetConsoleTextAttribute(hConsole, originalAttributes);
}

