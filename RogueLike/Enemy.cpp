#include "Enemy.h"
#include "Room.h"
#include "Player.h"

bool Enemy::step()
{
	//cout << "Enemy::step()" << endl;

	if (dead())
		return false;

	switch (move_type_)
	{
	case STAY:
		stay();
		break;

	case FOLLOW:
		move(current_room->getPathToPlayer(pos_));
		break;

	case RANDOM:
		moveRandomly();
		break;
	}
		
	return true;
}

Enemy::Enemy(string name, Position pos, Stats stats, MoveType move_type, string texture) : Character(name, pos, stats, texture), move_type_(move_type)
{
	current_room->addEnemy(this);
}

void Enemy::stay()
{
	Position new_pos = current_room->getPathToPlayer(pos_);
	if (new_pos == current_player->getPosition())
		move(new_pos);
}

void Enemy::moveRandomly()
{
	int r;
	do 
	{ 
		r = rand() % 4; 
	} while (!move(Position(pos_.x_ + DELTA_X[r], pos_.y_ + DELTA_Y[r])));
}

Enemy::~Enemy()
{
	current_room->removeEnemy(this);
}
