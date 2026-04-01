#include "Barrel.h"

void Barrel::setDir() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point floorChar = { currX, currY + 1 };

	// Floor markers ('<', '>', '=') encode barrel direction on sloped platforms.
	if (pBoard->getChar(floorChar) == (char)FloorCharacters::LEFT) 
	{
		_dirX = (int)Direction::NEGATIVE;
		prevFloorChar = (char)FloorCharacters::LEFT;
	}
	else if (pBoard->getChar(floorChar) == (char)FloorCharacters::RIGHT) 
	{
		_dirX = (int)Direction::POSITIVE;
		prevFloorChar = (char)FloorCharacters::RIGHT;
	}
	else if (isKeepingSameDir(floorChar)) 
	{
		if (prevFloorChar == (char)FloorCharacters::LEFT) 
			_dirX = (int)Direction::NEGATIVE;
		else 
			_dirX = (int)Direction::POSITIVE;
	}

	if (pBoard->isOnFloor(_position)) 
		counterFall = 0;
}

bool Barrel:: isKeepingSameDir(Point floorChar) 
{
	return (
		pBoard->getChar(floorChar) == (char)FloorCharacters::PREV ||
		pBoard->getChar(floorChar) == (char)Characters::WALL ||
		pBoard->getChar(floorChar) == (char)Characters::GHOST ||
		pBoard->getChar(floorChar) == (char)Characters::HAMMER
		);
}

void Barrel::move() 
{
	int nextX = _position.getX() + _dirX;
	int nextY = _position.getY() + _dirY;
	Point nextPos = { nextX, nextY };

	if (pBoard->isValidPosition(nextPos)) 
		_position.setX(nextX);

	freeFalling();
}

void Barrel::freeFalling() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point nextPos = { currX, currY + 1 };

	_dirX = (int)Direction::STAY;

	if (pBoard->isInAir(_position))
	{
		_position.setY(currY + 1);
		counterFall++;
	}
}

bool Barrel::getActiveStatus() 
{
	int currX = _position.getX();
	int currY = _position.getY();
	Point RnextPos = { currX + 1, currY };
	Point LnextPos = { currX - 1, currY };

	// Deactivate once it reaches the bottom, times out after a long fall, explodes, or leaves bounds.
	if (currY == (int)Boundaries::MAX_Y - 1 || (counterFall >= 8 && pBoard->isOnFloor(_position)) || isExplode || !pBoard->isInBoundaries(RnextPos) || !pBoard->isInBoundaries(LnextPos)) {
		isActive = false;
	}

	return isActive;
}

bool Barrel::isBarrelExplode() const 
{
	return isExplode;
}

void Barrel::setPosition(const Point& other) 
{
	_position.setPosition(other);
	isActive = true;
}

void Barrel::setBarrel() 
{
	_position = {};
	counterFall = 0;
	isActive = false;
	isExplode = false;
	_dirX = 0;
	_dirY = 0;
}

void Barrel::erase() const 
{
	char backgroundChar = pBoard->getChar(_position);

	if (backgroundChar == (char)Characters::BIG_GHOST || backgroundChar == (char)Characters::GHOST || backgroundChar == (char)Characters::MARIO)
		draw((char)Characters::AIR);
	else
		draw(backgroundChar);
}

Point Barrel::getPosition() const 
{
	return _position;
}

void Barrel::explosion() 
{
	erase();
	isExplode = true;
}

void Barrel::setExplodeStatus() 
{
	isExplode = false;
}
