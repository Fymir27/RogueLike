#include "Character.h"
#include "Field.h"
#include "Room.h"
#include "Item.h"
#include "UI.h"

std::ostream& operator<<(std::ostream& out, Character* character)
{
	out << endl;
	out << "########~ " << character->name_ << " ~########" << endl << endl;
	out << "+~~~  Stats  ~~~+" << endl;
	out << character->stats_;
	out << "+~~~~~~~~~~~~~~~+" << endl;
	out << string(character->name_.size() + 20, '#') << endl;

	return out;
}

bool Character::addItem(Item* item, unsigned int count)
{ 
	return inventory_->addItem(item, count); 
}

Character::Character(string name, Position pos, Stats stats, string filename) : name_(name), pos_(pos),
																				 stats_(stats), 
																				 inventory_(new Inventory())
{
	if (!texture_.loadFromFile(filename))
		cout << "Failed to load character texture!" << endl;

	sprite_.setTexture(texture_);

	cout << this;

	current_room->occupyField(pos_, this);
}

Character::~Character()
{
	cout << name_ << " ded." << endl << endl;
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
	int new_value = stats_.hp_[CUR] + amount;
	if(new_value > stats_.hp_[MAX])
		new_value = stats_.hp_[MAX];
	stats_.hp_[CUR] = new_value;
}

void Character::damage(const int amount)
{
	//cout << name_ << " got hit for " << amount << " damage." << endl;
	int new_value = stats_.hp_[CUR] - amount;
	if(new_value < 0)
		new_value = 0;
	stats_.hp_[CUR] = new_value;
}

void Character::attack(Character* target)
{
	UI::displayText(name_ + " attacks " + target->getName() + ".");
}

bool Character::move(Position new_pos)
{
	//Position old_pos = pos_;
	
	pos_ = current_room->stepOn(new_pos, this);

	if(pos_ == new_pos)
		return true;

	return false;
}

