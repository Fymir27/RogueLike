#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

using std::string;

class Character
{
protected:
	string name_;
	Position pos_;
	Stats stats_;
	Inventory* inventory_;

	Character(const string name, const Position pos, const Stats stats);

public:
	~Character();
	string getName() const { return name_; };
	Position getPosition() const { return pos_; };
	Stats getStats() const { return stats_; };
	void heal(const int amount) { stats_.hp_ += amount; };
	void damage(const int amount) { stats_.hp_ -= amount; };
	bool addItem(Item* item) { return inventory_->addItem(item); };
	void printInventory() const { cout << inventory_; };
	friend std::ostream& operator<<(std::ostream& out, Character* character);
};
