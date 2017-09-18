#include "Inventory.h"
#include "Item.h"
#include "Room.h"
#include "Common.h"
#include "Types.h"
#include "Player.h"

Inventory::Inventory() : limit_(9), rows_(3)
{
	if (!background_.loadFromFile("../images/inventory_background.png"))
		cout << "Failed to load Inventory background!" << endl;

	if (!font_.loadFromFile("../fonts/Arcade.ttf"))
		cout << "Failed to load Inventory font!" << endl;

	items_.resize(limit_);
	count_.resize(limit_);
}

bool Inventory::addItem(Item* new_item, unsigned int new_count)
{
	size_t max_count = new_item->getMaxCount();		

	for(size_t i = 0; i < items_.size(); i++)
	{
		if(items_[i] != NULL)
		{
			if (new_item->name_.compare(items_[i]->name_) == 0)
			{

				if(count_[i] == max_count)
					continue;

				if((count_[i] + new_count) <= max_count)
				{
					count_[i] += new_count;
					return true;
				}
				else
				{
					count_[i] = max_count;
					return addItem(new_item, new_count - max_count);
				}
			}
		}
		else
		{

			if(new_count <= max_count)
			{
				items_[i] = new_item;
				count_[i] = new_count;
				return true;
			}
			else
			{
				items_[i] = new_item;
				count_[i] = max_count;
				return addItem(new_item, new_count - max_count);
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
			break;
		Item* item = items_[i];
		sf::Sprite& sprite = item->getSprite();
		sprite.setScale(sf::Vector2f(1.5f,1.5f));
		sprite.setPosition(pos.x_ + i % rows_ * 64 + 10, pos.y_ + i / rows_ * 64 + 10);
		window.draw(sprite);

		//char string[3];
		//sprintf_s(string, 3, "%d", item->count_);
		sf::Text& text = item->text_;
		text.setFont(font_);
		text.setString(std::to_string(count_[i]));
		text.setCharacterSize(30);
		//text.setFillColor(sf::Color::Black);
		text.setPosition(pos.x_ + i % rows_ * 64 + 3, pos.y_ + i / rows_ * 64 + 30);
		window.draw(text);
	}
}

Inventory::~Inventory()
{
	items_.clear();
}


void Inventory::click(Position pos_clicked)
{
	/*
	Position origin = { sprite_.getPosition().x, sprite_.getPosition().y };
	cout << "origin: " << origin << endl;
	cout << "pos_clicked: " << pos_clicked << endl;
	unsigned int x = (pos_clicked.x_ - origin.x_) / TILE_SIZE;
	unsigned int y = (pos_clicked.y_ - origin.y_) / TILE_SIZE;
	cout << x << "|"<< y << endl;
	for (auto item : items_)
	{
		if (item->pos_.x_ == x && item->pos_.y_ == y)
		{
			item->tryUse(current_room, current_player);
			break;
		}
	}
	*/
}


void Inventory::useItem(int slot)
{
	try
	{
		if(items_.at(--slot) == NULL)
		{
			cout << "There is no Item!" << endl;
			return;
		}

		items_.at(slot)->tryUse(current_room, current_player);
		count_.at(slot)--;
		if(count_.at(slot) == 0)
			items_.at(slot) = NULL;
	}
		catch (std::exception e)
	{
		cout << "Invalid Field!" << endl;
	}
}

