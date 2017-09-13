#include "Player.h"
#include "Room.h"

Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats, "../images/player.png")
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

	move(new_pos);
}

void Player::click(const sf::Event & event)
{
	inventory_->click(event);
}

Inventory* Player::getInventory()
{
	return inventory_;
}

void Player::step()
{
	if(invincible)
	{
		if(--invincibility_frames == 0)
		{
			invincible = false;
		}
	}
}

void Player::damage(const int amount)
{
	if(!invincible)
	{
		stats_.hp_ -= amount;
		invincible = true;
		invincibility_frames = 40;
	}
}
