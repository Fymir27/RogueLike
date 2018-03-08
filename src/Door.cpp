#include "Field.h"
#include "Dungeon.h"
#include "Room.h"
#include "Character.h"
#include "Player.h"

Door::Door(Position pos, Direction dir) : Field(pos, 2, TRIGGER), dir_(dir)
{

}

Position Door::trigger(Character* character)
{
	if(character != current_player)
		return character->getPosition();

	current_room->freeField(character->getPosition());
	current_dungeon->changeRoom(dir_);
	return current_room->getDoorPosition(opposite(dir_)) + DELTA_POS[dir_];
}

void Door::free()
{
	status_ = TRIGGER;
	character_ = NULL;
}