#include "Enemy.h"
#include "Room.h"
#include "Player.h"

bool Enemy::step()
{
	/* 
	static int steps = 0;
	if (steps < 63)
	{
		steps++;
		return;
	}
	steps = 0;
	*/

	if(stats_.hp_[CUR] <= 0)
		return false;

	static int x_delta[4] = {0,1,0,-1};
	static int y_delta[4] = {-1,0,1,0};
	static int i = 1;

	Position new_pos = {pos_.x_ + x_delta[i], pos_.y_ + y_delta[i]};

	while(current_room->isSolid(new_pos)) //look for direction to move
	{
		i = (i+1) % 4;
		new_pos = {pos_.x_ + x_delta[i], pos_.y_ + y_delta[i]};
	}
	move(new_pos);
	return true;
}

Enemy::Enemy(string name, Position pos, Stats stats, string texture) : Character(name, pos, stats, texture)
{
	current_room->addEnemy(this);
}

Enemy::~Enemy()
{
	current_room->removeEnemy(this);
}
