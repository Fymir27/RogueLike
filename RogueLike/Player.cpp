#include "Player.h"
#include "Room.h"

Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats, "../images/player.png")
{

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

/*
void Player::damage(const int amount)
{
	if(!invincible)
	{
		if(amount > stats_.hp_[CUR])
		{
			stats_.hp_[CUR] = 0;
			return;
		}
		stats_.hp_[CUR] -= amount;
		invincible = true;
		invincibility_frames = 40;
	}
}
*/
