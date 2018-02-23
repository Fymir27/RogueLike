#pragma once
#include "Common.h"
#include "Field.h"
#include "Utils.h"
#include "TileMap.h"
#include "Enemy.h"

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
		size_t width_;
		size_t height_;
		list<Enemy*> enemies_;
        list<Enemy*> dead_enemies_;
		string       name;
		Position     pos_; //Position in Dungeon
		Position	 door_pos_[4];
		Room*        neighbours_[4] = { NULL, NULL, NULL, NULL };
		DijkstraMap2D* dm_player_ = NULL;
		vector < vector<bool> > spawn_locations_;

		Field* getField(int x, int y);

		void initSpawnLocations();

	public:
		Room(Position pos, size_t height);

		std::string getName() { return name; };
		size_t getColCount() { return map_.front().size(); };
		size_t getRowCount() { return map_.size(); };

		void addField(Field * field);
		Field* getField(Position pos);
		Position getFreePosition();
		Position getDoorPosition(Direction dir);
		vector<vector<bool>> const& getSpawnLocations();

        Character * getCharacter(Position pos);

		void generate();

		Position getPathToPlayer(Position from);
		vector<Position> getShortestPath(Position from, Position to);

		bool stepOn(Position to, Character* who, Position& new_pos); //returns new Position of Character
		void freeField(Position pos);
		void occupyField(Position pos, Character* who);
		void placeItem(Position pos, Item* item);

		void addNeighbour(Direction dir, Room * other);

		void draw(sf::RenderWindow& window);

		void addEnemy(Enemy* enemy);
		void removeEnemy(Enemy* enemy);
        void deleteDeadEnemies();
		void stepEnemies();
		void spawnEnemies(size_t count);
		Enemy* spawnEnemy(Position pos = Position(0,0), EnemyType type = RANDOM_ENEMY);
};

extern Room* current_room;