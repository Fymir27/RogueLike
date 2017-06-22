#pragma once

#include "Field.h"

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>
typedef std::vector<Field*> Row;

const float TILE_SIZE = 32;

class Room
{
	private:
		std::vector<Row*> rows_;
		std::string name;

	public:
		std::string getName() { return name; };
		Field* getField(int x, int y);
		size_t getColCount() { return rows_.front()->size(); };
		size_t getRowCount() { return rows_.size(); };
		Room();
		Room(const char* filename);
		bool readRoomFromFile(const char* filename);
		void draw(sf::RenderWindow& window);
};