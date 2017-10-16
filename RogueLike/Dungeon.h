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
	vector<vector<Room*>> layout_test_; //currently used
	///*static*/ vector<vector<vector<string>>> room_parts_;  // room_parts[height][section][random index]
	map<size_t, array<vector<vector<string>>, 3>> room_parts_;  // room_parts[height][section][random index][row]

public:
	void changeRoom(Direction exit);
	void loadFromFile(string filename);
	Room*& getRoom(size_t x, size_t y);
	void connect(Room * from, Direction dir, Room * to);
	//string * getRandomRoomParts();
	Room* Dungeon::generateRoom(size_t height)
	void generateLayout(size_t width, size_t height);
	void readRoomPartsFromFile();
};

extern Dungeon* current_dungeon;