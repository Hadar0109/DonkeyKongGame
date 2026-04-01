#include "ghost.h"


void Ghost::move() 
{
    EntityGhost::setDirX();

    int nextX = _position.getX() + _dirX;
    int nextY = _position.getY();
    Point nextPos = { nextX, nextY };

    if (pBoard->isValidPosition(nextPos)) 
        _position.setPosition(nextPos);   
}


