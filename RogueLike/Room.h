#pragma once

#include "Field.h"

#include <vector>
#include <string>
typedef std::vector<Field*> Row;

class Room
{
	private:
		std::vector<Row*> rows_;
		std::string name;

	public:
		std::string getName() { return name; };
		Field* getField(int x, int y);
		int getColCount() { return rows_.front()->size(); };
		int getRowCount() { return rows_.size(); };
		Room();
		Room(const char* filename);
		bool readRoomFromFile(const char* filename);
};