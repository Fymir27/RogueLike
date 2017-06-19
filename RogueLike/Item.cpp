#include "Item.h"
#include "Inventory.h"

std::ostream& operator<<(std::ostream& out, const Item* item)
{
	out << "# " << item->name_ << " (" << item->count_ << "x)";
	return out;
};

Item::Item(string name, int count) : name_(name), count_(count)
{
	//cout << "Item " << name << " created (" << count << "x)" << endl;
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