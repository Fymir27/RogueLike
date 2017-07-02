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

}

std::ostream& operator<<(std::ostream& out, const Inventory* inv)
{
	out << endl;
	out << "# --- Inventory --- #" << endl;
	for (Item* item : inv->items_)
		out << item << endl;
	out << "# ----------------- #" << endl << endl;
	return out;
}

bool Inventory::addItem(Item* new_item, bool silent)
{
	if (!silent)
	{
		cout << endl;
		cout << "Adding to Inventory:" << endl;
		cout << new_item << endl;
	}

	int old_count;
	int new_count = new_item->getCount();
	string old_name;
	string new_name = new_item->getName();
	for (Item* item : items_)
	{
		old_count = item->getCount();
		old_name = item->getName();
		if (new_name.compare(old_name) == 0)
		{
			//cout << "Found same Item; Trying to stack or fill up" << endl;
			if (old_count + new_count <= item->getMaxCount())
			{
				//cout << "Stacking!" << endl;
				item->increaseCount(new_count);
				delete new_item;
				return true;
			}
			else if (old_count < item->getMaxCount())
			{
				//cout << "Filling up" << endl;
				int space = item->getMaxCount() - old_count;
				item->increaseCount(space);
				new_item->decreaseCount(space);
				//cout << "Storing the Rest" << endl;
				return addItem(new_item, true);
			}
		}
	}
	if (items_.size() < limit_)
	{
		//cout << "Adding Item on new spot" << endl;
		int overflow = new_item->getCount() - new_item->getMaxCount();
		if (overflow > 0)
		{
			Item* overflowItem = new_item->duplicate();
			overflowItem->decreaseCount(new_item->getMaxCount());

			new_item->decreaseCount(overflow);
			new_item->associateWithInventory(this);
			size_t new_count = items_.size();
			new_item->pos_.x_ = new_count % rows_;
			new_item->pos_.y_ = new_count / rows_;
			items_.push_back(new_item);

			addItem(overflowItem, true);
		}
		else
		{
			new_item->associateWithInventory(this);
			size_t new_count = items_.size();
			new_item->pos_.x_ = new_count % rows_;
			new_item->pos_.y_ = new_count / rows_;
			items_.push_back(new_item);
		}

		return true;
	}
	else
	{
		cout << "No Inventory space left!" << endl;
	}
	return false;
}

void Inventory::removeItem(Item* item)
{
	items_.remove(item);
}

sf::Sprite& Inventory::getSprite()
{
	return sprite_;
}

void Inventory::draw(sf::RenderWindow & window)
{
	sprite_.setTexture(background_);
	sprite_.setPosition(current_room->getColCount() * TILE_SIZE, 0);
	window.draw(sprite_);

	unsigned int old_x = sprite_.getPosition().x;
	unsigned int old_y = sprite_.getPosition().y;

	for (auto item : items_)
	{
		sf::Sprite& sprite = item->getSprite();
		sprite.setPosition(old_x + item->pos_.x_ * TILE_SIZE, old_y + item->pos_.y_ * TILE_SIZE);
		window.draw(sprite);
		char string[3];
		sprintf_s(string, 3, "%d", item->count_);
		sf::Text& text = item->text_;
		text.setFont(font_);
		text.setString(string);
		text.setCharacterSize(30);
		text.setFillColor(sf::Color::Black);
		text.setPosition(old_x + item->pos_.x_ * TILE_SIZE + 3, old_y + item->pos_.y_ * TILE_SIZE + 30);
		window.draw(text);
	}
}

Inventory::~Inventory()
{
	cout << "Deleting Inventory" << endl;

	for (Item* item : items_)
	{
		cout << item << endl;
		delete item;
	}
	items_.clear();
}

void Inventory::click(sf::Event event)
{
	Position origin = { sprite_.getPosition().x, sprite_.getPosition().y };
	cout << "origin: " << origin << endl;
	Position pos_clicked = { event.mouseButton.x, event.mouseButton.y };
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
}