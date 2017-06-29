#include "Inventory.h"
#include "Item.h"
#include "Room.h"

Inventory::Inventory() : limit_(9), rows_(3)
{
	if (!background_.loadFromFile("../images/inventory_background.png"))
		cout << "Failed to load Inventory background!" << endl;

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
