#include "Enemy.h"
#include "Room.h"
#include "Player.h"

void Enemy::step()
{
	//-- collision check --//
	Position p_pos = current_player->getPosition();
	if(p_pos == pos_)
	{
		current_player->damage(10);
	}

	static int steps = 0;
	if (steps < 63)
	{
		steps++;
		return;
	}
	steps = 0;

	static int x_delta[4] = {0,1,0,-1};
	static int y_delta[4] = {-1,0,1,0};
	static int i = 1;

	Position new_pos = {pos_.x_ + x_delta[i], pos_.y_ + y_delta[i]};

	while(!move(new_pos))
	{
		i = (++i) % 4;
		new_pos = {pos_.x_ + x_delta[i], pos_.y_ + y_delta[i]};
	}
}

Enemy::Enemy(string name, Position pos, Stats stats, string texture) : Character(name, pos, stats, texture)
{
	current_room->addEnemy(this);
}
