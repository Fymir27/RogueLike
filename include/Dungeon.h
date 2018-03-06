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
	vector<vector<Room*>> layout_; //currently used
	map<size_t, RoomHeightClass> room_parts_;  // room_parts[height][section][random index][row]

    vector<char> available_biomes_ = { 'X', 'O', 'M', 'F', 'V'};
    void spread(Room* room, char biome, Direction from, size_t life_time);

    //counts how many rooms are behind each door except "from"
    //also sets distance from player
    size_t getRoomCountFromDirection(Room* room, Direction from, size_t prev_distance);

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