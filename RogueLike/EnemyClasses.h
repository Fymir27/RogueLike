#pragma once

#include "Enemy.h"

class Ghost : public Enemy
{
	private:
	public:
    Ghost(Position pos);
    void attack(Character* target);
};

class Spider : public Enemy
{
	private:
	public:
    Spider(Position pos);
    void attack(Character* target);
};

class Golem : public Enemy
{
	private:
	public:
    Golem(Position pos);
    void attack(Character* target);
};