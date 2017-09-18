#include "Field.h"
#include "Room.h"
#include "Character.h"

Field::Field(Position pos, int tile_nr) : tile_nr_(tile_nr), pos_(pos), occupied_(false)
{
	
}

bool Field::stepOn(Character* who)
{
	if(occupied_)
	{
		cout << who->getName() << " is attacking " << character_->getName() << '!' << endl;
		character_->damage(10); //TODO: get attacker's damage
		return false;
	}
	else
	{
		//step on
		occupied_ = true;
		character_ = who;
		return true;
	}
}

bool Field::stepOff()
{
	occupied_ = false;
	character_ = NULL;
	return true;
}