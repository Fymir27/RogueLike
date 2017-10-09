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
	vector<vector<char>> layout_char_;
public:
	void changeRoom(Direction exit);
	void loadFromFile(string filename);
	void generateLayout(size_t width, size_t height);
	void expand(size_t x, size_t y);
};

extern Dungeon* current_dungeon;