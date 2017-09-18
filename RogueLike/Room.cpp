#include "Room.h"
#include "Field.h"
#include "Common.h"
#include "Types.h"
#include "Player.h"
#include "Enemy.h"

#include <fstream>

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

bool Room::isSolid(Position pos)
{
	Field* field = getField(pos.x_, pos.y_);
	if(!field)
		return true;
	return field->isSolid();
}

bool Room::stepOn (Position pos, Character* character, Direction dir)
{
	return getField(pos.x_, pos.y_)->stepOn(character);
}

bool Room::stepOff(Position pos, Character* character, Direction dir)
{
	return getField(pos.x_, pos.y_)->stepOff();
}

Room::Room()
{
	cout << "Room ctor" << endl;
}

Room::Room(const char * filename, Position pos) : pos_(pos)
{
	if(!readRoomFromFile(filename))
	{
		cout << "[Error] Room could not be loaded!" << endl;
		return;
	}

	cout << "Tile Nrs:" << endl;
	for(auto row : map_)
	{
		for(auto field : row)
			cout << field->getTileNr();
		cout << endl;
	}
	tile_map_ = new TileMap();
	tile_map_->load("../images/tileset.png", map_, TILE_SIZE, (int)getColCount(), (int)getRowCount());
}

bool Room::readRoomFromFile(const char * filename)
{
	cout << "Reading Room " << filename << " from file..." << endl;
	name = filename;
	std::fstream file(filename);
	if (!file.is_open())
		return false;

	Row row;
	char c;

	cout << "File opened successfully!" << endl;
	bool reading = true;
	Position pos = { 0,0 };
	while (reading)
	{
		c = file.get();
		Field* field = NULL;

		switch (c)
		{
		case '/':
			map_.push_back(row);
			reading = false;
			continue;
			break;

		case '#':
			field = new Wall(pos);
			break;

		case ' ':
			field = new Floor(pos);
			break;

		case '*':
			field = new Tree(pos);
			break;

		case '%':
			field = new Water(pos);
			break;

		case 'X':
			field = new Lava(pos);
			break;

		case '^':
			field = new Door(pos, UP);
			doors_[UP] = pos;
			break;

		case '>':
			field = new Door(pos, RIGHT);
			doors_[RIGHT] = pos;
			break;

		case 'v':
			field = new Door(pos, DOWN);
			doors_[DOWN] = pos;
			break;

		case '<':
			field = new Door(pos, LEFT);
			doors_[LEFT] = pos;
			break;

		case '\n':
			map_.push_back(row);
			row.clear();
			field = NULL;
			pos.x_ = 0;
			pos.y_++;
			break;

		default:
			cout << "[Error] Unknown Symbol!" << endl;
			break;
		}

		if (field)
		{
			pos.x_++;
			row.push_back(field);
		}
		cout << c;
		//cout << pos << endl;
	}
	cout << endl;
	return true;
}

void Room::draw(sf::RenderWindow& window)
{
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

Position Room::getEntryPosition(Direction last_exit)
{
	return entry_positions_.at(last_exit);
}

void Room::addEntryPosition(Direction dir, Position pos)
{
	entry_positions_.insert(pair<Direction, Position>(dir, pos));
}

void Room::movePlayerToDoor(Direction entry)
{
	current_player->pos_ = doors_[entry];
}

void Room::addEnemy(Enemy* enemy)
{
	enemies_.push_back(enemy);
}

void Room::removeEnemy(Enemy* enemy)
{
	enemies_.remove(enemy);
}

void Room::stepEnemies() //called appr. 63 times a second
{
	for(Enemy* enemy : enemies_)
	{
		enemy->step();
	}
}