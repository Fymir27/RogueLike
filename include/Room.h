#pragma once

#include "Common.h"
#include "Field.h"
#include "Utils.h"
#include "TileMap.h"
#include "Enemy.h"
#include "Biome.h"

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
    int width_;
    int height_;

    list<shared_ptr<Enemy>> enemies_;
    vector<vector<bool> > spawn_locations_;

    shared_ptr<Biomes::Biome> biome_;
    size_t room_count_in_dir_[4] = { 0, 0, 0, 0 };
    size_t distance_from_spawn_ = 0;

    Position pos_; //Position in Dungeon
    Position door_pos_[4];
    Room* neighbours_[4] = {NULL, NULL, NULL, NULL};
    DijkstraMap2D* dm_player_ = NULL;

	list<shared_ptr<Effect>> effects_; //visual effects

    Field* getField(int x, int y);
    void initSpawnLocations();

public:
    //Room(Position pos, size_t height);
    Room(Position pos, int width = 22, int height = 20);

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

    void generateFromParts();
    void generate(bool generate_enemies = true);
    void makePath(Position from, Position to);

    /// widen paths by "bombing them"
    /// credit: http://www.darkgnosis.com/2018/03/03/contour-bombing-cave-generation-algorithm/
    void bombPaths(bool use_borders = true);

    void printToConsole();

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

};

extern Room* current_room;