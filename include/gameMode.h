#pragma once
#include <memory>

class Game;

class IGameMode 
{
public:
	virtual ~IGameMode() = default;

	virtual void run(Game& game) = 0;
	virtual void handleUserInput(Game& game) = 0;
	virtual void initializeTypeGame(Game& game) = 0;
	virtual void completeProcess(Game& game) = 0;
	virtual void checkResultsState(Game& game) = 0;
	virtual void marioLossLife(Game& game) = 0;
	virtual void validateLifeLoss(Game& game) = 0;
	virtual void marioMeetsPauline(Game& game) = 0;
	virtual void validatePaulineMeeting(Game& game) = 0;
	virtual void setNextIterationEvents(Game& game) = 0;
	virtual void handleGameEnd(Game& game, bool isVictory) = 0;
};

