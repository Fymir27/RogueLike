#include "Character.h"

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

Character::Character(string name, Position pos, Stats stats) : name_(name), pos_(pos),
																															 stats_(stats), 
																															 inventory_(new Inventory())
{
	if (!texture_.loadFromFile("../images/player.png"))
		cout << "Failed to load player texture!" << endl;

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
