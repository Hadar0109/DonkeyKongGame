#include "mario.h"

void Mario::erase() const 
{
	char backgroundChar = pBoard->getChar(_position);

	if (backgroundChar == (char)Characters::MARIO || backgroundChar == (char)Characters::GHOST || backgroundChar == (char)Characters::HAMMER)
		draw((char)Characters::AIR);
	else
		draw(backgroundChar);
}

void Mario::setDir(Keys key) 
{
	switch (key) 
	{
	case Keys::LEFT:
		handleHorizontal(Keys::LEFT);
		break;
	case Keys::RIGHT:
		handleHorizontal(Keys::RIGHT);
		break;
	case Keys::UP:
		handleUp();
		break;
	case Keys::DOWN:
		handleDown();
		break;
	case Keys::STAY:
		handleStay();
		break;
	default:
		break;
	}
}

void Mario::move() 
{
	if (isClimbingUp) 
		climbUp();
	else if (isJumping && !isFalling) 
		jump();
	else if (isClimbingDown) 
		climbDown();
	else 
		moveHorizontal();

	isMarioInAir();

	if (isFalling) 
		freeFalling();
	else 
		fallingCounter = 0;
}

void Mario::handleUp() 
{
	if (!isFalling) 
	{
		_dirY = (int)Direction::NEGATIVE;

		if ((pBoard->isLadder(_position) && !isJumping) || isClimbingUp) 
		{
			isClimbingUp = true;
			_dirX = (int)Direction::STAY;
		}
		else 
			isJumping = true;
	}
}

void Mario::climbUp() 
{
	int nextY = _position.getY() + _dirY;

	if (pBoard->getChar(_position) == (char)Characters::AIR) 
	{
		_dirY = (int)Direction::STAY;
		isClimbingUp = false;
	}
	else 
		_position.setY(nextY);
}

void Mario::jump() 
{
	int nextX = _position.getX() + _dirX;
	int nextY = _position.getY() + _dirY;
	Point nextPos = { nextX, nextY };

	// Allow a short fixed-height jump (2 ticks) before falling.
	if (jumpingCounter < 2) 
	{
		if (pBoard->isValidPosition(nextPos)) 
			_position.setPosition(nextPos);
		
		jumpingCounter++;
	}
	else 
	{
		isJumping = false;
		jumpingCounter = 0;
		_dirY = (int)Direction::STAY;
		isFalling = true;
	}
}

void Mario::handleDown() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point nextPos = { currX, currY + 1 };
	_dirY = (int)Direction::POSITIVE;

	if (pBoard->isLadder(Point(_position.getX(), _position.getY() + 2)) || pBoard->isLadder(nextPos)) 
	{
		isClimbingDown = true;
		isClimbingUp = false;
	}
	else 
		_dirX = _dirY = (int)Direction::STAY;
}

void Mario::climbDown() 
{
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
		isClimbingDown = false;
	}
}

void Mario::handleStay() 
{
	isClimbingUp = false;
	isClimbingDown = false;

	if (pBoard->isValidPosition(_position)) 
	{
		_dirX = (int)Direction::STAY;
		_dirY = (int)Direction::STAY;
	}
}

void Mario::handleHorizontal(Keys key) 
{
	if (!isClimbingUp && !isClimbingDown) 
	{
		if (key == Keys::LEFT) 
			_dirX = (int)Direction::NEGATIVE;
		else 
			_dirX = (int)Direction::POSITIVE;

		_dirY = (int)Direction::STAY;
	}
}

void Mario::moveHorizontal()
{
	int nextX = _position.getX() + _dirX;
	int nextY = _position.getY() + _dirY;
	Point nextPos = { nextX, nextY };

	if (pBoard->isValidPosition(nextPos)) 
		_position.setPosition(nextPos);
	else 
		_dirX = _dirY = (int)Direction::STAY;
}

void Mario::isMarioInAir() 
{
	if (pBoard->isInAir(_position) && !isJumping) 
		isFalling = true;
}

void Mario::setPosition(const Point& other) 
{
	_position.setPosition(other);
}

Point Mario::getPosition() const 
{
	return _position;
}

void Mario::freeFalling() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point currPos = { currX, currY + 1 };

	if (pBoard->isValidPosition(currPos)) 
	{
		_position.setY(currY + 1);
		fallingCounter++;
	}
	else 
		isFalling = false;
}

bool Mario::isFallDeadly() 
{
	// A long fall (5+ ticks) costs a life once Mario lands.
	if (fallingCounter >= 5 && (pBoard->isOnFloor(_position) || _position.getY() + 1 == (int)Boundaries::MAX_Y)) 
	{
		fallingCounter = 0;

		return true;
	}
	else 
		return false;
}



