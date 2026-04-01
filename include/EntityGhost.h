#pragma once
#include "Board.h"
#include "point.h"

class EntityGhost 
{
protected:
	Board* pBoard;
	Point _position;
	bool isExplode;
	int _dirX;
	bool isColliding;
	bool isActive;
	void draw(char c) const 
	{
		gotoxy(_position.getX(), _position.getY());
		std::cout << c;
	}

public:
	EntityGhost() : pBoard(nullptr), _position({ 0,0 }), isExplode(false), _dirX(1), isColliding(false), isActive(true) {}

	virtual ~EntityGhost() {}
	virtual void draw() const = 0; 
	virtual void move() = 0;

	void setBoard(Board& board) { pBoard = &board; }
	void erase() const;
	Point getPosition() const;
	void explosion();
	void setExplodeStatus();
	void setPosition(const Point& other);
	void setActiveStatus();
	void setCollision(bool status);
	bool getActiveStatus();
	int getDirX();
	void randDirX();
	void setDirX();
};

