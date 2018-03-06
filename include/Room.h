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
    TileMap* tile_map_ = NULL;
    Map map_; //Fields
    size_t width_;
    size_t height_;

    list<shared_ptr<Enemy>> enemies_;
    vector<vector<bool> > spawn_locations_;

    char biome_ = ' ';
    size_t room_count_in_dir_[4] = { 0, 0, 0, 0 };
    bool visited_for_room_count_ = false;
    size_t distance_from_spawn_ = 0;

    Position pos_; //Position in Dungeon
    Position door_pos_[4];
    Room* neighbours_[4] = {NULL, NULL, NULL, NULL};
    DijkstraMap2D* dm_player_ = NULL;

	list<shared_ptr<Effect>> effects_; //visual effects

    Field* getField(int x, int y);
    void initSpawnLocations();

public:
    Room(Position pos, size_t height);

    size_t getColCount()
    { return map_.front().size(); };

    size_t getRowCount()
    { return map_.size(); };

    void addField(Field* field);

    Field* getField(Position pos);

    Position getFreePosition();

    Position getDoorPosition(Direction dir);

    vector<vector<bool>> const& getSpawnLocations();

    Character* getCharacter(Position pos);

    void generate();

    Position getPathToPlayer(Position from);

    bool stepOn(Position to, Character* who, Position& new_pos); //returns new Position of Character
    void freeField(Position pos);
    void occupyField(Position pos, Character* who);

    void placeItem(Position pos, Item* item);

    void addNeighbour(Direction dir, Room* other);

    void addVisualEffect(shared_ptr<Effect>& e);
    void removeVisualEffect(shared_ptr<Effect>& e);

    size_t getEffectCount()
    { return effects_.size(); }
    void draw(sf::RenderWindow& window);

    void addEnemy(shared_ptr<Enemy> enemy);
    void stepEnemies();
    void spawnEnemy(string class_name, Position pos = {0,0});
    void spawnEnemies(size_t count);

    char getBiome()
    { return biome_; }
    void setBiome(char b)
    { biome_ = b; }
};

extern Room* current_room;