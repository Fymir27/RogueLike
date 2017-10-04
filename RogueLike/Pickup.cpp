#include "Pickup.h"
#include "Player.h"
#include "Item.h"
#include "Room.h"
#include "UI.h"

Pickup::Pickup(Position pos, Item* item, unsigned int count) : Field(pos, 0, TRIGGER), item_(item), count_(count)
{
	
}

bool Pickup::stepOn(Character* character)
{
	if (item_)
	{
		UI::displayText("Found " + item_->getName() + " x" + std::to_string(count_) + "!");
		cout << "Found " << item_->getName() << " x" << count_ << "!" << endl;
		character->addItem(item_, count_);
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
