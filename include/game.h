#pragma once
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <memory>

#include "board.h"
#include "utils.h"
#include "point.h"
#include "mario.h"
#include "gameConfig.h"
#include "barrel.h"    
#include "menu.h"
#include "ghost.h"
#include "EntityGhost.h"
#include "ClimbingGhost.h"
#include "steps.h"
#include "results.h"
#include "hudRenderer.h"
#include "gameMode.h"

class Game 
{
protected:
	friend class InteractiveMode;
	friend class ReplayMode;

	Board board;
	Mario hero;
	Menu menu;
	Point hammerPos;
	Point initIconPos;
	Steps steps;
	Results results;
	HudRenderer hud;
	std::vector<std::unique_ptr<EntityGhost>> ghostsVec;
	std::vector<Barrel> barrelVec;
	std::vector<std::string> fileNames;
	std::string filename_prefix;
	std::string stepsFilename;
	std::string resultsFilename;
	size_t nextLifeLostEvent = 0;
	size_t nextPaulineEvent = 0;
	size_t iteration = 0;

	long random_seed;
	bool isLoad = false;
	bool isSilent = false;
	bool isSave = false;
	bool isLastScreen = false;
	bool isGoodStepsFile = true;
	bool isGoodResultFile = true;
	bool unMatchingResult = false;
	bool isAutoTest = false;
	bool colorModeEnabled = false;
	bool isFileValid = true;
	bool marioHitBarrel = false;
	bool marioHitGhost = false;
	bool marioFallDeadly = false;
	bool marioNearExplosion = false;
	bool isPlaying = true;
	bool isArmed = false;
	int fileNumber = 0;
	int heroLives = 3;
	int score = 0;
	char key;
	char prevKey;
	static constexpr int radiusExp = 2;

	void playGame();
	void setFileNames(int index);
	void resetBoardAndHero();
	void killGhostOrBarrel(Point position);
	void createVecGhosts();
	void createBarrel();
	void moveBarrels();
	void moveGhosts();
	bool updateSingleBarrel(int index);
	bool updateSingleGhost(int index);
	void moveHero(char key);
	void moveWithHammer(char c, char key);
	void checkHeroFall();
	void checkGhostCollision(int index);
	bool handleMarioCollisionWithGhost(int index);
	bool handleMarioCollisionWithBarrel(int index);
	void handleHammerUsage();
	void handleMarioIsInExplosionRange(int index);
	void updateMarioArmedState();
	void deletePrevHammer();
	void printLegend();
	void printScore();
	void drawHeartIcon(int x) const;
	void decreaseLivesAndRestart();
	void displayExplosionEffect(Point marioPos);
	void clearExplosion(Point marioPos, int radius);
	void resetGameState();
	void restartGame();
	void handleGameEndDefault(bool isVictory);

	std::unique_ptr<IGameMode> mode;

public:
	explicit Game(std::unique_ptr<IGameMode> _mode) : mode(std::move(_mode)) {}

	void run();
	void displayGameOverMessage() const;
	void displayVictoryMessage() const;
	void setIsLoad(bool _isLoad) { isLoad = _isLoad; }
	void setIsSilent(bool _isSilent) { isSilent = _isSilent; }
	void setIsSave(bool _isSave) { isSave = _isSave; }
	void setIsAutoTest(bool _isAutoTest) { isAutoTest = _isAutoTest; }
	bool getIsAutoTest() const { return isAutoTest; }
	bool didReplayTestPass() const { return isFileValid && !unMatchingResult; }
};