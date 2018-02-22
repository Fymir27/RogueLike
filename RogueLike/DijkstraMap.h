#pragma once

#include "Common.h"
#include "Utils.h"

class DijkstraMap2D
{
	private:
	vector < vector<size_t> > map_;
	Position source_;
	size_t width_;
	size_t height_;

	void updateNeighbours(Position pos, size_t cur);

	public:
	DijkstraMap2D(size_t width, size_t height, Position source);
	Position getNextPosition(Position cur_pos);
	void updateSource(Position pos);
};