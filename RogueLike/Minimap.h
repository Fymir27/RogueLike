#pragma once

#include "Common.h"
#include "Utils.h"

enum RoomStatus
{
	ACTIVE,
	EXPLORED,
	UNEXPLORED,
	NONE
};

class Minimap //static class
{
	private:
	static vector<vector<RoomStatus>> map_;
	static Position active_room_;

	public:
	static void init(size_t width, size_t height, Position pos_active);
	static void setActiveRoom(size_t x, size_t y);
	static void setActiveRoom(Position pos);

	static bool isExplored(Position pos);

	static void print();

	static void draw(sf::RenderWindow & window);

};