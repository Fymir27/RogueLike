#include "Types.h"

std::ostream& operator<<(std::ostream& out, const Position& pos)
{
	out << "(" << pos.x_ << "|" << pos.y_ << ")";
	return out;
}

std::ostream& operator<<(std::ostream& out, Stats stats)
{
	out << "Hitpoints:   " << stats.hp_ << endl;
	out << "Mana:        " << stats.mana_ << endl;
	out << "Strength:    " << stats.str_ << endl;
	out << "Intelligence:" << stats.int_ << endl;
	out << "Dexterity:   " << stats.dex_ << endl;
	return out;
}

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