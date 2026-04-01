#pragma once

#include <windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>

void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void clearConsole();
void printMissingAuxFileMessage(const char* fileKind, const std::string& filename);
