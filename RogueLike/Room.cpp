#include "Room.h"
#include "Field.h"
#include "Common.h"
#include "Types.h"
#include "Player.h"
#include "EnemyClasses.h"
#include "DijkstraMap.h"
#include "Dungeon.h"

#include <fstream>
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
	return NULL;
}

Field* Room::getField(Position pos)
{
	try
	{
		return map_.at(pos.y_).at(pos.x_);
	}
	catch (std::exception e)
	{
		cout << "Invalid Field!" << endl;
		std::cout << e.what();
	}
	return NULL;
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
		for (int y = origin.y_ - 2; y <=  origin.y_ + 2; y++)
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

FIELD_STATUS Room::getFieldStatus(Position pos)
{
	try
	{
		return map_.at(pos.y_).at(pos.x_)->status_;
	}
	catch (std::exception e)
	{
		cout << "Invalid Field!" << endl;
		std::cout << e.what();
	}
}

Position Room::getFreePosition()
{
	Position pos;
	do
	{
		pos = Position(1 + (rand() % (width_ - 2)), 1 + (rand() % (height_ - 2)));
	} while(!spawn_locations_.at(pos.y_).at(pos.x_) || getField(pos)->status_== OCCUPIED);
	return pos;
}

Position Room::getDoorPosition(Direction dir)
{
	return door_pos_[dir];
}

vector<vector<bool>> const & Room::getSpawnLocations()
{
	return spawn_locations_;
}


void Room::generate()
{
	//cout << "Randomly generating Room" << Position(x, y) << endl;
	RoomHeightClass room_parts = current_dungeon->getRoomParts(height_);
	map_.resize(height_);
	for (size_t i = 0; i < 3; i++) //for every section
	{
		size_t r = rand() % room_parts[i].size();  //pick random part
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

	initSpawnLocations();
	//spawning enemies
	current_room = this;
	size_t prob = 3;
	while (roll(2, prob++)) { spawnEnemy(); }

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

	switch(new_field->status_)
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

//--- returns path from "from" to "to" (backwards!!) ---//
vector<Position> Room::getShortestPath(Position from, Position to)
{
	cout << "//-- Path finding start --//" << endl;
	try
	{
		size_t width = getColCount();
		size_t height = getRowCount();
		vector<vector<Node>> nodes(height);

		size_t x;
		size_t y;
		for (y = 0; y < height; y++)
		{
			nodes.at(y).resize(width);
			for (x = 0; x < width; x++)
			{
				Node* node = &nodes.at(y).at(x);
				node->pos = Position(x, y);
			}
		}

		static Position delta[4] = { Position(0, -1), Position(1, 0), Position(0, 1), Position(-1, 0) }; //order to look for neighbours

		//-- init --//
		Node* start = &nodes.at(from.y_).at(from.x_);
		start->distance = 0;

		while (true)
		{
			/*
			for (auto& line : nodes)
				for (auto& node : line)
					cout << node;
			*/


			//-- find node with shortest distance to from--//
			size_t shortest_distance = std::numeric_limits<size_t>::max();
			Node* new_node = NULL;

			for (y = 0; y < nodes.size(); y++)
			{
				for (x = 0; x < nodes.at(y).size(); x++)
				{
					Node* cur = &nodes.at(y).at(x);
					if (cur->visited)
					{
						continue;
					}
					else if (getField(x, y)->status_ == SOLID)
					{
						cur->visited = true;
					}
					else if (cur->distance < shortest_distance)
					{
						shortest_distance = cur->distance;
						new_node = cur;
					}
				} //for x
			} //for y

			if (new_node == NULL)
			{
				cout << "[Error] No path found!" << endl;
				return vector<Position>();
			}

			new_node->visited = true;

			//-- target reached; building path backwards --//
			if (new_node->pos == to)
			{
				cout << "Path found!" << endl;
				vector<Position> path;
				while (new_node->prev != NULL)
				{
					path.push_back(new_node->pos);
					new_node = new_node->prev;
				}
				for (auto pos : path)
					cout << pos;
				cout << endl;
				return path;
			}
			 
			//-- update distance for neighbours --//
			Position tmp;
			for (size_t i = 0; i < 4; i++)
			{
				tmp = new_node->pos + delta[i];
				if (tmp.x_ < 0)
					tmp.x_ = 0;
				if (tmp.y_ < 0)
					tmp.y_ = 0;
				Node* node = &nodes.at(tmp.y_).at(tmp.x_);
				if (!node->visited)
				{
					node->distance = new_node->distance + 1;
					node->prev = new_node;
				}
			}
		} //while

		return vector<Position>();
	}
	catch (std::out_of_range& e)
	{
		cout << "Path finding error" << endl;
		cout << e.what();
	}
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
	generate();
}

void Room::addNeighbour(Direction dir, Room* other)
{
	neighbours_[dir] = other;
	Position pos = door_pos_[dir];
	addField(new Door(pos, dir));

	/*
	Position pos;
	Position entry;
	switch (dir)
	{
	case UP:
		pos.x_ = getColCount() / 2;
		pos.y_ = 0;
		entry = pos;
		entry.y_ += 1;
		break;

	case RIGHT:
		pos.x_ = getColCount() - 1;
		pos.y_ = getRowCount() / 2;
		entry = pos;
		entry.x_ -= 1;
		break;

	case DOWN:
		pos.x_ = getColCount() / 2;
		pos.y_ = getRowCount() - 1;
		entry = pos;
		entry.y_ -= 1;
		break;

	case LEFT:
		pos.x_ = 0;
		pos.y_ = getRowCount() / 2;
		entry = pos;
		entry.x_ += 1;
		break;
	}
	map_.at(pos.y_).at(pos.x_) = new Door(pos, dir);
	entries_[dir] = entry;
	//tile_map_->load("../images/tileset.png", map_, TILE_SIZE, (int)getColCount(), (int)getRowCount());
	*/
}

void Room::draw(sf::RenderWindow& window)
{
	if (!tile_map_)
	{
		tile_map_ = new TileMap();
		tile_map_->load("../images/tileset.png", map_, TILE_SIZE, (int)getColCount(), (int)getRowCount());
	}
	window.draw(*tile_map_);
	for (auto row : map_)
	{
		for (auto field : row)
		{
			field->draw(window);
		}
	}
	for(auto enemy : enemies_)
	{
		enemy->draw(window);
	}
}

void Room::addField(Field* field)
{
	unsigned int x = field->pos_.x_;
	unsigned int y = field->pos_.y_;

	if (y >= map_.size() || x >= map_.at(y).size())
	{
		cout << "Can't add a field there!" << endl;
		return;
	}

	delete map_.at(y).at(x);
	map_.at(y).at(x) = field;

}

void Room::addEnemy(Enemy* enemy)
{
	enemies_.push_back(enemy);
}

void Room::removeEnemy(Enemy* enemy)
{
	enemies_.remove(enemy);
	//dead_enemies_.push_back(enemy);
}

void Room::stepEnemies() //called appr. 63 times a second
{
	if (!dm_player_)
		dm_player_ = new DijkstraMap2D(getColCount(), getRowCount(), current_player->getPosition());
	dm_player_->updateSource(current_player->getPosition());
	vector<Enemy*> dead_enemies;
	//cout << "Room::stepEnemies()" << endl;
	for(Enemy* enemy : enemies_)
	{
		if(!enemy->step())
		{
			dead_enemies.push_back(enemy);
		}
	}
	for(auto enemy : dead_enemies)
	{
		enemies_.remove(enemy);
		delete enemy;
	}
}

Enemy* Room::spawnEnemy(Position pos, EnemyType type)
{
	Enemy* e = NULL;

	if(pos == Position(0,0))
		pos = getFreePosition();

	if(type == RANDOM_ENEMY)
		type = (EnemyType)(1 + (rand() % (ENEMY_TYPE_COUNT)));

	switch(type)
	{
		case SPIDER:
		e = new Spider(pos);
		break;

		case GHOST:
		e = new Ghost(pos);
		break;

		case GOLEM:
		e = new Golem(pos);
		break;

		default:
		cout << "Unknown enemy type!" << endl;
		break;
	}

	return e;
}