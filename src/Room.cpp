#include "Room.h"
#include "Field.h"
#include "Common.h"
#include "Utils.h"
#include "Player.h"
#include "DijkstraMap.h"
#include "Dungeon.h"
#include "Effects.h"
#include "Lightmap.h"
#include "Item.h"

#include <sstream>

Room* current_room = NULL;

Field* Room::getField(int x, int y)
{
    try
    {
        return map_.at(y).at(x);
    }
    catch (std::exception e)
    {
        cout << "Invalid Field!" << endl;
        std::cout << e.what();
    }
    return nullptr;
}

Field* Room::getField(Position pos)
{
    return getField(pos.x_, pos.y_);
}

void Room::initSpawnLocations()
{
    spawn_locations_.resize(map_.size());
    for (size_t y = 0; y < map_.size(); y++)
    {
        for (size_t x = 0; x < map_[y].size(); x++)
        {
            Field* f = getField(x, y);
            if (f->status_ == FREE)
            {
                spawn_locations_[y].push_back(true);
            }
            else
            {
                spawn_locations_[y].push_back(false);
            }
        }
    }

    for (size_t i = 0; i < 4; i++)
    {

        if (neighbours_[i] == NULL)
            continue;

        Position origin = door_pos_[i];
        for (int y = origin.y_ - 2; y <= origin.y_ + 2; y++)
        {
            for (int x = origin.x_ - 2; x <= origin.x_ + 2; x++)
            {
                try
                {
                    spawn_locations_.at(y).at(x) = false;
                }
                catch (std::out_of_range&)
                {
                    continue;
                }
            }
        }
    }

    /*
    for (auto row : spawn_locations_)
    {
        for (bool spawnable : row)
        {
            cout << spawnable;
        }
        cout << endl;
    }
    */

}


Position Room::getFreePosition()
{
    Position pos;
    do
    {
        pos = Position(1 + (random_engine() % (width_ - 2)), 1 + (random_engine() % (height_ - 2)));
    } while (!spawn_locations_.at(pos.y_).at(pos.x_) || getField(pos)->status_ == OCCUPIED);
    return pos;
}

Position Room::getDoorPosition(Direction dir)
{
    return door_pos_[dir];
}

vector<vector<bool>> const& Room::getSpawnLocations()
{
    return spawn_locations_;
}

Character* Room::getCharacter(Position pos)
{
    return getField(pos)->character_;
}


void Room::generateFromParts()
{
    //cout << "Randomly generating Room" << Position(x, y) << endl;
    RoomHeightClass room_parts = current_dungeon->getRoomParts(height_);
    map_.resize(height_);
    for (size_t i = 0; i < 3; i++) //for every section
    {
        size_t r = random_engine() % room_parts[i].size();  //pick random part
        vector<string> const& part = room_parts[i][r];
        for (size_t y = 0; y < height_; y++)
        {
            string const& line = part.at(y);
            for (size_t x = 0; x < part[y].size(); x++)
            {
                char c = line.at(x);
                //cout << c;
                size_t room_x = map_.at(y).size();
                switch (c)
                {
                    case '#':
                        map_.at(y).push_back(new Wall(Position(room_x, y)));
                        break;

                    case '.':
                        map_.at(y).push_back(new Floor(Position(room_x, y)));
                        break;

                    case '^':
                        door_pos_[UP] = Position(room_x, y);
                        map_.at(y).push_back(new Wall(Position(room_x, y)));
                        break;

                    case '>':
                        door_pos_[RIGHT] = Position(room_x, y);
                        map_.at(y).push_back(new Wall(Position(room_x, y)));
                        break;

                    case 'v':
                        door_pos_[DOWN] = Position(room_x, y);
                        map_.at(y).push_back(new Wall(Position(room_x, y)));
                        break;

                    case '<':
                        door_pos_[LEFT] = Position(room_x, y);
                        map_.at(y).push_back(new Wall(Position(room_x, y)));
                        break;

                    default: //WUT?
                        map_.at(y).push_back(new Lava(Position(room_x, y)));
                        break;
                }
            }
        }
    }
    width_ = getColCount();
}


bool Room::stepOn(Position to, Character* who, Position& new_pos)
{
    Position old_pos = who->getPosition();
    Field* new_field = getField(to.x_, to.y_);
    Field* old_field = getField(old_pos.x_, old_pos.y_);
    Room* old_room = current_room;

    /*
    for (auto line : map_)
    {
        for (auto field : line)
            cout << field->status_;
        cout << endl;
    }
    */

    if (new_field == NULL)
    {
        cout << "new_field == NULL!" << endl;
        return false;
    }

    switch (new_field->status_)
    {
        case SOLID:
            return false;

        case FREE:
            old_field->free();
            new_field->occupy(who);
            new_pos = to;
            return true;

        case OCCUPIED:
            who->attack(new_field->getCharacter());
            new_pos = who->getPosition();
            return true;

        case TRIGGER:
            old_field->free();
            new_pos = new_field->trigger(who);
            new_field = current_room->getField(new_pos);
            new_field->occupy(who);
            if (current_room == old_room)
                return true;
            else
                return false; //register as non valid move so enemies dont move as well

        case PICKUP:
            old_field->free();
            new_field->occupy(who);
            new_field->pickUpItem(who);
            new_pos = to;
            return true;
    }
    return false;
}

Position Room::getPathToPlayer(Position from)
{
    if (dm_player_ == NULL)
        dm_player_ = new DijkstraMap2D(getColCount(), getRowCount(), current_player->getPosition());
    return dm_player_->getNextPosition(from);
}

void Room::freeField(Position pos)
{
    getField(pos.x_, pos.y_)->free();
}

void Room::occupyField(Position pos, Character* who)
{
    getField(pos.x_, pos.y_)->occupy(who);
}

void Room::placeItem(Position pos, shared_ptr<Items::Item> item, size_t count)
{
    getField(pos.x_, pos.y_)->placeItem(item, count);
}

/*
Room::Room(Position pos, size_t height) : pos_(pos), height_(height)
{
    generateFromParts();
}
 */

void Room::addNeighbour(Direction dir, Room* other)
{
    neighbours_[dir] = other;
    Position pos = door_pos_[dir];
    addField(new Door(pos, dir));
}

void Room::draw(sf::RenderWindow& window)
{
    if (tile_map_ == nullptr)
    {
        tile_map_ = new TileMap();
        tile_map_->load(biome_->texture_.c_str(), map_, TILE_SIZE, (int) getColCount(), (int) getRowCount());
    }
    window.draw(*tile_map_);

    //TODO: let items draw themselves
    for (auto& row : map_)
    {
        for (auto& field : row)
        {
            field->draw(window);
        }
    }

    for (auto& enemy : enemies_)
    {
        enemy->draw(window);
    }

    auto tmp_effects = effects_;
    for (auto& e : tmp_effects)
    {
        e->update();

        if (e->isActive())
        {
            window.draw(*e);
        }
        else
        {
            effects_.remove(e);
        }
    }
}


void Room::addField(Field* field)
{
    try
    {
        int x = field->pos_.x_;
        int y = field->pos_.y_;

        if (y >= map_.size() || x >= map_.at(y).size())
        {
            cout << "Can't add a field there!" << endl;
            return;
        }

        delete map_.at(y).at(x);
        map_.at(y).at(x) = field;
    }
    catch (std::out_of_range& e)
    {
        cout << e.what() << endl;
    }
}

void Room::addEnemy(shared_ptr<Enemy> enemy)
{
    enemies_.push_back(enemy);
}

void Room::stepEnemies()
{
    if (dm_player_ == nullptr)
        dm_player_ = new DijkstraMap2D(getColCount(), getRowCount(), current_player->getPosition());
    dm_player_->updateSource(current_player->getPosition());


    for (auto e = enemies_.begin(); e != enemies_.end();)
    {
        if (!(*e)->step()) // if dead
            e = enemies_.erase(e);
        else
            e++;
    }

}

void Room::spawnEnemy(string class_name, Position pos)
{
    if (pos == Position(0, 0))
        pos = getFreePosition();

    Field* spawn = getField(pos);
    if (spawn->status_ != FREE)
    {
        cout << "You can't spawn an enemy here! " << pos << " status:" << spawn->status_ << endl;
        return;
    }

    auto factory = Factory<Enemy>::get();
    auto enemy = factory->createEntity(class_name);
    spawn->occupy(enemy.get());
    enemy->setPosition(pos);
    enemies_.push_back(enemy);
}

void Room::spawnEnemies(size_t count)
{
    cout << "Spawning " << count << " enemies..." << endl;
    auto factory = Factory<Enemy>::get();
    auto enemy_classes = factory->getEntityNames();
    size_t r;
    string class_name;

    for (size_t i = 0; i < count; i++)
    {
        bool satisfied = false;
        while(!satisfied) // find enemy that can spawn in this room
        {
            r = random_engine() % enemy_classes.size();;
            class_name = enemy_classes.at(r);
            auto template_enemy = factory->getTemplateEntity(class_name);

            satisfied = true;
            for(auto& condition : template_enemy->getSpawnConditions())
            {
                if(!condition->isSatisfiedBy(*biome_))
                {
                    satisfied = false;
                    enemy_classes.erase(enemy_classes.begin() + r);
                    break;
                }
            }
        }
        spawnEnemy(class_name);
    }
}

void Room::addVisualEffect(shared_ptr<Effect>& e)
{
    effects_.push_back(shared_ptr<Effect>(e));
}

void Room::removeVisualEffect(shared_ptr<Effect>& e)
{
    effects_.remove(shared_ptr<Effect>(e));
}

void Room::generate(bool generate_enemies)
{
    Position center = { width_ / 2, height_ / 2 };
    //connect doors
    for (size_t i = 0; i < 4; ++i)
    {
        if(neighbours_[i] != nullptr)
        {
            // + DELTA so door itself doesnt get turned into path
            makePath(door_pos_[i] + DELTA_POS[opposite(static_cast<Direction>(i))], center);
        }
    }

    //randomly spawn an item
    if(roll(1, 3))
    {
        Position item_pos = { getRandomBetween((unsigned)2, width_ - 3), getRandomBetween((unsigned)2, height_ - 3)};
        cout << "Spawning item! " << item_pos << endl;
        auto field = new Floor(item_pos); // in case there was a wall
        addField(field);
        makePath(item_pos, center);

        auto factory = Factory<Items::Item>::get();
        auto item_names = factory->getEntityNames();
        size_t item_nr = random_engine() % item_names.size();
        auto item_name = item_names.at(item_nr);
        auto item = factory->createEntity(item_name);
        field->placeItem(item, 1 + random_engine() % 1);
    }

    bombPaths();

    initSpawnLocations();
    if(generate_enemies)
        spawnEnemies((size_t)getRandomBetween(1,2));
}

void Room::makePath(Position from, Position to)
{
    Position step_x, step_y = {0, 0};

    if(from.x_ != to.x_) step_x = (from.x_ < to.x_) ? DELTA_POS[RIGHT] : DELTA_POS[LEFT];
    if(from.y_ != to.y_) step_y = (from.y_ < to.y_) ? DELTA_POS[DOWN]  : DELTA_POS[UP];

    addField(new Floor(from));
    while(from!= to)
    {
        if((from.x_ != to.x_) && roll(1,2))
        {
            from = from + step_x;
        }
        else if(from.y_ != to.y_)
        {
            from = from + step_y;
        }
        addField(new Floor(from));
    }
}

void Room::printToConsole()
{
    cout << endl << "Room " << pos_ << endl;
    for(auto const& row : map_)
    {
        for (auto const& field : row)
        {
            switch(field->tile_nr_)
            {
                case 0: cout << '#'; break;
                case 1: cout << '.'; break;
                case 2: cout << 'X'; break;
                default: cout << '?'; break;
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout << endl;
}


void Room::bombPaths( bool use_borders)
{
    cout << "Bombing paths..." << endl;
    vector<Field*> candidates;
    for(auto row : map_)
    {
        for(auto field : row)
        {
            if(field->tile_nr_ == 1) //FLOOR
                candidates.push_back(field);
        }
    }
    
    //shuffle candiates
    std::random_shuffle(candidates.begin(), candidates.end()/*, random_engine*/);

    size_t iteration_count = std::round(candidates.size() * 2.d);
    //cout << candidates.size() << " canditadtes" << endl;

    for (size_t i = 0; i < iteration_count; ++i)
    {
        //cout << "Iteration count: " << i << "/" << iteration_count << endl;
        size_t candidate_id;

        // 1/3 chance that we will use as a bombing point one of the last 15 positions
        if (roll(1,3))
        {
            //cout << "Taking newer cells!" << endl;
            candidate_id = getRandomBetween(candidates.size() - 15, candidates.size() - 1);
        }
        else // otherwise use lower half of remaining tiles
        {
            //cout << "Taking older cells!" << endl;
            candidate_id = getRandomBetween<size_t>(0, candidates.size() / 2);
        }

        // check boundaries
        if (candidate_id >= candidates.size())  //unsigned, so now lower check needed
            candidate_id = candidates.size() - 1;

        int bomb_radius = 1; //roll(1,50) ? 1 : 2; //small chance of bigger radius

        //cout << "bomb radius: " << bomb_radius << endl;

        Position bomb_pos = candidates[candidate_id]->pos_;

        // step by step prompt:
        //cout << "Bomb?" << endl;
        //string s;
        //std::getline(cin, s);

        // bomb
        for (int y = std::max(0, bomb_pos.y_ - bomb_radius - 1); y < std::max((int)height_, bomb_pos.y_ + bomb_radius); y++)
        {
            for (int x = std::max(0, bomb_pos.x_ - bomb_radius - 1); x < std::max((int)width_, bomb_pos.x_ + bomb_radius); x++)
            {
                //cout << "Checking " << Position(x,y) << endl;
                //circle collision check
                if ((x - bomb_pos.x_)*(x - bomb_pos.x_) + (y - bomb_pos.y_)*(y - bomb_pos.y_) < bomb_radius * bomb_radius /*TODO: remove?*/ + bomb_radius)
                {
                    //cout << "hit!" << endl;
                    int tmp_x = clamp(x, use_borders ? 1 : 0, use_borders ? (int)width_  - 2 : (int)width_  - 1);
                    int tmp_y = clamp(y, use_borders ? 1 : 0, use_borders ? (int)height_ - 2 : (int)height_ - 1);

                    if(getField(tmp_x, tmp_y)->tile_nr_ == 0) //we hit a wall!
                    {
                        auto field = new Floor(Position(tmp_x, tmp_y));
                        addField(field);
                        candidates.push_back(field);
                    }
                }
            } // for x
        } // for y
        //erase bombed cell
        candidates.erase(candidates.begin() + candidate_id);
        //printToConsole();
    } // for i
} // bombPaths()

Room::Room(Position pos, unsigned width, unsigned height) : pos_(pos), width_(width), height_(height)
{
    lightmap_ = new Lightmap(this);

    try
    {
        // start with wall everywhere
        map_.resize(height_);
        for (size_t y = 0; y < height_; ++y)
        {
            for (size_t x = 0; x < width_; ++x)
            {
                map_.at(y).push_back(new Wall({(int) x, (int) y}));
            }
        }

        // pick Door Positions
        Position door_pos;
        int      door_margin = 5;

        door_pos = {getRandomBetween(door_margin, (int) width_ - door_margin), 0};
        door_pos_[UP] = door_pos;

        door_pos = {(int) width_ - 1, getRandomBetween(door_margin, (int) height_ - door_margin)};
        door_pos_[RIGHT] = door_pos;

        door_pos = {getRandomBetween(door_margin, (int) width_ - door_margin), (int) height_ - 1};
        door_pos_[DOWN] = door_pos;

        door_pos = {0, getRandomBetween(door_margin, (int) width_ - door_margin)};
        door_pos_[LEFT] = door_pos;
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }
}

Character* Room::getFirstCharacterInDirection(Position from, Direction dir)
{
    Field* field = nullptr;
    while(true)
    {
        from = from  + DELTA_POS[dir];
        field = getField(from);
        if(field->character_ != nullptr)
            return field->character_;
        if(field->status_ == SOLID)
            break;
    }
    return nullptr;
}






