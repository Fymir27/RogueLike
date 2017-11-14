#include "Enemy.h"
#include "Room.h"
#include "Player.h"
#include "UI.h"

bool Enemy::step()
{
	//cout << "Enemy::step()" << endl;
    advanceEffects();

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
		checkSurroundings();
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
			if(pos_ + DELTA_POS[i] == current_player->getPosition())
				move_type_ = FOLLOW;
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
	UI::displayText(name_ + " defeated. Gained " + std::to_string(exp_reward_) + " Exp.");
	current_player->grantExp(exp_reward_);
	current_room->removeEnemy(this);
}

