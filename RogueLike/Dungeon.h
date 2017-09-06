#include "Types.h"
#include "Common.h"

class Room;

class Dungeon
{
private:
	string name_;
	unsigned int height_;
	unsigned int width_;
	map<string, Room*> loaded_rooms_;
	vector<vector<Room*>> layout_;
public:
	void changeRoom(Direction exit);
	void loadFromFile(string filename);
};

extern Dungeon* current_dungeon;