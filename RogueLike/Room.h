#pragma once

#include "Field.h"
#include "Types.h"
#include "TileMap.h"

#include "Common.h"

class TileMap;
class Room
{
	private:
		TileMap* tile_map_;
		Map map_;
		std::string name;

	public:
		std::string getName() { return name; };
		Field* getField(int x, int y);
		size_t getColCount() { return map_.front().size(); };
		size_t getRowCount() { return map_.size(); };
		Room();
		Room(const char* filename);
		bool readRoomFromFile(const char* filename);
		void draw(sf::RenderWindow& window);
};

extern Room* current_room;