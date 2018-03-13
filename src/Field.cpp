#include "Field.h"
#include "Room.h"
#include "Character.h"
#include "UI.h"
#include "ItemOLD.h"

Field::Field(Position pos, int tile_nr, FIELD_STATUS status) : tile_nr_(tile_nr), pos_(pos), status_(status)
{
	
}

Field::~Field()
{

}

void Field::free()
{
	status_ = FREE;
	character_ = NULL;
}

void Field::occupy(Character* character)
{
	character_ = character;
	status_ = OCCUPIED;
}

void Field::placeItem(ItemOLD* item)
{
	item_ = item;
	status_ = PICKUP;
}

void Field::pickUpItem(Character* character)
{
	UI::displayText("Found " + item_->getName() + " x" + std::to_string(item_->getCount()) + "!");
	character->addItem(item_);
	item_ = NULL;
	count_ = 0; 
}

void Field::draw(sf::RenderWindow & window)
{
	if (status_ == PICKUP)
	{
		sf::Sprite& sprite = item_->getSprite();
		sprite.setPosition(pos_.x_ * TILE_SIZE, pos_.y_ * TILE_SIZE);
		window.draw(sprite);
	}
}

/*
bool Field::stepOn(Character* who)
{
	if(who == character_)
		return false;

	if(status_ == OCCUPIED)
	{
		UI::displayText(who->getName() + " hits!");
		cout << who->getName() << " hits!" << endl;
		character_->damage(10); //TODO: get attacker's damage
		return false;
	}
	else
	{
		//step on
		occupied_ = true;
		character_ = who;
		return true;
	}
}

bool Field::stepOff()
{
	occupied_ = false;
	character_ = NULL;
	return true;
}
*/