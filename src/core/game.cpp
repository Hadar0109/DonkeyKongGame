#include "game.h"
#include "BoardRenderer.h"

void Game::run() 
{
	mode->run(*this);
}

void Game::playGame() 
{
	for (int i = fileNumber; i < fileNames.size(); ++i) 
	{
		if (i == fileNames.size() - 1)
			isLastScreen = true;

		setFileNames(i);
		mode->initializeTypeGame(*this);

		if (!isGoodStepsFile || !isGoodResultFile) 
		{
			if (i != fileNames.size() - 1)
				++fileNumber;
			else
				isPlaying = false;

			break;
		}

		srand(random_seed);
		board.clearGhostVec();
		board.load(fileNames[i]);

		if (board.isBoardInvalid()) 
		{
			clearConsole();
			continue;
		}

		resetBoardAndHero();
		resetGameState();

		if (!isSilent) printLegend();

		ghostsVec.clear();
		createVecGhosts();

		while (true) 
		{
			isFileValid = true;
			++iteration;
			nextLifeLostEvent = 0;
			mode->setNextIterationEvents(*this);
			mode->handleUserInput(*this);
			createBarrel();
			moveBarrels();
			moveGhosts();
			checkHeroFall();
			moveHero(prevKey);

			if (marioHitBarrel || marioHitGhost || marioFallDeadly || marioNearExplosion) 
			{
				marioHitBarrel = false; marioHitGhost = false; marioFallDeadly = false; marioNearExplosion = false;
				mode->marioLossLife(*this);
				if (!isFileValid) break;
			}
			else 
			{
				mode->validateLifeLoss(*this);
				if (!isFileValid) break;
			}

			updateMarioArmedState();

			if (hero.getPosition() == board.getPaulinePos()) 
			{
				score += 100;
				mode->marioMeetsPauline(*this);

				if (!isFileValid) break;

				if (isLastScreen) 
				{
					isArmed = false;
					mode->handleGameEnd(*this, true);
				}

				break;
			}
			else 
			{
				mode->validatePaulineMeeting(*this);
				if (!isFileValid) break;
			}

			mode->checkResultsState(*this);

			if (!isFileValid) break;

			if (heroLives == 0) 
			{
				isArmed = false;
				mode->handleGameEnd(*this, false);
				break;
			}
		}

		mode->completeProcess(*this);
		results.clearResultsList();
		steps.clearStepsList();
		iteration = 0;
		nextPaulineEvent = 0;

		if (isLastScreen) break;

		++fileNumber;
	}
}

void Game::resetBoardAndHero() 
{
	board.reset();

	if (!isSilent)
		BoardRenderer::print(board);

	hero.setPosition(board.getMarioInitPos());
	hero.setBoard(board);
	hero.setDir(Keys::STAY);
}

void Game::printLegend() 
{
	hud.renderLegend(initIconPos, heroLives, score, colorModeEnabled);
}

void Game::handleHammerUsage() 
{
	Point nextPos;

	if (prevKey == (char)Keys::LEFT)
		nextPos = { hammerPos.getX() - 1, hammerPos.getY() };
	else 
		nextPos = { hammerPos.getX() + 1, hammerPos.getY() };

	killGhostOrBarrel(nextPos);
}

void Game::killGhostOrBarrel(Point position) 
{
	for (int i = 0; i < ghostsVec.size(); i++) 
	{
		if (ghostsVec[i]->getPosition() == position && ghostsVec[i]->getActiveStatus()) 
		{
			ghostsVec[i]->explosion();
			score += 20;
		}
	}
	for (int i = 0; i < barrelVec.size(); i++) 
	{
		if (barrelVec[i].getPosition() == position && barrelVec[i].getActiveStatus()) 
		{
			barrelVec[i].explosion();
			score += 10;
		}
	}
	printScore();
}

/* Initializes the vector of Ghost objects by reading positions from the board
   and setting up each ghost's position, direction, and board. */
void Game::createVecGhosts() 
{
	std::vector<ghostData> ghostsData = board.getGhostPos();

	for (int i = 0; i < ghostsData.size(); i++) 
	{
		if (ghostsData[i].ghostType == (char)Characters::GHOST) 
		{
			auto g = std::make_unique<Ghost>();
			g->setPosition(ghostsData[i].initPos);
			g->setBoard(board);
			ghostsVec.push_back(std::move(g));
		}
		else 
		{
			auto g = std::make_unique<ClimbingGhost>();
			g->setPosition(ghostsData[i].initPos);
			g->setBoard(board);
			ghostsVec.push_back(std::move(g));
		}
	}
}

void Game::createBarrel() 
{
	int logSize = barrelVec.size();

	if (iteration % 30 == 0) 
	{
		Barrel b;
		b.setPosition(board.getDonkeyPos());
		barrelVec.push_back(b);
		barrelVec[logSize].setBoard(board);

		if (!isSilent)
			barrelVec[logSize].draw();
	}
}

bool Game::updateSingleBarrel(int index) 
{
	if (barrelVec[index].getActiveStatus()) 
	{
		barrelVec[index].setDir();

		if (handleMarioCollisionWithBarrel(index)) 
			return true;
		
		if (!isSilent) 
		{
			barrelVec[index].erase();
			barrelVec[index].move();
			barrelVec[index].draw();
		}
		else 
			barrelVec[index].move();
		
		if (handleMarioCollisionWithBarrel(index)) 
			return true;
	}
	else if (!barrelVec[index].isBarrelExplode())
	{
		barrelVec[index].explosion();
		handleMarioIsInExplosionRange(index);
	}

	return false;
}

void Game::moveBarrels() 
{
	for (int i = 0; i < barrelVec.size(); i++) 
	{
		if (updateSingleBarrel(i))
			return;
	}
}

bool Game::updateSingleGhost(int index) 
{
	if (ghostsVec[index]->getActiveStatus()) 
	{
		handleMarioCollisionWithGhost(index);
		checkGhostCollision(index);

		if (!isSilent)
			ghostsVec[index]->erase();

		ghostsVec[index]->move();
		handleMarioCollisionWithGhost(index);
		ghostsVec[index]->setCollision(false);

		if (!isSilent)
			ghostsVec[index]->draw();
	}

	return false;
}

void Game::moveGhosts() 
{
	for (int i = 0; i < ghostsVec.size(); i++) 
	{
		if (updateSingleGhost(i))
			return;
	}
}

void Game::checkGhostCollision(int index) 
{
	for (int j = 0; j < ghostsVec.size(); j++) 
	{
		if (j == index) 
			continue;
		
		if (ghostsVec[index]->getPosition().getY() == ghostsVec[j]->getPosition().getY())
		{
			if (ghostsVec[index]->getPosition().getX() + ghostsVec[index]->getDirX() == ghostsVec[j]->getPosition().getX()) 
			{
				ghostsVec[index]->setCollision(true);
				ghostsVec[j]->setCollision(true);
			}
		}
	}
}

bool Game::handleMarioCollisionWithGhost(int index) 
{
	Point marioPos = hero.getPosition();
	Point ghostPos = ghostsVec[index]->getPosition();

	if (marioPos == ghostPos) {
		decreaseLivesAndRestart();
		marioHitGhost = true;

		return true;
	}
	else 
		return false;
}

void Game::checkHeroFall() 
{
	if (hero.isFallDeadly())
	{
		marioFallDeadly = true;
		decreaseLivesAndRestart();
	}
}

void Game::moveHero(char key) 
{
	if (!isSilent)
		hero.erase();

	if (isArmed) 
	{
		if (!isSilent)
			deletePrevHammer();

		moveWithHammer((char)Characters::AIR, key);
	}

	hero.move();

	if (!isSilent)
		hero.draw();

	if (isArmed) 
		moveWithHammer((char)Characters::HAMMER, key);
}

void Game::moveWithHammer(char c, char key) 
{
	if (key == (char)Keys::RIGHT) 
	{
		hammerPos = { hero.getPosition().getX() + 1, hero.getPosition().getY() };

		if (hammerPos.getX() == (int)Boundaries::MAX_X - 1) 
			hammerPos = { hero.getPosition().getX() - 1, hero.getPosition().getY() };
		
	}
	else if (key == (char)Keys::LEFT) 
	{
		hammerPos = { hero.getPosition().getX() - 1, hero.getPosition().getY() };

		if (hammerPos.getX() == 0) 
			hammerPos = { hero.getPosition().getX() + 1, hero.getPosition().getY() };
	}

	if (!isSilent) 
	{
		gotoxy(hammerPos.getX(), hammerPos.getY());

		if (c == (char)Characters::AIR) 
		{
			char backgroundChar = board.getChar(hammerPos);
			std::cout << backgroundChar;
		}
		else if (board.isInBoundaries(hammerPos)) 
			std::cout << c;
	}
}

void Game::deletePrevHammer() 
{
	gotoxy(hammerPos.getX(), hammerPos.getY());
	char backgroundChar = board.getChar(hammerPos);

	if (backgroundChar == (char)Characters::GHOST || backgroundChar == (char)Characters::MARIO || backgroundChar == (char)Characters::HAMMER)
		std::cout << (char)Characters::AIR;
	else
		std::cout << backgroundChar;
}

/* Sets the file names for steps and results based on the provided index.
   The filename prefix is derived from the fileNames list by removing the file extension.
   It then sets the filenames for steps and results by appending ".steps" and ".result" respectively. */
void Game::setFileNames(int index) 
{
	filename_prefix = fileNames[index].substr(0, fileNames[index].find_last_of('.'));
	stepsFilename = filename_prefix + ".steps";
	resultsFilename = filename_prefix + ".result";
}

void Game::restartGame() 
{
	resetGameState();

	if (!isSilent) 
	{
		if (colorModeEnabled) 
		{
			// handle number of life
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD pos = { static_cast<SHORT>(initIconPos.getX() + heroLives + 2) , initIconPos.getY() };
			SetConsoleCursorPosition(hConsole, pos);
			std::cout << ' ';
		}
		else 
			printLegend();
	}
}

bool Game::handleMarioCollisionWithBarrel(int index) 
{
	Point marioPos = hero.getPosition();
	Point barrelPos = barrelVec[index].getPosition();

	if (marioPos == barrelPos) 
	{
		decreaseLivesAndRestart();
		marioHitBarrel = true;

		return true;
	}
	else 
		return false;
}

void Game::handleMarioIsInExplosionRange(int index) 
{
	Point marioPos = hero.getPosition();
	Point barrelPos = barrelVec[index].getPosition();

	if (abs(barrelPos.getX() - marioPos.getX()) <= radiusExp && abs(barrelPos.getY() - marioPos.getY()) <= radiusExp) 
	{
		decreaseLivesAndRestart();
		marioNearExplosion = true;
	}
}

void Game::updateMarioArmedState() 
{
	if (hero.getPosition() == board.getHammerPos() && board.isHaveAHammer()) 
	{
		isArmed = true;
		board.setCurrentBoard((char)Characters::AIR);
	}

	if (isArmed && !isSilent) 
		hud.renderArmedStatus(board.getLegendPos());
}

void Game::decreaseLivesAndRestart() 
{
	heroLives--;
	isArmed = false;

	if (!isSilent) 
	{
		// Display explosion effect at Mario's position
		displayExplosionEffect(hero.getPosition());

		gotoxy(board.getLegendPos().getX(), board.getLegendPos().getY() + 2);
		std::cout << "              ";
	}

	restartGame();
}

void Game::drawHeartIcon(int x) const 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x) , initIconPos.getY() };
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

void Game::resetGameState() 
{
	for (int i = 0; i < barrelVec.size(); i++) 
	{
		barrelVec[i].erase();
		barrelVec[i].setBarrel();
	}

	for (int i = 0; i < ghostsVec.size(); i++) 
	{
		ghostsVec[i]->setActiveStatus();
		ghostsVec[i]->setExplodeStatus();
	}

	barrelVec.clear();

	if (!isSilent) 
	{
		deletePrevHammer();

		if (board.isHaveAHammer()) 
		{
			gotoxy(board.getHammerPos().getX(), board.getHammerPos().getY());
			std::cout << (char)Characters::HAMMER;
			board.setCurrentBoard((char)Characters::HAMMER);
		}

		hero.erase();
	}

	hero.setDir(Keys::STAY);
	hero.setPosition(board.getMarioInitPos());
	initIconPos = board.getLegendPos();
}

void Game::handleGameEndDefault(bool isVictory)
{
	clearConsole();

	if (!isSilent) 
	{
		if (isVictory) 
			displayVictoryMessage();
		else 
			displayGameOverMessage();
	}

	Sleep(isSilent ? 0 : 2000);
	resetGameState();
	heroLives = 3;
	score = 0;
	fileNames.clear();
	fileNumber = 0;
	isLastScreen = false;
	clearConsole();
	results.clearResultsList();
	steps.clearStepsList();
	// Important: do NOT call run() here.
	// Calling run() from inside gameplay creates a nested menu loop (so exiting the menu won't terminate the program).
	// Returning allows control to unwind back to InteractiveMode::run()'s main loop.
}

void Game::displayGameOverMessage() const 
{
	const char* youLostMessage[] = {
		"__     ______  _    _         _      ____   _____ _______     ",
		"\\ \\   / / __ \\| |  | |       | |    / __ \\ / ____|__   __|",
		" \\ \\_/ / |  | | |  | |       | |   | |  | | (___    | |     ",
		"  \\   /| |  | | |  | |       | |   | |  | |\\___ \\   | |    ",
		"   | | | |__| | |__| |       | |___| |__| |____) |  | |       ",
		"   |_|  \\____/ \\____/        |______\\____/|_____/   |_|    "
	};

	const int ROWS = 6;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (colorModeEnabled) 
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);

	for (int i = 0; i < ROWS; i++) 
	{
		std::cout << youLostMessage[i] << std::endl;
	}

	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << "\n\n You scored: " << score;
}

void Game::displayVictoryMessage() const 
{
	const char* youWonMessage[] = {
	"__     ______  _    _         __          ______  _   _         ",
	"\\ \\   / / __ \\| |  | |        \\ \\        / / __ \\| \\ | | ",
	" \\ \\_/ / |  | | |  | |         \\ \\  /\\  / / |  | |  \\| |  ",
	"  \\   /| |  | | |  | |          \\ \\/  \\/ /| |  | | . ` |    ",
	"   | | | |__| | |__| |           \\  /\\  / | |__| | |\\  |     ",
	"   |_|  \\____/ \\____/             \\/  \\/   \\____/|_| \\_|  "
	};

	const int ROWS = 6;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (colorModeEnabled) 
		SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);

	for (int i = 0; i < ROWS; i++) 
	{
		std::cout << youWonMessage[i] << std::endl;
	}

	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << "\n\nCongratulations! You scored: " << score;
}

void Game::printScore() 
{
	hud.renderScore(initIconPos, score);
}

void Game::displayExplosionEffect(Point marioPos) 
{
	const char explosionChars[] = { '*', 'o', 'x', '+', '-' }; // Characters to simulate explosion
	int explosionRadius = 2;
	int yCoordinate;

	if (board.getChar({ marioPos.getX(), marioPos.getY() + 1 }) != (char)Characters::WALL)
		yCoordinate = marioPos.getY() + explosionRadius;
	else
		yCoordinate = marioPos.getY();
	
	for (int frame = 0; frame < 5; ++frame) 
	{ 
		for (int y = marioPos.getY() - explosionRadius; y <= yCoordinate; ++y) 
		{
			for (int x = marioPos.getX() - explosionRadius; x <= marioPos.getX() + explosionRadius; ++x) 
			{
				if (board.isInBoundaries(Point(x, y))) 
				{
					gotoxy(x, y);
					std::cout << explosionChars[frame % 5];
				}
			}
		}

		Sleep(isSilent ? 0 : 40);
		clearExplosion(marioPos, explosionRadius); 
	}
}

void Game::clearExplosion(Point marioPos, int radius)
{
	for (int y = marioPos.getY() - radius; y <= marioPos.getY() + radius; ++y) 
	{
		for (int x = marioPos.getX() - radius; x <= marioPos.getX() + radius; ++x) 
		{
			if (board.isInBoundaries(Point(x, y)) && board.getChar({ x,y }) != (char)Characters::WALL) 
			{ 
				gotoxy(x, y);
				std::cout << board.getChar(Point(x, y));
			}
		}
	}
}


