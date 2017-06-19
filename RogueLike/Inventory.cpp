#include "Inventory.h"
#include "Item.h"

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
			items_.push_back(new_item);

			addItem(overflowItem, true);
		}
		else
		{
			new_item->associateWithInventory(this);
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
