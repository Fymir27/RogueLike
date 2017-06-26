#include "Player.h"
#include <iostream>
#include "Room.h"

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats)
{
	cout << this;
}

void Player::processInput(const sf::Event& event)
{
	cout << "Player::processInput" << endl;

	Position new_pos = pos_;
	//if(key == X)
	{
		new_pos.x_++;
	}
	Field* field = current_room->getField(new_pos.x_, new_pos.y_);
	if(!field)
		return;
	if(field->stepOn(this))
	{
		pos_ = new_pos;
	}
	cout << "Player moved to:" << endl;
	cout << pos_;
}
