#include "Inventory.h"
#include "Item.h"
#include "Room.h"
#include "Common.h"
#include "Utils.h"
#include "Player.h"

Inventory::Inventory() : limit_(9), rows_(3)
{
	if (!background_.loadFromFile("images/inventory_background.png"))
		cout << "Failed to load Inventory background!" << endl;

	if (!font_.loadFromFile("fonts/8bitOperatorPlus-Regular.ttf"))
		cout << "Failed to load Inventory font!" << endl;

	items_.resize(limit_);
	count_.resize(limit_);
}

bool Inventory::addItem(Item* new_item)
{
	size_t max_count = new_item->max_count_;		
	size_t new_count = new_item->count_;
	Item* cur_item = NULL;
	
	for(size_t i = 0; i < items_.size(); i++)
	{
		cur_item = items_[i];
		if(cur_item != NULL)
		{
			if (new_item->name_.compare(cur_item->name_) == 0)
			{
				if(cur_item->count_ == max_count)
					continue;

				int overflow = cur_item->count_ + new_count - max_count;

				if(overflow <= 0)
				{
					cur_item->count_ += new_count;
					delete new_item;
					return true;
				}
				else
				{
					cur_item->count_ = max_count;
					new_item->count_ = overflow;
					return addItem(new_item);
				}
			}
		}
		else
		{
			int overflow = new_count - max_count;
			if(overflow <= 0)
			{
				items_[i] = new_item;
				return true;
			}
			else
			{
				items_[i] = new_item;
				items_[i]->count_ = max_count;
				Item* tmp = new_item->clone();
				tmp->count_ = overflow;
				return addItem(tmp);
			}
		}
	}
	return false;
}

void Inventory::removeItem(Item* item)
{
	//items_.remove(item);
}

sf::Sprite& Inventory::getSprite()
{
	return sprite_;
}

void Inventory::draw(sf::RenderWindow& window, Position pos)
{
	sprite_.setTexture(background_);
	sprite_.setPosition(pos.x_, pos.y_);
	window.draw(sprite_);

	for(size_t i = 0; i < items_.size(); i++)
	{
		if(!items_[i])
			continue;
		Item* item = items_[i];
		sf::Sprite& sprite = item->getSprite();
		sprite.setScale(sf::Vector2f(1.5f,1.5f));
		float x = pos.x_ + i % rows_ * 63 + 10;
		float y = pos.y_ + i / rows_ * 63 + 10;
		sprite.setPosition(x, y);
		window.draw(sprite);
		sf::Text text;
		text.setFont(font_);
		text.setCharacterSize(20);
		text.setPosition(x, y + 30);
		text.setString(std::to_string(item->getCount()));
		window.draw(text);
	}
}

Inventory::~Inventory()
{
	items_.clear();
}



bool Inventory::useItem(int slot)
{
	try
	{
		if(items_.at(--slot) == NULL)
		{
			cout << "There is no Item!" << endl;
			return false;
		}

		items_.at(slot)->use(current_player);
		size_t new_count = --items_.at(slot)->count_;
		if (new_count == 0)
		{
			delete items_.at(slot);
			items_.at(slot) = NULL;
		}
		return true;
	}
	catch (std::exception e)
	{
		cout << "Invalid Item!" << endl;
		return false;
	}
}

