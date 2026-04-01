#pragma once
#include "point.h"
#include "board.h"
#include "utils.h"

class Barrel 
{
	Board* pBoard;
	Point _position;
	bool isExplode;
	int _dirX;
	int _dirY;
	int counterFall;
	char prevFloorChar;
	bool isActive;

	void freeFalling();
	void draw(char c) const 
	{
		gotoxy(_position.getX(), _position.getY());
		std::cout << c;
	}

public:
	Barrel(): pBoard(nullptr), _position({ 0,0 }), isExplode(false), _dirX(0), _dirY(0),counterFall(0), prevFloorChar(), isActive(false) {}

	void setDir();
	bool isKeepingSameDir(Point floorChar);
	void draw() const  { draw((char)Characters::BARREL); }
	void move();
	void setPosition(const Point& other);
	bool getActiveStatus();
	bool isBarrelExplode() const;
	void setBarrel();
	void setBoard(Board& board) { pBoard = &board; }
	void erase() const;
	Point getPosition() const;
	void explosion();
	void setExplodeStatus();
};