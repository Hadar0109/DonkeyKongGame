#include "EntityGhost.h"

void EntityGhost::erase() const 
{
	char backgroundChar = pBoard->getChar(_position);

	if (backgroundChar == (char)Characters::BIG_GHOST || backgroundChar == (char)Characters::GHOST || backgroundChar == (char)Characters::MARIO)
		draw((char)Characters::AIR);
	else
		draw(backgroundChar);
}

Point EntityGhost::getPosition() const 
{
	return _position;
}

void EntityGhost::explosion() 
{
	erase();
	isExplode = true; 
}

void EntityGhost::setExplodeStatus() 
{
	isExplode = false;
}

void EntityGhost::setPosition(const Point& other) 
{
	_position.setPosition(other);
}

void EntityGhost::setActiveStatus() 
{
	isActive = true;
}

void EntityGhost::setCollision(bool status) 
{
	isColliding = status;
}

bool EntityGhost::getActiveStatus() 
{
	if (isExplode) 
		isActive = false;
	
	return isActive;
}

int EntityGhost::getDirX() 
{
	return _dirX;
}

void EntityGhost::randDirX() 
{
	if ((double)rand() / RAND_MAX >= 0.95) 
		_dirX = -_dirX;
}

void EntityGhost::setDirX() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point nextPos = { currX + _dirX, currY };
	
	// Add occasional randomness to avoid fully deterministic pacing.
	if (currY == (int)Boundaries::MAX_Y - 1) 
		randDirX();
	else if (isColliding == true || !pBoard->isOnFloor(nextPos)) 
		_dirX = -_dirX;
	else 
		randDirX();
}