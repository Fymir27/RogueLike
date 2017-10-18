#include "DijkstraMap.h"
#include "Room.h"
#include "Field.h"


void DijkstraMap2D::updateNeighbours(Position pos, size_t cur)
{
	for (size_t i = 0; i < 4; i++)
	{
		int new_x = pos.x_ + DELTA_X[i];
		int new_y = pos.y_ + DELTA_Y[i];

		if (current_room->getFieldStatus(Position(new_x, new_y)) == SOLID)
		{
			continue;
		}

		try
		{
			size_t& tmp = map_.at(new_y).at(new_x);
			if (tmp > (cur + 1))
			{
				tmp = cur + 1;
				updateNeighbours(Position(new_x, new_y), tmp);
			}
		}
		catch (std::out_of_range& e)
		{
			continue;
		}
	}
}

DijkstraMap2D::DijkstraMap2D(size_t width, size_t height, Position source) : width_(width), height_(height), source_(source)
{
	map_.resize(height_);
	for (auto& row : map_)
	{
		row.resize(width_);
		for (size_t& value : row)
		{
			value = std::numeric_limits<size_t>::max();
		}
	}

	map_.at(source_.y_).at(source_.x_) = 0;
	updateNeighbours(source_, 0);

	/*
	cout << endl;
	for (auto& row : map_)
	{
	for (auto val : row)
	{
	if (val > 99)
	cout << " # ";
	else
	printf_s("%3zd", val);
	}
	cout << endl;
	}
	*/
}

Position DijkstraMap2D::getNextPosition(Position cur_pos)
{
	Position result;
	int new_x;
	int new_y;
	size_t min = std::numeric_limits<size_t>::max();
	for (size_t i = 0; i < 4; i++)
	{
		new_x = cur_pos.x_ + DELTA_X[i];
		new_y = cur_pos.y_ + DELTA_Y[i];
		size_t tmp = map_.at(new_y).at(new_x);
		if (tmp <= min)
		{
			min = tmp;
			result = Position(new_x, new_y);
		}
	}
	return result;
}

void DijkstraMap2D::updateSource(Position pos)
{
	source_ = pos;

	for (auto& row : map_)
		for (size_t& value : row)
			value = std::numeric_limits<size_t>::max();
	map_.at(source_.y_).at(source_.x_) = 0;
	updateNeighbours(source_, 0);
}
