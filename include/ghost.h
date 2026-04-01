#pragma once
#include "EntityGhost.h"

class Ghost : public EntityGhost 
{
public:
	Ghost(): EntityGhost() {}
	
	void move() override;
	void draw() const override { EntityGhost::draw((char)Characters::GHOST); }
};

