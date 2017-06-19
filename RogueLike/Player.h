#pragma once
#include "Character.h"

class Player : public Character
{
private:

public:
	Player(const string name, const Position pos, const Stats stats);
};

