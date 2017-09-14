#pragma once

#include "Field.h"
#include "Types.h"
#include "TileMap.h"

#include "Common.h"

class Enemy;
class TileMap;
class Room
{
	friend class Dungeon;

	private:
		TileMap* tile_map_;
		Map map_; //Fields
		string name;
		Position pos_; //Position in Dungeon

		Position doors_[4];
		//-- maps last exit direction to entry position --//
		map<Direction, Position> entry_positions_;

		list<Enemy*> enemies_;


	public:
		std::string getName() { return name; };
		Field* getField(int x, int y);
		size_t getColCount() { return map_.front().size(); };
		size_t getRowCount() { return map_.size(); };
		Room();
		Room(const char* filename, Position pos = {0,0});
		bool readRoomFromFile(const char* filename);
		void draw(sf::RenderWindow& window);
		void addField(Field * field);

		Position getEntryPosition(Direction last_exit);
		void     addEntryPosition(Direction dir, Position pos);

		void movePlayerToDoor(Direction entry);

		void addEnemy(Enemy* enemy);
		void removeEnemy(Enemy* enemy);
		void stepEnemies();
};

extern Room* current_room;