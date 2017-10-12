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
	/*static*/ vector<vector<vector<string>>> room_parts_;  // room_parts[height][section][random index]

public:
	void changeRoom(Direction exit);
	void loadFromFile(string filename);
	Room** getRoom(size_t x, size_t y);
	void connect(Room * from, Direction dir, Room * to);
	string * getRandomRoomParts();
	void generateLayout(size_t width, size_t height);
	void readRoomPartsFromFile();
};

extern Dungeon* current_dungeon;