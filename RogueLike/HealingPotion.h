#pragma once
#include "Common.h"
#include "Item.h"


class HealingPotion : public Item
{
private:
	size_t heal_amount_;
public:
	HealingPotion(string name, string descr, size_t count, size_t max_count, size_t heal_amount, string texture_file);
	void use(Character* who);
	Item* clone();
	HealingPotion(const HealingPotion& orig);
};

class SmallHealingPotion : public HealingPotion
{
public:
	SmallHealingPotion(int count = 1) : 
	HealingPotion("Small Healing Potion", "Restores 20 HP", count, 10, 20, "../images/item_small_healing_potion.png") {};
};