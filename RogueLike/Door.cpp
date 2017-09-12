#include "Door.h"
#include "Field.h"
#include "Dungeon.h"

Door::Door(Position pos, Direction dir) : Field(pos, 1), dir_(dir)
{

}

bool Door::stepOn(Character* character)
{
	cout << "Stepped on a Door!" << endl;
	current_dungeon->changeRoom(dir_);
	return false;
}