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

void Dungeon::generateLayout(size_t width, size_t height)
{
	cout << "Generating Dungeon... (" << width << "x" << height << ")" << endl;
	layout_char_.resize(height);
	for (auto& row : layout_char_)
	{
		row.resize(width);
		for (char room : row)
		{
			room = '0';     //no room
		}
	}
	
	size_t margin_hor = rand() % width;  //left/right
	size_t margin_ver = rand() % height; //up/down

	cout << "Margins (hor/ver): " << margin_hor << "/" << margin_ver << endl;

	for (size_t y = 0; y < height; y++)
	{
		layout_char_.at(y).at(margin_hor) = '#';  //vertical line
		int dir_grow = roll(1, 2) ? 1 : -1;       //branch left or right
		size_t stack_count = 1;
		while (roll(1, 2))
		{
			size_t x = margin_hor + stack_count * dir_grow;

			if ((x < 0) || (x >= width))
				break;//grew too far

			layout_char_.at(y).at(x) = '#';
			stack_count++;
		}
	}

	for (size_t x = 0; x < width; x++)
	{
		layout_char_.at(margin_ver).at(x) = '#';  //horizontal line
		int dir_grow = roll(1, 2) ? 1 : -1;       //branch up or down
		size_t stack_count = 1;
		while (roll(1, 2))
		{
			size_t y = margin_ver + stack_count * dir_grow;

			if ((y < 0) || (y >= height)) 
				break;//grew too far
			
			layout_char_.at(y).at(x) = '#';
			stack_count++;
		}
	}

	for (auto& row : layout_char_)
	{
		for (char room : row)
			cout << room;
		cout << endl;
	}

	cout << endl;
}

void Dungeon::expand(size_t x, size_t y)
{
	static size_t room_count = 1;

	if (room_count >= 20)
		return;

	cout << "Dungeon::expand " << x << "|" << y << endl;

	size_t neighbour_count = 0;
	bool neighbours[4] = { false, false, false, false };

	for (size_t i = 0; i < 4; i++)
	{
		if (layout_char_.at(y + DELTA_Y[i]).at(x + DELTA_X[i]) == '#') //checks for neighbours in all directions
		{
			neighbour_count++;
			neighbours[i] = true;
		}
	}
	for (size_t i = 0; i < 4; i++)
	{
		if (!neighbours[i])
		{
			if (roll(3 - neighbour_count, room_count/2 + 2))
			{
				size_t new_x = x + DELTA_X[i]; //Position of new room
				size_t new_y = y + DELTA_Y[i];
				layout_char_.at(new_y).at(new_x) = '#';
				room_count++;
				expand(new_x, new_y);
			}
		}
	}
}

void Dungeon::changeRoom(Direction dir)
{
	Position pos = current_room->pos_;
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