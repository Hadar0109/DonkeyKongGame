#pragma once
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <fstream>

#include "gameConfig.h"
#include "point.h"
#include "utils.h"


struct ghostData 
{
	Point initPos;
	char ghostType;
};

class Board 
{
	char originalBoard[(int)Boundaries::MAX_Y][(int)Boundaries::MAX_X + 1];
	char currentBoard[(int)Boundaries::MAX_Y][(int)Boundaries::MAX_X + 1];
	Point marioPos{};
	Point paulinePos{};
	Point donkeyPos{};
	Point hammerPos{};
	Point legendPos{};
	std::vector<ghostData> ghostsInitPos;
	bool haveHammer = false;
	bool fillWall = false;
	int haveMario = 0;
	int havePauline = 0;
	int countErrors = 0;
	std::vector<std::string> errorMessage;
	void handleNewLine(int& curr_row, int& curr_col);
	void handleSpecialChars(char c, int& curr_row, int& curr_col);
	void handleMario(int curr_row, int curr_col);
	void handlePauline(int curr_row, int curr_col);
	void handleWall(int curr_row, int curr_col);
	void handleGhost(int& curr_row, int& curr_col, char type);
	void validateGhostPositions();
	void validateMarioPauline();
	void validateRowAndColumn(int curr_row, int curr_col);
	void addFrame(int last_row);
	void addDummyRow(int last_row);
	void handleMissingCols(int row, int col);

public:
	void load(const std::string& filename);
	void reset();
	bool isValidPosition(const Point& point) const;
	bool isLadder(const Point& point) const;
	bool isOnFloor(const Point& point) const;
	bool isFloorChar(char ch) const;
	bool isInAir(const Point& point) const;
	bool isInBoundaries(const Point& point) const;
	const char* getPrintableBuffer() const { return currentBoard[0]; }
	Point getMarioInitPos() const;
	Point getPaulinePos() const;
	Point getDonkeyPos() const;
	Point getHammerPos() const;
	Point getLegendPos() const;
	std::vector<ghostData> getGhostPos() const;
	char getChar(const Point& point) const 
	{
		return currentBoard[point.getY()][point.getX()];
	}
	void clearGhostVec();
	bool isGhostChar(const Point& point) const;
	bool isHaveAHammer();
	bool isBoardInvalid();
	void setCurrentBoard(char c);
};