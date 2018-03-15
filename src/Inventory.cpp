#include "Inventory.h"
#include "Room.h"
#include "Common.h"
#include "Utils.h"
#include "Player.h"
#include "Character.h"
#include "Item.h"

Inventory::Inventory() : limit_(9), rows_(3)
{
	if (!background_.loadFromFile("images/inventory_background.png"))
		cout << "Failed to load Inventory background!" << endl;

	if (!font_.loadFromFile("fonts/8bitOperatorPlus-Regular.ttf"))
		cout << "Failed to load Inventory font!" << endl;

	items_.resize(limit_);
	count_.resize(limit_);
	for (size_t i = 0; i < count_.size(); ++i)
	{
		count_[i] = 0;
	}
}

bool Inventory::addItem(shared_ptr<Items::Item> new_item, size_t count)
{
    cout << "Adding " << count << "x " << new_item->getName() << " to Inventory..." << endl;
 	size_t max_count = new_item->max_stack_size_;

	shared_ptr<Items::Item> cur_item;
	
	for(size_t i = 0; i < items_.size(); i++)
	{
		cur_item = items_[i];
		if(cur_item != nullptr)
		{
			if (new_item->getName() == cur_item->getName())
			{
				if(count_[i] == max_count)
					continue;

				long long overflow = count_[i] + count - max_count;

				if(overflow <= 0)
				{
					count_[i] += count;
					return true;
				}
				else
				{
                    count_[i] = max_count;
					return addItem(new_item, (size_t)overflow);
				}
			}
		}
		else
		{
            long long overflow = count - max_count;
			if(overflow <= 0)
			{
				items_[i] = new_item;
                count_[i] = count;
				return true;
			}
			else
			{
				items_[i] = new_item;
				count_[i] = max_count;
				return addItem(new_item, (size_t)overflow);
			}
		}
	}
	return false;
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
		auto item = items_[i];
		auto sprite = item->getSprite();
		sprite.setScale(sf::Vector2f(1.5f,1.5f));
		float x = pos.x_ + i % rows_ * 63 + 10;
		float y = pos.y_ + i / rows_ * 63 + 10;
		sprite.setPosition(x, y);
		window.draw(sprite);
		sf::Text text;
		text.setFont(font_);
		text.setCharacterSize(20);
		text.setPosition(x, y + 30);
		text.setString(std::to_string(count_[i]));
		window.draw(text);
	}
}

Inventory::~Inventory()
{
	items_.clear();
}



bool Inventory::useItem(size_t slot)
{
	try
	{
		if(items_.at(--slot) == nullptr)
		{
			cout << "There is no Item!" << endl;
			return false;
		}

		items_.at(slot)->use(current_player);
		count_[slot]--;
		if (count_[slot] == 0)
		{
			items_[slot].reset();

		}
		return true;
	}
	catch (std::exception& e)
	{
		cout << "Invalid Item!" << endl;
		return false;
	}
}

bool Inventory::throwItem(size_t slot, Character* target)
{
	try
	{
		if(items_.at(--slot) == nullptr)
		{
			cout << "There is no Item!" << endl;
			return false;
		}

		items_.at(slot)->throwAt(target);
		count_[slot]--;
		if (count_[slot] == 0)
		{
			items_[slot].reset();

		}
		return true;
	}
	catch (std::exception& e)
	{
		cout << "Invalid Item!" << endl;
		return false;
	}
}

