#include "Room.h"
#include "Field.h"
#include "Floor.h"
#include "Wall.h"

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
