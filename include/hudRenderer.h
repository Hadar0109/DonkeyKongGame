#pragma once
#include "point.h"

class HudRenderer
{
public:
	void renderLegend(const Point& iconPos, int lives, int score, bool colorModeEnabled) const;
	void renderScore(const Point& iconPos, int score) const;
	void renderArmedStatus(const Point& legendPos) const;

private:
	void drawHeartIcon(int x, int y) const;
};

