#include "Character.h"
#include "Field.h"
#include "Room.h"
#include "Item.h"

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
}

Character::~Character()
{
	cout << name_ << " ded." << endl << endl;
	current_room->stepOff(pos_, this, UP);
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

bool Character::move(Position new_pos)
{
	Room* room = current_room;
	Position old_pos = pos_;
	
	if(!room->stepOn(new_pos, this, UP))
		return false;

	if(!room->stepOff(old_pos, this, UP))
		return false;

	if(room == current_room)
		pos_ = new_pos;

	return true;
}

bool Character::isSolid(Position pos)
{
	return current_room->getField(pos.x_, pos.y_)->isSolid();
}
