#include "Types.h"

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

