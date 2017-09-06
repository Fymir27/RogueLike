#include "Dungeon.h"
#include "Room.h"

#include <fstream>

Dungeon* current_dungeon = NULL;

void Dungeon::loadFromFile(string filename)
{
	cout << "Reading Dungeon " << filename << " from file..." << endl;
	std::fstream file(filename);
	if (!file.is_open())
	{
		cout << "[Error] Dungeon could not be loaded!" << endl;
		return;
	}

	cout << "File opened successfully!" << endl;

	file >> height_;
	file >> width_;

	string starting_room;
	file >> starting_room;

	vector<Room*> row;
	string room_name;
	Room* room;
	Position pos = {0,0};

	for(size_t y = 0; y < height_; y++)
	{
		pos.x_ = 0;
		for(size_t x = 0; x < width_; x++)
		{
			file >> room_name;

			if(room_name.compare("-"))
			{
				room = new Room(room_name.c_str(), pos);
				loaded_rooms_.insert(pair<string, Room*>(room_name, room));
				row.push_back(room);
			}
			else
			{
				row.push_back(NULL);
			}
			pos.x_++;
		}
		layout_.push_back(row);
		row.clear();
		pos.y_++;
	}
	current_room = loaded_rooms_[starting_room];
}

void Dungeon::changeRoom(Direction dir)
{
	cout << "Changing Room in direction " << dir << endl;
	Position pos = current_room->pos_;
	cout << "Current pos of Room: " << pos << endl;

	Direction entry; //Entry point of char

	switch(dir)
	{
		case UP:
		pos.y_--;
		entry = DOWN;
		break;

		case DOWN:
		pos.y_++;
		entry = UP;
		break;

		case LEFT:
		pos.x_--;
		entry = RIGHT;
		break;

		case RIGHT:
		pos.x_++;
		entry = LEFT;
		break;

		default:
		cout << "Dungeon::changeRoom: Unknown direction!" << endl;
		break;
	}

	cout << "New Room is at Pos. " << pos << " in Dungeon!" << endl; 
	Room* new_room = layout_.at(pos.y_).at(pos.x_);
	current_room = new_room;
	current_room->movePlayerToDoor(entry);
}