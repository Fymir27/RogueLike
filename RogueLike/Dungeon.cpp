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

Room** Dungeon::getRoom(size_t x, size_t y)
{
	return &layout_test_.at(y).at(x);
}

void Dungeon::connect(Room* from, Direction dir, Room* to)
{
	from->addNeighbour(dir, to);
	to->addNeighbour(Direction((dir + 2) % 4), from);
}

void Dungeon::generateLayout(size_t width, size_t height)
{
	cout << "Generating Dungeon... (" << width << "x" << height << ")" << endl;

	//-- Setting the size --//
	layout_test_.resize(height);
	for (auto& row : layout_test_)
	{
		row.resize(width);
		for (auto& room : row)
		{
			room = NULL;     //no rooms anywhere
		}
	}
	
	//-- Create two crossing lines and branch out from them randomly --//
	//TODO: add more lines the bigger the room is
	size_t margin_hor = rand() % width;  //from left
	size_t margin_ver = rand() % height; //from top

	cout << "Margins (hor/ver): " << margin_hor << "/" << margin_ver << endl;

	Room** tmp = NULL;

	for (size_t y = 0; y < height; y++)
	{
		tmp = getRoom(margin_hor, y);            //vertical line
		*tmp = new Room(Position(margin_hor, y));
		if (y > 0)
		{
			connect(*tmp, UP, *getRoom(margin_hor, y - 1));
		}
		int dir_grow = roll(1, 2) ? 1 : -1;       //branch left or right
		size_t stack_count = 1;
		while (roll(1, 2)) //grow branch
		{
			size_t x = margin_hor + stack_count * dir_grow;

			if ((x < 0) || (x >= width))
				break; //too far

			tmp = getRoom(x, y);
			if (*tmp == NULL)
				*tmp = new Room(Position(x, y));
			connect(*tmp, (dir_grow == 1 ? LEFT : RIGHT), *getRoom(x + (dir_grow * -1), y));
			stack_count++;
		}
	}

	for (size_t x = 0; x < width; x++)
	{
		tmp = getRoom(x, margin_ver);            //horizontal line
		if(*tmp == NULL)
			*tmp = new Room(Position(x, margin_ver));
		if (x > 0)
		{
			connect(*tmp, LEFT, *getRoom(x - 1, margin_ver));
		}
		int dir_grow = roll(1, 2) ? 1 : -1;       //branch up or down
		size_t stack_count = 1;
		while (roll(1, 2)) //grow branch
		{
			size_t y = margin_ver + stack_count * dir_grow;

			if ((y < 0) || (y >= height))
				break; //too far

			tmp = getRoom(x, y);
			if (*tmp == NULL)
				*tmp = new Room(Position(x,y));
			connect(*tmp, (dir_grow == 1 ? UP : DOWN), *getRoom(x, y + (dir_grow * -1)));

			stack_count++;
		}
	}

	current_room = *getRoom(6, 0);

	/*
	for (auto row : layout_test_)
	{
		for (auto room : row)
			if (room == NULL)
				cout << ' ';
			else
				cout << '#';
		cout << endl;
	}
	*/

	//-- DEBUG --//
	for (auto& row : layout_test_)
	{
		for (Room* room : row)
		{
			if (room != NULL)
				cout << ' ' << (char)(room->neighbours_[UP] ? '|' : ' ') << ' ';
			else
				cout << "...";
		}
		cout << endl;
		for (auto room : row)
		{
			if (room != NULL)
				cout << (char)(room->neighbours_[LEFT] ? '-' : ' ') << '#' << (char)(room->neighbours_[RIGHT] ? '-' : ' ');
			else
				cout << "...";
		}
		cout << endl;
		for (auto room : row)
		{
			if (room)
				cout << ' ' << (char)(room->neighbours_[DOWN] ? '|' : ' ') << ' ';
			else
				cout << "...";
		}
		cout << endl;
	}
	cout << endl;
}

void Dungeon::readRoomPartsFromFile()
{
	string filename[3];
	filename[0] = "room_parts_left.txt";
	filename[1] = "room_parts_middle.txt";
	filename[2] = "room_parts_right.txt";

	room_parts_.resize(1);

	std::ifstream file;
	for (size_t i = 0; i < 3; i++)
	{
		file.open(filename[i]);
		if (!file.is_open())
		{
			file.close();
			cout << "[Error] " << filename[i] << " couln't be opened." << endl;
			return;
		}

		//TODO: delete \n !

		vector<string> section;
		while (!file.eof())
		{
			string part = "";
			std::getline(file, part, '-');
			//TODO: sort by height
			cout << part;
			section.push_back(part);
		}	
		room_parts_.at(0).push_back(section);
		cout << "--------------" << endl;
		file.close();
	}
	Room* tmp = new Room(Position(0, 0));
	string test[3] = { room_parts_[0][0][0], room_parts_[0][1][0], room_parts_[0][2][0] };
	tmp->generateFromParts(test);
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
	Room* new_room = layout_test_.at(pos.y_).at(pos.x_);
	current_room = new_room;
	current_room->movePlayerToDoor(entry);
}