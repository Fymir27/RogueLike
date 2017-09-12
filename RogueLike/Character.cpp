#include "Character.h"
#include "Field.h"
#include "Room.h"

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
	delete inventory_;
}

void Character::draw(sf::RenderWindow & window)
{
	sprite_.setPosition(sf::Vector2f(pos_.x_ * TILE_SIZE, pos_.y_ * TILE_SIZE));
	window.draw(sprite_);
}

bool Character::move(Position new_pos)
{
	Field* field = current_room->getField(pos_.x_, pos_.y_);

	if(!field->stepOff())
		return false;

	field = current_room->getField(new_pos.x_, new_pos.y_);

	if(field == NULL)
		return false;

	if(!field->stepOn(this))
		return false;

	pos_ = new_pos;
	return true;
}
