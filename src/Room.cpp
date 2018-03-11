#include "Room.h"
#include "Field.h"
#include "Common.h"
#include "Utils.h"
#include "Player.h"
#include "DijkstraMap.h"
#include "Dungeon.h"
#include "Effects.h"

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

struct Node
{
    Position pos;
    bool visited = false;
    size_t distance = std::numeric_limits<size_t>::max();
    Node* prev = NULL;
};

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    out << node.pos << "[" << node.visited << "], " << node.distance << endl;
    return out;
};

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

void Room::placeItem(Position pos, Item* item)
{
    getField(pos.x_, pos.y_)->placeItem(item);
}

Room::Room(Position pos, size_t height) : pos_(pos), height_(height)
{
    generateFromParts();
}

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

void Room::generate()
{
    try
    {
        width_ = 20;
        height_ = 20;
        // start with wall everywhere
        map_.resize(height_);
        for (size_t y        = 0; y < height_; ++y)
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
        //addField(new Door(door_pos, UP));

        door_pos = {(int) width_ - 1, getRandomBetween(door_margin, (int) height_ - door_margin)};
        door_pos_[RIGHT] = door_pos;
        //addField(new Door(door_pos, RIGHT));

        door_pos = {getRandomBetween(door_margin, (int) width_ - door_margin), (int) height_ - 1};
        door_pos_[DOWN] = door_pos;
        //addField(new Door(door_pos, DOWN));

        door_pos = {0, getRandomBetween(door_margin, (int) width_ - door_margin)};
        door_pos_[LEFT] = door_pos;
        //addField(new Door(door_pos, LEFT));

        //connect doors
        makePath(door_pos_[UP]    + DELTA_POS[DOWN], door_pos_[DOWN] + DELTA_POS[UP]);
        makePath(door_pos_[RIGHT] + DELTA_POS[LEFT], door_pos_[LEFT] + DELTA_POS[RIGHT]);

        bombPaths();

    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }
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


void Room::bombPaths()
{
    vector<Field*> candidates;
    for(auto row : map_)
    {
        for(auto field : row)
        {
            candidates.push_back(field);
        }
    }
    
    //shuffle candiates
    std::random_shuffle(candidates.begin(), candidates.end()/*, random_engine*/);

    //TODO: finish: http://www.darkgnosis.com/2018/03/03/contour-bombing-cave-generation-algorithm/

}




