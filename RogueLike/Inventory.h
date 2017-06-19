#pragma once
#include <iostream>
#include <list>

using std::list;
using std::cout;
using std::endl;

class Item;
class Inventory
{
private:
	list<Item*> items_;
	size_t limit_;

public:
	Inventory() : limit_(3) {/*cout << "New Inventory Instantiated" << endl; */ };
	~Inventory();

	void sort() {};
	bool addItem(Item* item, bool silent = false); //returns false when full
	friend std::ostream& operator<<(std::ostream& out, const Inventory* inv);
	void removeItem(Item* item);
};
