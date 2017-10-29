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

	protected:
		size_t exp_reward_;

	public:
		
		//-ctors-//
		Enemy(string name, Position pos, Stats stats, MoveType move_type, string texture = "../images/enemy.png");

		~Enemy();

		virtual bool step(); //returns false when the enemy is dead

		//void attack(Character* target);
};
