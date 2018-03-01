#include "Player.h"
#include "Room.h"
#include "UI.h"
#include "Abilities.h"

#include "pugixml.hpp"
#include <fstream>

using namespace pugi;


Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats, string filename) : Character(name, pos, stats, filename)
{
    cout << "Spawning Player..." << endl;

    cout << "------- Testing XML Parsing ----------" << endl;
    std::ifstream file("test.xml");
    if(!file.is_open())
        cout << "Failed to open file" << endl;

    xml_document doc;
    xml_parse_result res = doc.load(file);
    cout << "Parsing result: " << res.description() << endl;
    cout << doc.child("enemy").attribute("class").value() << endl;
    cout << doc.child("enemy").child("stats").attribute("str").value() << endl;
    cout << doc.child("enemy").child("stats").attribute("dex").value() << endl;

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

