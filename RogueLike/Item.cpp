#include "Item.h"
#include "Inventory.h"

std::ostream& operator<<(std::ostream& out, const Item* item)
{
	out << "# " << item->name_ << " (" << item->count_ << "x)";
	return out;
};

Item::Item(string name, int count, const char* texture_file) : name_(name), count_(count), texture_file_(texture_file)
{
	if (!texture_.loadFromFile(texture_file))
		cout << "Couldn't load texture for " << name << endl;
	sprite_.setTexture(texture_);
}
void Item::decreaseCount(const int amount)
{
	count_ -= amount;
	if (count_ == 0)
	{
		inventory_->removeItem(this);
	}
}

bool Item::tryUse(Room* room, Character* character)
{
	if (usable_)
	{
		use(room, character);
		decreaseCount(1);
		cout << "Item has " << count_ << " charges left" << endl;
	}
	return usable_;
}

sf::Sprite& Item::getSprite()
{
	return sprite_;
}