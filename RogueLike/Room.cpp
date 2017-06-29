#include "Room.h"
#include "Field.h"
#include "Common.h"

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

		switch(c)
		{
		case '/':
			map_.push_back(row);
			reading = false;
			continue;
			break;

		case '#':
			field = new Wall();
			break;

		case ' ':
			field = new Floor();
			break;

		case '*':
			field = new Tree();
			break;

		case '%':
			field = new Water();
			break;

		case 'X':
			field = new Lava();
			break;

		case '\n':
			map_.push_back(row);
			row.clear();
			field = NULL;
			break;
		default:
			break;
		}

		if(field)
			row.push_back(field);
		cout << c;
	}
	cout << endl;
	return true;
}

void Room::draw(sf::RenderWindow& window)
{
	/*
	size_t rows = getRowCount();
	size_t cols = getColCount();

	//-- draw tiles --//
	sf::Sprite tile;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			tile.setTexture(getField(x,y)->getTexture());
			tile.setPosition(x*TILE_SIZE, y*TILE_SIZE);
			window.draw(tile);
		}
	}
	*/
	window.draw(*tile_map_);
}
