#pragma once
#include "Common.h"
#include "Types.h"
#include "Character.h"

enum MoveType
{
	STAY,
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

	void stay();
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
	Ghost(Position pos) : Enemy("Ghost", pos, Stats(30, 100, 0, 20, 0), FOLLOW, "../images/ghost.png") {};
};

class Spider : public Enemy
{
	private:
	public:
	Spider(Position pos) : Enemy("Spider", pos, Stats(10, 10, 5, 10, 15), RANDOM, "../images/spider.png") {};
};

class Golem : public Enemy
{
	private:
	public:
	Golem(Position pos) : Enemy("Golem", pos, Stats(100, 0, 30, 5, 5), STAY, "../images/golem.png") {};
};