#include <chrono>
#include <cctype>
#include <conio.h>

#include "gameModes.h"
#include "game.h"
#include "BoardRepository.h"
#include "BoardRenderer.h"
#include "ResultErrorPresenter.h"

// ---------------- InteractiveMode ----------------
void InteractiveMode::run(Game& game) 
{
	ShowConsoleCursor(false);
	BoardRepository::getAllBoardFileNames(game.fileNames);

	while (game.isPlaying) 
	{
		game.menu.displayMenu(MenuType::MAIN_MENU);
		int choice = getUserChoice();
		handleMenuChoice(game, (char)choice);
	}
}

void InteractiveMode::handleUserInput(Game& game) 
{
	bool marioKeyPressed = false;
	bool hammerKeyPressed = false;

	for (int i = 0; i < 5; i++) 
	{
		Sleep(12);

		if (_kbhit()) 
		{
			game.key = std::tolower(_getch());

			if (isValidKey(game.key)) 
			{
				if (game.key == (char)Keys::ESC) 
				{
					pauseGame(game);
					BoardRenderer::print(game.board);
					game.printLegend();
				}

				if (game.key == (char)Keys::LEFT || game.key == (char)Keys::RIGHT)
					game.prevKey = game.key;

				if (game.isArmed && game.key == (char)Keys::HAMMER && !hammerKeyPressed) 
				{
					hammerKeyPressed = true;
					game.handleHammerUsage();
					gotoxy(game.hammerPos.getX(), game.hammerPos.getY());
					std::cout << '+';
					Sleep(20);
					game.steps.addStep(game.iteration, game.key);
				}
				else if (!marioKeyPressed) 
				{
					marioKeyPressed = true;
					game.hero.setDir((Keys)game.key);
					game.steps.addStep(game.iteration, game.key);
				}
			}
		}
	}
}

void InteractiveMode::initializeTypeGame(Game& game) 
{
	if (game.isSave) 
	{
		game.random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
		game.steps.setRandomSeed(game.random_seed);
	}
}

void InteractiveMode::completeProcess(Game& game) 
{
	if (game.isSave) 
	{
		game.steps.saveSteps(game.stepsFilename);
		game.results.saveResults(game.resultsFilename, game.score, game.colorModeEnabled);
	}
}

void InteractiveMode::marioLossLife(Game& game) 
{
	if (game.isSave)
		game.results.addResult(game.iteration, ResultValue::DISQUALIFIED);
}

void InteractiveMode::marioMeetsPauline(Game& game) 
{
	if (game.isSave)
		game.results.addResult(game.iteration, ResultValue::FINISHED);
}

void InteractiveMode::handleGameEnd(Game& game, bool isVictory) 
{
	game.handleGameEndDefault(isVictory);
}

void InteractiveMode::checkResultsState(Game&) {}
void InteractiveMode::validateLifeLoss(Game&) {}
void InteractiveMode::validatePaulineMeeting(Game&) {}
void InteractiveMode::setNextIterationEvents(Game&) {}

void InteractiveMode::handleMenuChoice(Game& game, char choice) 
{
	bool validChoice = false;

	while (!validChoice) 
	{
		game.fileNumber = 0;

		switch (choice) 
		{
		case '1':
			clearConsole();
			game.colorModeEnabled = false;
			game.playGame();
			validChoice = true;
			break;
		case '2':
			clearConsole();
			game.colorModeEnabled = true;
			game.playGame();
			validChoice = true;
			break;
		case '3':
			clearConsole();
			printFilesOptions(game);
			game.fileNumber--;
			clearConsole();
			game.colorModeEnabled = false;
			game.playGame();
			validChoice = true;
			break;
		case '8':
			clearConsole();
			game.menu.displayMenu(MenuType::INSTRUCTIONS);
			_getch();
			clearConsole();
			validChoice = true;
			break;
		case '9':
			clearConsole();
			std::cout << "Thanks for playing! Goodbye!" << std::endl;
			game.isPlaying = false;
			validChoice = true;
			break;
		default:
			choice = _getch();
			break;
		}
	}
}

void InteractiveMode::pauseGame(Game& game) 
{
	clearConsole();
	game.menu.displayMenu(MenuType::PAUSE_MENU);

	while (true) 
	{
		if (_kbhit()) 
		{
			char key = std::tolower(_getch());

			if (key == (char)Keys::ESC) 
			{
				clearConsole();
				return;
			}
		}
	}
}

bool InteractiveMode::isValidKey(char key) const 
{
	return (key == (char)Keys::UP ||
		key == (char)Keys::DOWN ||
		key == (char)Keys::LEFT ||
		key == (char)Keys::RIGHT ||
		key == (char)Keys::STAY ||
		key == (char)Keys::HAMMER);
}

char InteractiveMode::getUserChoice() const 
{
	return _getch();
}

void InteractiveMode::printFilesOptions(Game& game) 
{
	printHeader();
	int size = (int)game.fileNames.size();
	printFileOptions(game, 0, 9, 7);

	if (size > 9) 
	{
		printAdditionalInfo(17);
		game.fileNumber = getFileNumber();

		if (game.fileNumber == 0) 
		{
			clearConsole();
			printHeader();
			printFileOptions(game, 9, size, 7);
			int num = getFileNumber();

			if (num != 0) 
				game.fileNumber = num + 9;
		}
	}
	else 
		game.fileNumber = getFileNumber();
}

void InteractiveMode::printHeader() const 
{
	gotoxy(21, 4);
	std::cout << "===================================" << std::endl;
	gotoxy(28, 5);
	std::cout << " CHOOSE YOUR SCREEN: " << std::endl;
	gotoxy(21, 6);
	std::cout << "===================================" << std::endl;
}

void InteractiveMode::printFileOptions(Game& game, int startIndex, int endIndex, int startLineNum) const 
{
	int lineNum = startLineNum;

	for (int i = startIndex; i < endIndex && i < (int)game.fileNames.size(); i++) 
	{
		gotoxy(28, lineNum);
		std::cout << i - startIndex + 1 << ". ";
		std::cout << game.fileNames[i] << std::endl;
		lineNum++;
	}
}

void InteractiveMode::printAdditionalInfo(int lineNum) const 
{
	gotoxy(14, lineNum++);
	std::cout << "There are over 10 screen options. Showing first 9.";
	gotoxy(17, lineNum++);
	std::cout << "Press 0 for more, or any key for main menu";
}

int InteractiveMode::getFileNumber() const 
{
	return _getch() - '0';
}


// ---------------- ReplayMode ----------------
void ReplayMode::run(Game& game) 
{
	ShowConsoleCursor(false);
	BoardRepository::getAllBoardFileNames(game.fileNames);

	while (game.isPlaying) 
	{
		game.playGame();
	}
}

void ReplayMode::handleUserInput(Game& game) 
{
	for (int i = 0; i < 2; i++) 
	{
		if (game.steps.isNextStepOnIteration(game.iteration)) 
		{
			char key = game.steps.popStep();

			if (key == (char)Keys::LEFT || key == (char)Keys::RIGHT)
				game.prevKey = key;
			else if (game.isArmed && key == (char)Keys::HAMMER)
			{
				game.handleHammerUsage();
				gotoxy(game.hammerPos.getX(), game.hammerPos.getY());
				std::cout << '+';
			}

			game.hero.setDir((Keys)key);
		}

		Sleep(game.isSilent ? 0 : 20);
	}
}

void ReplayMode::initializeTypeGame(Game& game) 
{
	game.steps = Steps::loadSteps(game.stepsFilename);
	game.isGoodStepsFile = game.steps.getGoodFile();
	game.random_seed = game.steps.getRandomSeed();
	game.results = Results::loadResults(game.resultsFilename);
	game.isGoodResultFile = game.results.getGoodFile();

	if (!game.isGoodStepsFile || !game.isGoodResultFile) 
	{
		Sleep(1000);
		clearConsole();
	}

	game.colorModeEnabled = (bool)game.results.getColorMode(game.resultsFilename);
}

void ReplayMode::marioLossLife(Game& game) 
{
	if (game.results.popResult() != std::pair{ game.iteration, ResultValue::DISQUALIFIED }) 
	{
		ResultErrorPresenter::show("TEST FAILED: ", "Mario lost a life in the game, but it is missing from the results file", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
		game.unMatchingResult = true;
		game.isFileValid = false;
	}

	game.nextLifeLostEvent = game.results.getNextBombIteration();
}

void ReplayMode::completeProcess(Game& game) 
{
	if (!game.unMatchingResult) 
	{
		if (game.results.popResult().second != ResultValue::NO_RESULT)
		{
			ResultErrorPresenter::show("TEST FAILED: ", "The results file includes extra events after the game ended", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
			game.unMatchingResult = true;
		}
	}
}

void ReplayMode::checkResultsState(Game& game)
{
	if (game.results.isFinishedBy(game.iteration)) 
	{
		clearConsole();
		game.isFileValid = false;

		if (!game.results.isReachedPauline())
		{
			ResultErrorPresenter::show("TEST FAILED: ", "The results file ended, but the game was not fully completed", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
			game.isLastScreen = true;
			handleGameEnd(game, false);
			game.unMatchingResult = true;
		}
	}
}

void ReplayMode::validateLifeLoss(Game& game)
{
	if (game.iteration == game.nextLifeLostEvent)
	{
		ResultErrorPresenter::show("TEST FAILED: ", "The results file indicates a life loss event, but no such event occurred in the game", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
		game.unMatchingResult = true;
		game.isFileValid = false;
	}
}

void ReplayMode::validatePaulineMeeting(Game& game)
{
	if (game.iteration == game.nextPaulineEvent)
	{
		ResultErrorPresenter::show("TEST FAILED: ", "The results file indicates reaching Pauline, but no such event occurred in the game", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
		game.unMatchingResult = true;
		game.isFileValid = false;
	}
}

void ReplayMode::marioMeetsPauline(Game& game)
{
	if (game.results.popResult() != std::pair{ game.iteration, ResultValue::FINISHED }) 
	{
		ResultErrorPresenter::show("TEST FAILED: ", "Mario reached Pauline in the game, but it is missing from the results file", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
		game.unMatchingResult = true;
		game.isFileValid = false;
	}
	else {
		if (game.isLastScreen) 
		{
			if (game.isSilent) 
			{
				ResultErrorPresenter::show("TEST PASSED: ", "You completed all the screens. YOU WON!", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
				game.isPlaying = false;
			}
		}
		else 
		{
			if (game.isSilent)
				ResultErrorPresenter::show("TEST PASSED: ", "You reached Pauline and moved on to the next screen", game.fileNames[game.fileNumber], game.iteration, game.score, !game.getIsAutoTest());
		}
	}
}

void ReplayMode::setNextIterationEvents(Game& game) 
{
	if (!game.results.isFinishedBy(game.iteration)) 
	{
		game.nextLifeLostEvent = game.results.getNextBombIteration();
		game.nextPaulineEvent = game.results.getNextPaulineIteration();
	}
}

void ReplayMode::handleGameEnd(Game& game, bool isVictory)
{
	clearConsole();

	if (!game.isSilent)
	{
		if (isVictory) 
			game.displayVictoryMessage();
		else 
			game.displayGameOverMessage();
	}

	Sleep(game.isSilent ? 0 : 2000);
	completeProcess(game);
	game.resetGameState();
	clearConsole();
	game.fileNames.clear();
	game.results.clearResultsList();
	game.steps.clearStepsList();
	game.isPlaying = false;
}

