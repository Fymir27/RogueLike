#include "Utils.h"
#include "Common.h"


class Room;

typedef vector<string> RoomPart;
typedef vector<RoomPart> RoomSection;
typedef array<RoomSection, 3> RoomHeightClass;

class Dungeon
{
private:
	string name_;
	unsigned int height_;
	unsigned int width_;
	map<string, Room*> loaded_rooms_;
	vector<vector<Room*>> layout_;
    list<Room*> rooms_sorted_;   // all the rooms sorted by distance to spawn in ascending order
	map<size_t, RoomHeightClass> room_parts_;  // room_parts[height][section][random index][row]

	void updateRoomInfo(Room* spawn);

public:
	void generateBiomes(Room* start);
	void printBiomes();

	void changeRoom(Direction exit);
	//void loadFromFile(string filename);
	Room*& getRoom(size_t x, size_t y);
	void connect(Room * from, Direction dir, Room * to);
    bool hasNeighbour(Position pos, Direction dir);
	//Room * generateRoom(size_t x, size_t y, size_t height = 0); // 0 = random
	size_t getRandomRoomHeight();
	//string * getRandomRoomParts();
	void generate(size_t width, size_t height);
	void readRoomPartsFromFile();
	RoomHeightClass const& getRoomParts(size_t height);

};

extern Dungeon* current_dungeon;