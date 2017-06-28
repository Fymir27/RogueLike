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
	else if(key == sf::Keyboard::Up) new_pos.y_--;
	else if(key == sf::Keyboard::Down) new_pos.y_++;

	Field* field = current_room->getField(new_pos.x_, new_pos.y_);

	if(field == NULL)
		return;

	cout << "Trying to step on field" << new_pos << endl;
	if(field->stepOn(this))
	{
		cout << "Stepped on!" << endl;
		pos_ = new_pos;
	}
}

void Player::draw(sf::RenderWindow& window)
{
	sprite_.setPosition(sf::Vector2f(pos_.x_ * TILE_SIZE, pos_.y_ * TILE_SIZE));
	window.draw(sprite_);
}

Inventory& Player::getInventory() const
{
	return *inventory_;
}


