#include "Player.h"
#include "Room.h"

Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats)
{

}

void Player::processInput(const sf::Event& event)
{
	auto key = event.key.code;
	Position new_pos = pos_;
	if     (key == sf::Keyboard::Right) new_pos.x_++;
	else if(key == sf::Keyboard::Left)  new_pos.x_--;
	else if(key == sf::Keyboard::Up)    new_pos.y_--;
	else if(key == sf::Keyboard::Down)  new_pos.y_++;

	Field* field = current_room->getField(new_pos.x_, new_pos.y_);

	if(field == NULL)
		return;

	if(field->stepOn(this))
	{
		pos_ = new_pos;
	}
}

void Player::click(const sf::Event & event)
{
	inventory_->click(event);
}

Inventory* Player::getInventory()
{
	return inventory_;
}
