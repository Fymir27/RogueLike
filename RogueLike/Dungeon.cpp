#include "Dungeon.h"
#include "Room.h"
#include "Minimap.h"
#include "DijkstraMap.h"
#include "Player.h"

#include <fstream>

Dungeon* current_dungeon = NULL;

/*
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
*/

Room*& Dungeon::getRoom(size_t x, size_t y)
{
	return layout_.at(y).at(x);
}

void Dungeon::connect(Room* from, Direction dir, Room* to)
{
	from->addNeighbour(dir, to);
	to->addNeighbour(Direction((dir + 2) % 4), from);
}

/*
Room* Dungeon::generateRoom(size_t x, size_t y, size_t height)
{
	//cout << "Randomly generating Room" << Position(x, y) << endl;
	if (height == 0) height = getRandomRoomHeight();
	Room* room = new Room(Position(x,y));
	room->map_.resize(height);
	for(size_t i = 0; i < 3; i++) //for every section
	{
		size_t r = rand() % room_parts_[height][i].size();      //pick random part
		vector<string> const& part = room_parts_[height][i][r];
		for(size_t y = 0; y < height; y++)
		{
			string const& line = part.at(y);
			for(size_t x = 0; x < part[y].size(); x++)
			{
				char c = line.at(x);
				//cout << c;
				size_t room_x = room->map_.at(y).size();
				switch (c)
				{
				case '#':
					room->map_.at(y).push_back(new Wall(Position(room_x, y)));
					break;

				case '.':
					room->map_.at(y).push_back(new Floor(Position(room_x, y)));
					break;

				case '^':
					room->door_pos_[UP] = Position(room_x,y);
					room->map_.at(y).push_back(new Wall(Position(room_x, y)));
					break;
				
				case '>':
					room->door_pos_[RIGHT] = Position(room_x,y);
					room->map_.at(y).push_back(new Wall(Position(room_x, y)));
					break;

				case 'v':
					room->door_pos_[DOWN] = Position(room_x,y);
					room->map_.at(y).push_back(new Wall(Position(room_x, y)));
					break;

				case '<':
					room->door_pos_[LEFT] = Position(room_x,y);
					room->map_.at(y).push_back(new Wall(Position(room_x, y)));
					break;

				default: //WUT?
					room->map_.at(y).push_back(new Lava(Position(room_x, y)));
					break;
				}
			}
			//cout << endl;
		}
		cout << "Door Positions: "<< endl;
		for (int i = 0; i < 4; i++)
		{
			cout << room->door_pos_[i] << endl;
		}
	}
	room->width_ = room->getColCount();
	room->height_ = room->getRowCount();
	layout_.at(y).at(x) = room;

	size_t prob = 2;
	current_room = room; //Character() needs current_room set
	while(roll(1,prob++)) { room->spawnEnemy(); }

	return room;
}
*/

size_t Dungeon::getRandomRoomHeight()
{
	size_t h;
	do
	{
		h = (rand() % 7) + 4; //max room height 10?
	} while (room_parts_.find(h) == room_parts_.end());
	return h;
}

void Dungeon::generate(size_t width, size_t height)
{
	cout << "Generating Dungeon... (" << width << "x" << height << ")" << endl;

	//-- Setting the size --//
	layout_.resize(height);
	for (auto& row : layout_)
	{
		row.resize(width);
		for (auto& room : row)
		{
			room = NULL;     //no rooms anywhere
		}
	}

	width_ = width;
	height_ = height;
	
	//-- Create two crossing lines and branch out from them randomly --//
	//TODO: add more lines the bigger the room is
	size_t margin_hor  = 0; //from left
	size_t margin_ver =  0; //from top

	while(margin_hor < 2 || margin_hor > width - 2)
		margin_hor =  rand() % width;
	while(margin_ver < 2 || margin_ver > width - 2)
		margin_ver =  rand() % height;

	cout << "Margins (hor/ver): " << margin_hor << "/" << margin_ver << endl;

	Room* tmp = NULL;

	for (size_t y = 0; y < height; y++)     //vertical line
	{
		tmp = new Room(Position(margin_hor, y), getRandomRoomHeight());
		layout_[y][margin_hor] = tmp;
		if (y > 0)
		{
			connect(tmp, UP, getRoom(margin_hor, y - 1));
		}
		int dir_grow = roll(1, 2) ? 1 : -1;       //branch left or right
		size_t stack_count = 1;
		while (roll(1, 2)) //grow branch
		{
			size_t x = margin_hor + stack_count * dir_grow;

			if ((x < 0) || (x >= width))
				break; //too far

			tmp = getRoom(x, y);
			if (tmp == NULL)
			{
				tmp = new Room(Position(x, y), getRandomRoomHeight());
				layout_[y][x] = tmp;
			}
			connect(tmp, (dir_grow == 1 ? LEFT : RIGHT), getRoom(x + (dir_grow * -1), y));
			stack_count++;
		}
	}

	for (size_t x = 0; x < width; x++)
	{
		tmp = getRoom(x, margin_ver);            //horizontal line
		if (tmp == NULL)
		{
			tmp = new Room(Position(x, margin_ver), getRandomRoomHeight());
			layout_[margin_ver][x] = tmp;
		}
		if (x > 0)
		{
			connect(tmp, LEFT, getRoom(x - 1, margin_ver));
		}
		int dir_grow = roll(1, 2) ? 1 : -1;       //branch up or down
		size_t stack_count = 1;
		while (roll(1, 2)) //grow branch
		{
			size_t y = margin_ver + stack_count * dir_grow;

			if ((y < 0) || (y >= height))
				break; //too far

			tmp = getRoom(x, y);
			if (tmp == NULL)
			{
				tmp = new Room(Position(x, y), getRandomRoomHeight());
				layout_[y][x] = tmp;
			}
			connect(tmp, (dir_grow == 1 ? UP : DOWN), getRoom(x, y + (dir_grow * -1)));

			stack_count++;
		}
	}

	//random starting room
	while (current_room == NULL)
		current_room = getRoom(rand() % width, rand() % height);

	current_room->initSpawnLocations();

	//DM.getNextPosition(Position(1,1);

	Minimap::init(width_, height_, current_room->pos_);

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
	for (auto& row : layout_)
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
	DijkstraMap2D DM(current_room->getColCount(), current_room->getRowCount(), Position(1, 1));
}

void Dungeon::readRoomPartsFromFile()
{
	cout << "Reading parts from file" << endl;

	string filename[3];
	filename[0] = "room_parts_left.txt";
	filename[1] = "room_parts_middle.txt";
	filename[2] = "room_parts_right.txt";

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

		vector<string> part;
		vector<vector<string>> section;
		while (!file.eof())
		{
			part.clear();
			while((file.peek() != '-') && !file.eof())
			{
				//cout << "Reading line..." << endl;
				string line;
				std::getline(file, line, '\n');
				part.push_back(line);
			}

			file.get(); //discard '-'
			file.get(); //discard '\n'

			//room_parts_.insert();

			for(auto line : part)
				cout << line << endl;	
			
			room_parts_[part.size()][i].push_back(part);
	
		}	
		file.close();
	}
	//Room* tmp = new Room(Position(0, 0));
	//string test[3] = { room_parts_[0][0][2], room_parts_[0][1][1], room_parts_[0][2][2] };
	//tmp->generateFromParts(test);
}

RoomHeightClass const & Dungeon::getRoomParts(size_t height)
{
	return room_parts_.at(height);
}

//TODO: make simpler (use DELTA)
void Dungeon::changeRoom(Direction dir)
{
	Position pos = current_room->pos_ + DELTA_POS[dir];

	/*
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
	*/

	cout << "New Room is at Pos. " << pos << " in Dungeon!" << endl; 
	current_room = layout_.at(pos.y_).at(pos.x_);
	if(!Minimap::isExplored(pos))
	{
		current_room->initSpawnLocations();
		current_room->spawnEnemies(rand() % 3);
	}
	Minimap::setActiveRoom(pos);	
}