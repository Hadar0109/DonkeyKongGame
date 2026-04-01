#pragma once

#include <iostream>

class Point 
{
    int x;
    int y;

public:
    Point() = default; // empty ctor
    constexpr Point(int _x, int _y) : x(_x), y(_y) {}

    bool operator ==(const Point& other) const { return x == other.x && y == other.y; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setPosition(const Point& other) { x = other.getX(); y = other.getY(); }
    constexpr int getX() const { return x; }
    constexpr int getY() const { return y; }
};