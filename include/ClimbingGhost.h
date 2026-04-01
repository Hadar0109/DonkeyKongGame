#pragma once
#include "EntityGhost.h"

class ClimbingGhost : public EntityGhost
{
	int _dirY;
	bool isClimbingUp;
	bool isClimbingDown;
	int prevDirX;
	int counter;
	
	void climbUp();
	void climbDown();

public:
	ClimbingGhost() : EntityGhost(), _dirY(0), isClimbingUp(false), isClimbingDown(false), prevDirX(0), counter(0) {}
	
	void handleUp();
	void handleDown();
	virtual void move() override;
	virtual void draw() const override { EntityGhost::draw((char)Characters::BIG_GHOST); }
};

