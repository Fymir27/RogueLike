#pragma once

#include "Field.h"
#include "Types.h"
#include "Item.h"

class Item;
class Pickup : public Field
{
	private:
	Item* item_;
	unsigned int count_;
	public:
	Pickup(Position pos, Item* item, unsigned int count);
	bool stepOn(Character* character);
	void draw(sf::RenderWindow & window);
};