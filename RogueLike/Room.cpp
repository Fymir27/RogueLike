#include "Room.h"
#include "Field.h"
#include "Common.h"
#include "Types.h"

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

Room::Room()
{
	cout << "Room ctor" << endl;
}

Room::Room(const char * filename)
{
	readRoomFromFile(filename);
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
	cout << "Progress:" << endl;
	bool reading = true;
	while (reading)
	{
		c = file.get();
		Field* field = NULL;
		Position pos = { 0.0 };

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

		case '\n':
			map_.push_back(row);
			row.clear();
			field = NULL;
			pos.x_ = 0;
			pos.y_++;
			break;

		default:
			break;
		}

		if (field)
		{
			pos.x_++;
			row.push_back(field);
		}

		cout << c;
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
