#include "utils.h"

void gotoxy(int x, int y)
{
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void clearConsole() 
{
    system("cls");
}

void printMissingAuxFileMessage(const char* fileKind, const std::string& filename)
{
    std::cout << "The " << fileKind << " file for " << filename << " does not exist. \nYou are moving to the next screen.";
}

