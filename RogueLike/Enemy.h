#pragma once
#include "Common.h"
#include "Types.h"
#include "Character.h"

enum MoveType
{
	SLEEP,
	STAY,
	WAIT,
	FOLLOW,
	RANDOM
};

#define ENEMY_TYPE_COUNT 3
enum EnemyType
{
	RANDOM_ENEMY,
	SPIDER,
	GHOST,
	GOLEM
};

class Enemy : public Character
{
	private:
	MoveType move_type_;

	bool checkSurroundings();
	void moveRandomly();

	public:
		
		//-ctors-//
		Enemy(string name, Position pos, Stats stats, MoveType move_type, string texture = "../images/enemy.png");

		~Enemy();

		virtual bool step(); //returns false when the enemy is dead
};

class Ghost : public Enemy
{
	private:
	public:
	Ghost(Position pos) : Enemy("Ghost", pos, Stats(0, 5, 0, 15, 5), FOLLOW, "../images/ghost.png") {};
};

class Spider : public Enemy
{
	private:
	public:
	Spider(Position pos) : Enemy("Spider", pos, Stats(5, 2, 10, 0, 5), RANDOM, "../images/spider.png") {};
};

class Golem : public Enemy
{
	private:
	public:
	Golem(Position pos) : Enemy("Golem", pos, Stats(15, 10, 5, 5, 10), WAIT, "../images/golem.png") {};
};