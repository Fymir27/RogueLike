#include "Player.h"
#include "Room.h"
#include "UI.h"
#include "Abilities.h"

Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats, "../images/player.png")
{
    ability_bar_.resize(9);
    ability_bar_[0] = new Fireball();
    ability_bar_[1] = new Regeneration();
}

Inventory* Player::getInventory()
{
	return inventory_;
}


bool Player::move(Position new_pos)
{
	if (Character::move(new_pos)) //check if move is valid
	{ 
		return true;
	}
	return false;
}

void Player::rest()
{
	hp_ += (stats_.will_ / 10);
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


bool Player::castSpell(int nr, Character* target)
{
    try
    {
        Ability *ab = ability_bar_.at(nr - 1);

        size_t cost = ab->getCost();
        if (mana_ >= cost)
        {
            UI::displayText(name_ + " casts " + ab->getName() + " on " + target->getName() + ".");
            ab->cast(target);
            mana_ -= cost;
        } else
        {
            UI::displayText("Not enough Ressource!");
            return false;
        }
        return true;
    }
    catch(...)
    {
        UI::displayText("No such Spell!");
    }
}

