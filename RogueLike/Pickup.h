#pragma once

#include "Field.h"
#include "Types.h"

class Item;
class Pickup : public Field
{
	private:
	Item* item_;
	public:
	Pickup(Position pos, Item* item);
	bool stepOn(Character* character);
	void draw(sf::RenderWindow & window);
};