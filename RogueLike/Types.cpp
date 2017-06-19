#include "Types.h"
#include <iostream>

using std::cout;
using std::endl;

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