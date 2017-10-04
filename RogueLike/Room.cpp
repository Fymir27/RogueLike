#include "Room.h"
#include "Field.h"
#include "Common.h"
#include "Types.h"
#include "Player.h"
#include "Enemy.h"

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

Position Room::stepOn(Position new_pos, Character* who)
{
	Position old_pos = who->getPosition();
	Field* new_field = getField(new_pos.x_, new_pos.y_);
	Field* old_field = getField(old_pos.x_, old_pos.y_);

	if(new_field == NULL)
		return old_pos;

	switch(new_field->status_)
	{
		case SOLID:
		return old_pos;

		case FREE:
		new_field->occupy(who);
		old_field->free();
		return new_pos;

		case OCCUPIED:
		who->attack(new_field->getCharacter());
		return old_pos;

		case TRIGGER:
		return new_field->trigger(who);

		case PICKUP:
		new_field->occupy(who);
		new_field->pickUpItem(who);
		old_field->free();
		return new_pos;
	}
	return old_pos;
}

vector<Position> Room::getShortestPath(Position from, Position to)
{
	vector<Position> tmp;
	tmp.push_back({0,0});
	return tmp;
}

void Room::freeField(Position pos)
{
	getField(pos.x_, pos.y_)->free();
}

void Room::occupyField(Position pos, Character* who)
{
	getField(pos.x_, pos.y_)->occupy(who);
}

void Room::placeItem(Position pos, Item* item, size_t count)
{
	getField(pos.x_, pos.y_)->placeItem(item, count);
}

Room::Room()
{
	
}

Room::Room(const char * filename, Position pos) : pos_(pos)
{
	if(!readRoomFromFile(filename))
	{
		cout << "[Error] Room could not be loaded!" << endl;
		return;
	}

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
	bool reading = true;
	Position pos = { 0,0 };
	while (reading)
	{
		c = file.get();
		Field* field = NULL;

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

		case '^':
			field = new Door(pos, UP);
			entries_[UP] = (pos + Position(0,1));
			break;

		case '>':
			field = new Door(pos, RIGHT);
			entries_[RIGHT] = (pos + Position(-1,0));
			break;

		case 'v':
			field = new Door(pos, DOWN);
			entries_[DOWN] = (pos + Position(0,-1));
			break;

		case '<':
			field = new Door(pos, LEFT);
			entries_[LEFT] = (pos + Position(1,0));
			break;

		case '\n':
			map_.push_back(row);
			row.clear();
			field = NULL;
			pos.x_ = 0;
			pos.y_++;
			break;

		default:
			cout << "[Error] Unknown Symbol!" << endl;
			break;
		}

		if (field)
		{
			pos.x_++;
			row.push_back(field);
		}
		cout << c;
		//cout << pos << endl;
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
	for(auto enemy : enemies_)
	{
		enemy->draw(window);
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

Position Room::getEntryPosition(Direction entry)
{
	return entries_[entry];
}

void Room::addEntryPosition(Direction dir, Position pos)
{
	entries_[dir] = pos;
}

void Room::movePlayerToDoor(Direction entry)
{
	//current_player->pos_ = doors_[entry];
}

void Room::addEnemy(Enemy* enemy)
{
	enemies_.push_back(enemy);
}

void Room::removeEnemy(Enemy* enemy)
{
	enemies_.remove(enemy);
	//dead_enemies_.push_back(enemy);
}

void Room::stepEnemies() //called appr. 63 times a second
{
	vector<Enemy*> dead_enemies;
	for(Enemy* enemy : enemies_)
	{
		if(!enemy->step())
		{
			dead_enemies.push_back(enemy);
		}
	}
	for(auto enemy : dead_enemies)
	{
		enemies_.remove(enemy);
		delete enemy;
	}
}