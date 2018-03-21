#include "Utils.h"
#include "Room.h"
#include "Field.h"

std::default_random_engine random_engine;

Position::Position(int x, int y) : x_(x), y_(y)
{
	
}

std::ostream& operator<<(std::ostream& out, const Position& pos)
{
	out << "(" << pos.x_ << "|" << pos.y_ << ")";
	return out;
}

Position operator+(Position first, const Position& second)
{
	first.x_ += second.x_;
	first.y_ += second.y_;
	return first;
}

Position operator-(Position first, const Position& second)
{
	first.x_ -= second.x_;
	first.y_ -= second.y_;
	return first;
}

bool operator==(const Position& first, const Position& second)
{
	return (first.x_ == second.x_ && first.y_ == second.y_);
}

bool operator!=(const Position& first, const Position& second)
{
	return (first.x_ != second.x_ || first.y_ != second.y_);
}

vector<Position> bresenham(Position start, Position end, bool ignore_walls)
{
	vector<Position> pixels;

	int x_start = start.x_, x_end = end.x_;
	int y_start = start.y_, y_end = end.y_;

	int dx = x_end - x_start;
	int dy = y_end - y_start;

	int step_x = sign(dx);
	int step_y = sign(dy);

	dx = absolute(dx);
	dy = absolute(dy);

	Position step_fast, step_slow;
	int      d_fast,    d_slow;

	if(dx > dy)
	{
		step_fast = { step_x, 0};
		step_slow = { 0, step_y };

		d_fast = dx;
		d_slow = dy;
	}
	else
	{
		step_fast = { 0, step_y };
		step_slow = { step_x, 0};

		d_fast = dy;
		d_slow = dx;
	}

	int err = d_fast / 2;

	pixels.push_back(start);
    Field* field = nullptr;

	while(start != end)
	{
		err -= d_slow;
		if(err < 0)
		{
			err += d_fast;
			start = start + step_slow;
		}
		start = start + step_fast;
        pixels.push_back(start);

        field = current_room->getField(start);
        if(field == nullptr)
            break;
        if(!ignore_walls && field->getFieldStatus() == SOLID)
            break;
	}
	return pixels;
}
