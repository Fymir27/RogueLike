#include "Pickup.h"
#include "Player.h"
#include "Item.h"
#include "Room.h"

Pickup::Pickup(Position pos, Item * item) : Field(pos, 0), item_(item)
{
	
}

bool Pickup::stepOn(Character* character)
{
	if (item_)
	{
		character->addItem(item_);
		item_ = NULL;
	}
	return true;
}

void Pickup::draw(sf::RenderWindow& window)
{
	if (!item_)
		return;
	sf::Sprite sprite = item_->getSprite();
	sprite.setPosition(pos_.x_ * TILE_SIZE, pos_.y_ * TILE_SIZE);
	window.draw(sprite);
}
