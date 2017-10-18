#pragma once
#include "Common.h"
#include "Field.h"
#include "Types.h"
#include "TileMap.h"


class Enemy;
class TileMap;
class Item;
class DijkstraMap2D;
class Room
{
	friend class Dungeon;

	private:
		TileMap*     tile_map_ = NULL;
		Map          map_; //Fields
		list<Enemy*> enemies_;
		string       name;
		Position     pos_; //Position in Dungeon
		Position     entries_[4];
		Room*        neighbours_[4] = { NULL, NULL, NULL, NULL };
		DijkstraMap2D* dm_player_ = NULL;

		Field* getField(int x, int y);
		Field* getField(Position pos);

	public:
		std::string getName() { return name; };
		size_t getColCount() { return map_.front().size(); };
		size_t getRowCount() { return map_.size(); };

		FIELD_STATUS getFieldStatus(Position pos);

		Position getPathToPlayer(Position from);
		vector<Position> getShortestPath(Position from, Position to);
		void updateDistanceToPlayer();
		bool stepOn(Position to, Character* who, Position& new_pos); //returns new Position of Character
		void freeField(Position pos);
		void occupyField(Position pos, Character* who);
		void placeItem(Position pos, Item* item);


		Room(Position pos);

		void addNeighbour(Direction dir, Room * other);

		Room(const char* filename, Position pos = {0,0});

		bool readRoomFromFile(const char* filename);
		void draw(sf::RenderWindow& window);
		void addField(Field * field);

		Position getEntryPosition(Direction entry);
		void     addEntryPosition(Direction dir, Position pos);

		void movePlayerToDoor(Direction entry);

		void addEnemy(Enemy* enemy);
		void removeEnemy(Enemy* enemy);
		void stepEnemies();
};

extern Room* current_room;