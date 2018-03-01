#include "Enemy.h"
#include "Room.h"
#include "Player.h"
#include "UI.h"

bool Enemy::step()
{
    advanceEffects();
    if(dead())
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
	//current_room->addEnemy(this);
}

bool Enemy::checkSurroundings()
{
	for(size_t i = 0; i < 4; i++)
	{
		if(current_room->getField(pos_ + DELTA_POS[i])->getFieldStatus() == OCCUPIED)
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
		if(counter++ > 3) //trapped
		{
			move_type_ = SLEEP;
			break;
		}
	} while (!move(Position(pos_.x_ + DELTA_X[r], pos_.y_ + DELTA_Y[r])));
}

void Enemy::damage(unsigned int amount)
{
    Character::damage(amount);
    if(move_type_ == WAIT)
        move_type_ = FOLLOW;
}

Enemy::~Enemy()
{
	UI::displayText(name_ + " defeated. Gained " + std::to_string(exp_reward_) + " Exp.");
	current_player->grantExp(exp_reward_);
}

Enemy::Enemy(string name, string texture, Stats stats, size_t exp_reward, string attack_verb, float scaling[5], MoveType move_type) :
		Character(name, {0,0}, stats, texture), move_type_(move_type), exp_reward_(exp_reward), attack_verb_(attack_verb)
{
    std::copy(scaling, scaling + 5, scaling_);
}

void Enemy::attack(Character* target)
{
    auto damage = (unsigned)std::roundf(  stats_.str_ * scaling_[0] +
                                    stats_.end_ * scaling_[1] +
                                    stats_.dex_ * scaling_[2] +
                                    stats_.int_ * scaling_[3] +
                                    stats_.wil_ * scaling_[4] );

    UI::displayText(name_ + ' ' + attack_verb_ + ' ' + target->getName() + " for " + std::to_string(damage) + " damage.");
    target->damage(damage);
}

Enemy::Enemy(Enemy* orig) : Character(orig), move_type_(orig->move_type_),
                            exp_reward_(orig->exp_reward_), attack_verb_(orig->attack_verb_)
{
    std::copy(orig->scaling_, orig->scaling_ + 5, scaling_);
}



