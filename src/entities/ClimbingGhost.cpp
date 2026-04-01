#include "ClimbingGhost.h"
#include <cmath>

void ClimbingGhost::move() 
{
	handleDown();
	handleUp();
	EntityGhost::setDirX();

	if (isClimbingUp) 
		climbUp();
	else if (isClimbingDown) 
		climbDown();
	else 
		prevDirX = _dirX;

	int nextX = _position.getX() + _dirX;
	int nextY = _position.getY();
	Point nextPos = { nextX, nextY };

	if (pBoard->isValidPosition(nextPos)) 
		_position.setPosition(nextPos);
}

void ClimbingGhost::handleUp() 
{
	if (pBoard->isLadder(_position) && !isClimbingDown) 
	{
		if (!isClimbingUp)
			counter++;

		if (counter % 4 == 0) 
			isClimbingUp = true;
	}
}

void ClimbingGhost::climbUp() 
{
	_dirY = (int)Direction::NEGATIVE;
	_dirX = (int)Direction::STAY;
	int nextY = _position.getY() + _dirY;

	if (pBoard->getChar(_position) == (char)Characters::AIR) 
	{
		_dirY = (int)Direction::STAY;
		_dirX = prevDirX;
		isClimbingUp = false;
	}
	else 
		_position.setY(nextY);
}

void ClimbingGhost::handleDown() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point nextPos = { currX, currY + 1 };
	_dirY = (int)Direction::POSITIVE;

	if ((pBoard->isLadder(Point(_position.getX(), _position.getY() + 2)) || pBoard->isLadder(nextPos)) && !isClimbingUp) 
	{
		if (!isClimbingDown)
			counter++;

		if (counter % 4 == 0) 
			isClimbingDown = true;
	}
}

void ClimbingGhost::climbDown() 
{
	_dirY = (int)Direction::POSITIVE;
	_dirX = (int)Direction::STAY;
	
	int currX = _position.getX();
	int currY = _position.getY();
	int nextY = currY + _dirY;
	Point nextPos = { currX, nextY };
	Point underLadderPos = { currX, nextY + 1 };

	if (pBoard->isOnFloor(_position) && pBoard->isLadder(underLadderPos)) 
		_position.setY(nextY);
	else if (pBoard->isValidPosition(nextPos)) 
		_position.setY(nextY);
	else 
	{
		_dirY = (int)Direction::STAY;
		_dirX = prevDirX;
		isClimbingDown = false;
	}
}