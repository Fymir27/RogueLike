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


std::ostream& operator<<(std::ostream& out, Stats stats)
{
	out << "Hitpoints:   " << stats.hp_[CUR] << "|" << stats.hp_[MAX] << endl;
	out << "Hitpoints:   " << stats.mana_[CUR] << "|" << stats.mana_[MAX] << endl;
	out << "Strength:    " << stats.str_ << endl;
	out << "Intelligence:" << stats.int_ << endl;
	out << "Dexterity:   " << stats.dex_ << endl;
	return out;
}

/*
char* getStatString(Stats stats)
{
	static char buffer[1024];
	sprintf_s(buffer, 1024, 
		"Hitpoints %d\n"
		"Mana     %d\n"
		"Str      %d\n"
		"Int      %d\n"
		"Dex      %d\n",
		stats.hp_, stats.mana_, stats.str_, stats.int_, stats.dex_);
	return buffer;
}
*/
