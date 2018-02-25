#include "Player.h"
#include "Room.h"
#include "UI.h"
#include "Abilities.h"

Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats, string filename) : Character(name, pos, stats, filename)
{
    cout << "Spawning Player..." << endl;
    ability_bar_.resize(3);
    ability_bar_[0] = new Fireball();
    ability_bar_[1] = new Regeneration();
    ability_bar_[2] = new SyphonSoul();
    cout << "Initiated Player abilites" << endl;

    //auto bpe = new BigParticleEffect(sf::Color::Red, 20);
    //static auto e = shared_ptr<Effect>(bpe->createInstance());
    //e->setPosition(worldToScreen(pos_));
    //Effect::addEffect(e, true);
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

Character* Player::findTarget(Direction dir)
{
    Character* target = NULL;
    Position pos = pos_;
    FIELD_STATUS status;
    do
    {
        pos = pos + DELTA_POS[dir];
        status = current_room->getField(pos)->getFieldStatus();

        //special case for doors
        if(status == TRIGGER)
        {
            for(size_t i = 0; i < 4; i ++)
            {
                if(current_room->getDoorPosition((Direction)i) == pos)
                {
                    status = SOLID;
                    break;
                }
            }
        }

    } while (status != SOLID && status != OCCUPIED);
    return current_room->getCharacter(pos);
}

void Player::draw(sf::RenderWindow &window)
{
    Character::draw(window);
}

void Player::coolDownAbilities()
{
    for(auto& ab : ability_bar_)
    {
        ab->coolDown();
    }
}

