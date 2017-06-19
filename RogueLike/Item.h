#pragma once
#include <string>
#include <assert.h>

using std::string;

class Inventory;
class Room;
class Character;

class Item
{
protected:
	string name_;
	string description_;
	int count_;
	Item* self_;
	Inventory* inventory_;
	bool usable_ = true;

public:
	Item(string name, int count);
	virtual ~Item() {};

	virtual const int getMaxCount() { return 0; };
	int getCount() const { return count_; };
	void increaseCount(const int amount) { count_ += amount; };
	void decreaseCount(const int amount);

	void associateWithInventory(Inventory* inventory) { inventory_ = inventory; };
	string getName() const { return name_; };
	friend std::ostream& operator<<(std::ostream& out, const Item* item);
	bool tryUse(Room* room, Character* character);
	virtual void use(Room* room, Character* character) { assert(0 && "Trying to use base class Item"); };
	virtual Item* duplicate() { return new Item(name_, count_); };

};