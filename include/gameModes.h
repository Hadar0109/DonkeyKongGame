#pragma once

#include "gameMode.h"

class InteractiveMode final : public IGameMode 
{
public:
	void run(Game& game) override;
	void handleUserInput(Game& game) override;
	void initializeTypeGame(Game& game) override;
	void completeProcess(Game& game) override;
	void checkResultsState(Game& game) override;
	void marioLossLife(Game& game) override;
	void validateLifeLoss(Game& game) override;
	void marioMeetsPauline(Game& game) override;
	void validatePaulineMeeting(Game& game) override;
	void setNextIterationEvents(Game& game) override;
	void handleGameEnd(Game& game, bool isVictory) override;

private:
	void handleMenuChoice(Game& game, char choice);
	void pauseGame(Game& game);
	bool isValidKey(char key) const;
	char getUserChoice() const;
	void printFilesOptions(Game& game);
	void printHeader() const;
	void printFileOptions(Game& game, int startIndex, int endIndex, int startLineNum) const;
	void printAdditionalInfo(int lineNum) const;
	int getFileNumber() const;
};

class ReplayMode final : public IGameMode 
{
public:
	void run(Game& game) override;
	void handleUserInput(Game& game) override;
	void initializeTypeGame(Game& game) override;
	void completeProcess(Game& game) override;
	void checkResultsState(Game& game) override;
	void marioLossLife(Game& game) override;
	void validateLifeLoss(Game& game) override;
	void marioMeetsPauline(Game& game) override;
	void validatePaulineMeeting(Game& game) override;
	void setNextIterationEvents(Game& game) override;
	void handleGameEnd(Game& game, bool isVictory) override;
};

