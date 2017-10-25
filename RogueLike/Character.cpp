#include "Character.h"
#include "Field.h"
#include "Room.h"
#include "Item.h"
#include "UI.h"


std::ostream& operator<<(std::ostream& out, Stats stats)
{
	out << "Strength:    " << stats.str_ << endl;
	out << "Intelligence:" << stats.int_ << endl;
	out << "Dexterity:   " << stats.dex_ << endl;
	return out;
}


Stats::Stats(int str, int end, int dex, int intel, int will) :
	 str_(str), end_(end), dex_(dex), int_(intel), will_(will)
{

}

bool Character::addItem(Item* item)
{ 
	return inventory_->addItem(item); 
}

Character::Character(string name, Position pos, Stats stats, string filename) : name_(name), pos_(pos),
																				 stats_(stats), 
																				 inventory_(new Inventory())
{
	//cout << "-~=# " << name_ << " #=~-" << endl;
	if (!texture_.loadFromFile(filename))
		cout << "Failed to load character texture!" << endl;

	hp_ = Ressource<int>(stats_.end_ * 10);
	mana_ = Ressource<int>(stats_.int_ * 10);

	sprite_.setTexture(texture_);

	current_room->occupyField(pos_, this);
}

Character::~Character()
{
	UI::displayText(name_ + " defeated.");
	current_room->freeField(pos_);
	delete inventory_;
}

void Character::draw(sf::RenderWindow & window)
{
	sprite_.setPosition(sf::Vector2f(pos_.x_ * TILE_SIZE, pos_.y_ * TILE_SIZE));
	window.draw(sprite_);
}

void Character::heal(const int amount)
{
	hp_ += amount;
	cout << name_ << " healed for " << amount << endl;
}

void Character::damage(const int amount)
{
	hp_ -= amount;
	cout << name_ << " damaged for " << amount << endl;
}

void Character::attack(Character* target)
{
	size_t damage = stats_.str_ + stats_.dex_/2 + stats_.int_/3;
	UI::displayText(name_ + " attacks " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}

bool Character::move(Position new_pos)
{
	//cout << name_ << " moving from " << pos_ << " to " << new_pos << endl;
	bool valid = current_room->stepOn(new_pos, this, pos_);
	//cout << "Result: " << pos_ << endl;
	return valid;
}

