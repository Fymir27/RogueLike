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
	case SLEEP:
		break;

	case STAY:
		checkSurroundings();
		break;

	case WAIT:
		if(checkSurroundings())
			move_type_ = FOLLOW;
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

bool Enemy::checkSurroundings()
{
	for(size_t i = 0; i < 4; i++)
	{
		if(current_room->getFieldStatus(pos_ + DELTA_POS[i]) == OCCUPIED)
		{
			move(pos_ + DELTA_POS[i]);
			return true;
		}
	}		
}

void Enemy::moveRandomly()
{
	int counter = 0;
	int r = rand() % 4;
	do 
	{ 
		r = (r+1) % 4; 
		if(counter++ > 3)
		{
			move_type_ = SLEEP;
			break;
		}
	} while (!move(Position(pos_.x_ + DELTA_X[r], pos_.y_ + DELTA_Y[r])));
}

Enemy::~Enemy()
{
	current_room->removeEnemy(this);
}

void Enemy::attack(Character* target)
{
	cout << "Enemy::attack" << endl;
}
