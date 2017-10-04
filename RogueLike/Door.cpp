#include "Door.h"
#include "Field.h"
#include "Dungeon.h"
#include "Room.h"
#include "Character.h"

Door::Door(Position pos, Direction dir) : Field(pos, 5, TRIGGER), dir_(dir)
{

}

Position Door::trigger(Character* character)
{
	cout << "Stepped on a Door!" << endl;
	current_room->freeField(character->getPosition());
	current_dungeon->changeRoom(dir_);
	int entry = dir_;
	entry = (entry + 2) % 4; //opposite entry
	return current_room->getEntryPosition((Direction)entry);
}