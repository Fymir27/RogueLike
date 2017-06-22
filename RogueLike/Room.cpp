#include "Room.h"
#include "Field.h"
#include "Floor.h"
#include "Wall.h"
#include <SFML\Graphics.hpp>

#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

Field* Room::getField(int x, int y)
{
	try
	{
		return rows_.at(y)->at(x);
	}
	catch (std::exception e)
	{
		std::cout << e.what();
	}
	return nullptr;
}

Room::Room()
{
}

Room::Room(const char * filename)
{
	readRoomFromFile(filename);
}

bool Room::readRoomFromFile(const char * filename)
{
	name = filename;
	std::fstream file(filename);
	if (!file.is_open())
		return false;

	Row* cur = new Row;
	char c;

	bool reading = true;
	while (reading)
	{
		c = file.get();
		switch(c)
		{
		case '/':
			rows_.push_back(cur);
			reading = false;
			break;

		case '#':
			cur->push_back(new Wall());
			break;

		case ' ':
			cur->push_back(new Floor());
			break;

		case '\n':
			rows_.push_back(cur);
			cur = new Row;
			break;
		}
	}
	return true;
}

void Room::draw(sf::RenderWindow& window)
{
	size_t rows = getRowCount();
	size_t cols = getColCount();

	//-- draw tiles --//
	sf::Color wall_color(0x80, 0x80, 0x80);
	sf::Color tile_color;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			tile_color = getField(x, y)->getColor();
			sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
			tile.setFillColor(tile_color);
			tile.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
			window.draw(tile);
		}
	}
}
