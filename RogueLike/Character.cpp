#include "Character.h"

std::ostream& operator<<(std::ostream& out, Character* character)
{
	out << endl;
	out << "########~ " << character->name_ << " ~########" << endl << endl;
	out << "+~~~  Stats  ~~~+" << endl;
	out << character->stats_;
	out << "+~~~~~~~~~~~~~~~+" << endl;
	character->printInventory();
	out << string(character->name_.size() + 20, '#') << endl;

	return out;
}

Character::Character(string name, Position pos, Stats stats) : name_(name), pos_(pos),
																															 stats_(stats), 
																															 inventory_(new Inventory())
{
	//std::printf("Character %s spawned at %d|%d\n", name_.c_str(), pos_.x_, pos_.y_);
}

Character::~Character()
{
	cout << name_ << " ded." << endl << endl;
	delete inventory_;
}