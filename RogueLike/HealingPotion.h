#pragma once
#include <string>
#include "Item.h"

using std::string;

class HealingPotion : public Item
{
private:
	int heal_amount_;
public:
	HealingPotion(string name, int count, int heal_amount, const char* texture_file);
	const int getMaxCount() { return 10; };
	void use(Room* room, Character* player);
	Item* duplicate();
};

class SmallHealingPotion : public HealingPotion
{
public:
	SmallHealingPotion(int count = 1) : HealingPotion("Small Healing Potion", count, 20, "TEMP") {};
};

class MediumHealingPotion : public HealingPotion
{
public:
	MediumHealingPotion(int count = 1) : HealingPotion("Medium Healing Potion", count, 100, "../images/item_medium_healing_potion.png") {};
};

class LargeHealingPotion : public HealingPotion
{
public:
	LargeHealingPotion(int count = 1) : HealingPotion("Large Healing Potion", count, 500, "TEMP") {};
};