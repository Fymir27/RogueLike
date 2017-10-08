#include "Enemy.h"
#include "Room.h"
#include "Player.h"

bool Enemy::step()
{
	cout << "Enemy::step()" << endl;
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

	vector<Position> path_to_player;

	path_to_player = current_room->getShortestPath(pos_, current_player->getPosition()); //path is backwards!
	
	cout << "Path to player:" << endl;

	move(path_to_player.back());

	/*
	Position player_pos = current_player->getPosition();
	const int X = 0;
	const int Y = 1;
	Position delta[2];
	delta[X] = { player_pos.x_ - pos_.x_, 0 };
	delta[Y] = { 0, player_pos.y_ - pos_.y_ };
	int move_to = Y;

	cout << "delta X: " << delta[X] << endl;
	cout << "delta Y: " << delta[Y] << endl;

	if(abs(delta[X].x_) > abs(delta[Y].y_))
		move_to = X;

	cout << "Decision: " << ((bool)move_to ? "Y" : "X") << endl;

	//normalize
	if(delta[X].x_ != 0)
		delta[X].x_ /= abs(delta[X].x_);
	if(delta[Y].y_ != 0)
		delta[Y].y_ /= abs(delta[Y].y_);

	cout << "Norm. delta X: " << delta[X] << endl;
	cout << "Norm. delta Y: " << delta[Y] << endl;

	if(move(pos_ + delta[move_to]))
	{
		
		cout << "New pos: " << pos_ << endl;
	}
	else
	{
		move_to = (move_to + 1) % 2;
		cout << "Decision: " << ((bool)move_to ? "Y" : "X") << endl;
		if(!current_room->isSolid(pos_ + delta[move_to]))
		{
			move(pos_ + delta[move_to]);
			cout << "New pos: " << pos_ << endl;
		}
	}
	*/
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
