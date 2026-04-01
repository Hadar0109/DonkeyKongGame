#include "Board.h"

void Board::load(const std::string& filename) 
{
	haveHammer = false;
	haveMario = false;
	havePauline = false;
	fillWall = false;
	countErrors = 0;

	std::ifstream screen_file(filename);

	if (!screen_file.is_open()) 
		std::cerr << "Error: Could not open file " << filename << std::endl;

	int curr_row = 0;
	int curr_col = 0;
	char c;

	while (!screen_file.get(c).eof() && curr_row < (int)Boundaries::MAX_Y) 
	{
		if (c == '\n') 
		{
			handleNewLine(curr_row, curr_col);
			++curr_row;
			curr_col = 0;

			continue;
		}

		bool isNeedDeleted = false;
		if (curr_col < (int)Boundaries::MAX_X) 
		{
			handleSpecialChars(c, curr_row, curr_col);

			if (c == (char)Characters::GHOST) 
			{
				handleGhost(curr_row, curr_col, c);
				isNeedDeleted = true;
			}
			else if (c == (char)Characters::BIG_GHOST) 
			{
				handleGhost(curr_row, curr_col, c);
				isNeedDeleted = true;
			}
			else if (c == (char)Characters::MARIO) 
			{
				handleMario(curr_row, curr_col);
				isNeedDeleted = true;
			} 

			if (!isNeedDeleted)
				originalBoard[curr_row][curr_col++] = c;
			else
				originalBoard[curr_row][curr_col++] = (char)Characters::AIR;
		}
	}

	validateMarioPauline();
	validateRowAndColumn(curr_row, curr_col);

	if (fillWall) 
		addFrame(curr_row);

	if (curr_row < (int)Boundaries::MAX_Y) 
		addDummyRow(curr_row);

	validateGhostPositions();
}

void Board::handleNewLine(int& curr_row, int& curr_col) 
{
	if (curr_col < (int)Boundaries::MAX_X) 
		handleMissingCols(curr_row, curr_col);

	originalBoard[curr_row][(int)Boundaries::MAX_X] = '\n';
}

void Board::handleSpecialChars(char c, int& curr_row, int& curr_col) 
{
	if (c == (char)Characters::PAULINE) 
		handlePauline(curr_row, curr_col);
	else if (c == (char)Characters::DONKEY_KONG) 
		donkeyPos = { curr_col, curr_row };
	else if (c == (char)Characters::WALL) 
		handleWall(curr_row, curr_col);
	else if (c == (char)Characters::LEGEND) 
		legendPos = { curr_col, curr_row };
	else if (c == (char)Characters::HAMMER) 
	{
		haveHammer = true;
		hammerPos = { curr_col, curr_row };
	}
	else if (c != (char)Characters::GHOST &&
		c != (char)Characters::LEGEND &&
		c != (char)Characters::HAMMER &&
		c != (char)Characters::WALL &&
		c != (char)Characters::LADDER &&
		c != (char)Characters::DONKEY_KONG &&
		c != (char)Characters::PAULINE &&
		c != (char)Characters::MARIO &&
		c != (char)Characters::AIR &&
		c != (char)Characters::BIG_GHOST &&
		!isFloorChar(c)) 
	{
		countErrors++;
		errorMessage.push_back("ERROR: there is unknown character in your game: '" + std::string(1, c) + "'!\n"); 
	}
}

void Board::handleMario(int curr_row, int curr_col) 
{
	if (haveMario == 0) 
		marioPos = { curr_col, curr_row };
	else 
	{
		countErrors++;
		errorMessage.push_back("ERROR: there is more than one Mario in your game!\n");
	}

	haveMario++;
}

void Board::handlePauline(int curr_row, int curr_col) 
{
	if (havePauline == 0) 
	{
		paulinePos = { curr_col, curr_row };
		havePauline = true;
	}
	else 
	{
		countErrors++;
		errorMessage.push_back("ERROR: there is more than one Pauline in your game!\n");
	}

	havePauline++;
}


void Board::handleWall(int curr_row, int curr_col) 
{
	if (curr_col == 0 || curr_col == (int)Boundaries::MAX_X || curr_row == 0 || curr_row == (int)Boundaries::MAX_Y) 
		fillWall = true;
}

void Board::handleGhost(int& curr_row, int& curr_col, char type) 
{
	ghostData g;
	g.initPos = { curr_col, curr_row };
	g.ghostType = type;

	ghostsInitPos.push_back(g);
}

void Board::validateGhostPositions() 
{
	if (ghostsInitPos.size() != 0) 
	{
		for (auto& ghostPos : ghostsInitPos) 
		{
			if (!isFloorChar(originalBoard[ghostPos.initPos.getY() + 1][ghostPos.initPos.getX()])) 
			{
				countErrors++;
				errorMessage.push_back("ERROR: ghost isn't standing on a floor!\n");

				break;
			}
		}
	}
}

void Board::validateMarioPauline() 
{
	if (haveMario == 0) 
	{
		countErrors++;
		errorMessage.push_back("ERROR: mario isn't in the game!\n");
	}

	if (havePauline == 0) 
	{
		countErrors++;
		errorMessage.push_back("ERROR: pauline isn't in the game!\n");
	}
}

void Board::validateRowAndColumn(int curr_row, int curr_col) 
{
	if (curr_row == (int)Boundaries::MAX_Y) 
	{
		countErrors++;
		errorMessage.push_back("ERROR: You have exceeded the limits of the screen!\n");
	}

	if (curr_col < (int)Boundaries::MAX_X) 
		handleMissingCols(curr_row, curr_col);
}

void Board::addFrame(int last_row) 
{
	if (legendPos.getY() != 0) 
	{
#pragma warning(suppress : 4996)
		strcpy(originalBoard[0], std::string((int)Boundaries::MAX_X, (char)Characters::WALL).c_str());
#pragma warning(suppress : 4996)
		strcpy(originalBoard[last_row], std::string((int)Boundaries::MAX_X, (char)Characters::WALL).c_str());
		originalBoard[0][(int)Boundaries::MAX_X] = '\n';
		originalBoard[last_row][(int)Boundaries::MAX_X] = '\0';
	}
	else 
	{
#pragma warning(suppress : 4996)
		strcpy(originalBoard[last_row], std::string((int)Boundaries::MAX_X, (char)Characters::WALL).c_str());
		originalBoard[last_row][(int)Boundaries::MAX_X] = '\0';
	}

	for (int row = 1; row < last_row; ++row) 
	{
		originalBoard[row][0] = (char)Characters::WALL;
		originalBoard[row][(int)Boundaries::MAX_X - 1] = (char)Characters::WALL;
	}
}

void Board::addDummyRow(int last_row) 
{
#pragma warning(suppress : 4996)
	strcpy(originalBoard[last_row + 1], std::string((int)Boundaries::MAX_X, (char)Characters::WALL).c_str());
	originalBoard[last_row + 1][(int)Boundaries::MAX_X] = '\0';
}

void Board::handleMissingCols(int row, int col) 
{
#pragma warning(suppress : 4996)
	strcpy(originalBoard[row] + col, std::string((int)Boundaries::MAX_X - col, (char)Characters::AIR).c_str());
}

void Board::reset() 
{
	memcpy(currentBoard, originalBoard, (int)Boundaries::MAX_Y * ((int)Boundaries::MAX_X + 1));
}

bool Board::isValidPosition(const Point& point) const 
{
	char position = getChar(point);

	if (isInBoundaries(point)) 
	{
		if (!isFloorChar(position))
			return true;
	}

	return false;
}

bool Board::isOnFloor(const Point& point) const 
{
	int x = point.getX();
	int y = point.getY() + 1;
	Point p = { x, y };
	char position = getChar(p);

	return (isFloorChar(position));
}

bool Board::isFloorChar(char ch) const 
{
	return (ch == (char)FloorCharacters::LEFT || ch == (char)FloorCharacters::RIGHT || ch == (char)FloorCharacters::PREV || ch == (char)Characters::WALL);
}

bool Board::isInAir(const Point& point) const 
{
	int x = point.getX();
	int y = point.getY() + 1;
	Point charBelow = { x, y };

	return (getChar(charBelow) == (char)Characters::AIR);
}

bool Board::isInBoundaries(const Point& point) const 
{
	int x = point.getX();
	int y = point.getY();

	return ((x < (int)Boundaries::MAX_X - 1 && x > 0) && (y < (int)Boundaries::MAX_Y && y > 0));
}

Point Board::getMarioInitPos() const 
{
	return marioPos;
}

Point Board::getPaulinePos() const 
{
	return paulinePos;
}

Point Board::getDonkeyPos() const 
{
	return donkeyPos;
}

Point Board::getHammerPos() const 
{
	return hammerPos;
}

Point Board::getLegendPos() const 
{
	return legendPos;
}

std::vector<ghostData> Board::getGhostPos() const 
{
	return ghostsInitPos;
}

void Board::clearGhostVec() 
{
	ghostsInitPos.clear();
}

bool Board::isGhostChar(const Point& point) const 
{
	return (getChar(point) == (char)Characters::GHOST);;
}

bool Board::isHaveAHammer() 
{
	return haveHammer;
}

bool Board::isBoardInvalid()  
{
	int sizeErrorMesVec = errorMessage.size();

	if (sizeErrorMesVec > 0) 
	{
		for (const std::string& str : errorMessage) 
		{
			std::cout << str << std::endl;
		}

		std::cout << "Your board is invalid, you are moved to the next board" << std::endl;
		Sleep(3000);
		errorMessage.clear();

		return true;
	}

	return false;
}

void Board::setCurrentBoard(char c) 
{
	currentBoard[hammerPos.getY()][hammerPos.getX()] = c;
}

bool Board::isLadder(const Point& point) const 
{
	return (getChar(point) == (char)Characters::LADDER);
}

