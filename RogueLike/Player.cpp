#include "Player.h"
#include <iostream>
#include "Room.h"

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats)
{
	if(!texture_.loadFromFile("../images/player.png"))
		return;

	sprite_.setTexture(texture_);	

	cout << this;
}

void Player::processInput(const sf::Event& event)
{
	cout << "Player::processInput" << endl;
	auto key = event.key.code;
	Position new_pos = pos_;
	if(key == sf::Keyboard::Right) new_pos.x_++;
	else if(key == sf::Keyboard::Left) new_pos.x_--;
	else if(key == sf::Keyboard::Up) new_pos.y_++;
	else if(key == sf::Keyboard::Down) new_pos.y_--;

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

void Player::draw(sf::RenderWindow& window)
{
	sprite_.setPosition(sf::Vector2f(pos_.x_ * TILE_SIZE, (current_room->getRowCount() - 1 - pos_.y_) * TILE_SIZE));
	window.draw(sprite_);
}


