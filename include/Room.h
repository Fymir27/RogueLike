#pragma once

#include "Common.h"
#include "Field.h"
#include "Utils.h"
#include "TileMap.h"
#include "Enemy.h"
#include "Biome.h"

class Enemy;
class TileMap;
class DijkstraMap2D;
class Lightmap;

namespace
{
    class Item;
}

class Room
{
    friend class Dungeon;

private:
    TileMap* tile_map_ = NULL;
    Map map_; //Fields
    unsigned width_;
    unsigned height_;

    list<shared_ptr<Enemy>> enemies_;
    vector<vector<bool> > spawn_locations_;

    shared_ptr<Biomes::Biome> biome_;
    size_t room_count_in_dir_[4] = { 0, 0, 0, 0 };
    size_t distance_from_spawn_ = 0;

    Position pos_; //Position in Dungeon
    Position door_pos_[4];
    Room* neighbours_[4] = {NULL, NULL, NULL, NULL};
    DijkstraMap2D* dm_player_ = NULL;

    Lightmap* lightmap_ = nullptr;
	list<shared_ptr<Effect>> effects_; //visual effects

    Field* getField(int x, int y);
    void initSpawnLocations();

public:
    //Room(Position pos, size_t height);
    Room(Position pos, unsigned width = 22, unsigned height = 20);

    size_t getColCount()
    { return map_.front().size(); };
    size_t getRowCount()
    { return map_.size(); };

    unsigned getWidth()
    { return width_; }

    unsigned getHeight()
    { return height_; }

    void addField(Field* field);

    Field* getField(Position pos);

    Position getFreePosition();

    Position getDoorPosition(Direction dir);

    vector<vector<bool>> const& getSpawnLocations();

    Character* getFirstCharacterInDirection(Position from, Direction dir);
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

    void placeItem(Position pos, shared_ptr<Items::Item> item, size_t count);

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

    Lightmap* getLightmap()
    { return lightmap_; }

};

extern Room* current_room;