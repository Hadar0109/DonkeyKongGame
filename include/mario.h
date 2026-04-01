#pragma once

#include "board.h"
#include "point.h"
#include "gameConfig.h"
#include "utils.h"


class Mario 
{
	Board* pBoard = nullptr;
	Point _position;
	int _dirX;
	int _dirY;
	int jumpingCounter = 0;
	bool isJumping = false;
	bool isClimbingUp = false;
	bool isClimbingDown = false;
	bool isFalling = false;
	int fallingCounter = 0;

	void draw(char c) const 
	{
		gotoxy(_position.getX(), _position.getY());
		std::cout << c;
	}
	void jump();
	void climbUp();
	void climbDown();
	void handleUp();
	void handleDown();
	void handleStay();
	void handleHorizontal(Keys key);
	void moveHorizontal();
	void isMarioInAir();
	void freeFalling();

public:
	void setBoard(Board& board) { pBoard = &board; }
	void draw() const { draw((char)Characters::MARIO); }
	void setDir(Keys key);
	void move();
	void erase() const;
	void setPosition(const Point& other);
	Point getPosition() const;
	bool isFallDeadly();
};