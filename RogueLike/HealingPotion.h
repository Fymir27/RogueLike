#pragma once
#include "Common.h"
#include "Item.h"


class HealingPotion : public Item
{
private:
	size_t heal_amount_;
public:
	HealingPotion(string name, string descr, size_t count, size_t max_count, size_t heal_amount, const char* texture_file);
	void use(Character* who);
};

class SmallHealingPotion : public HealingPotion
{
public:
	SmallHealingPotion(int count = 1) : 
	HealingPotion("Small Healing Potion", "Restores 20 HP", count, 10, "../images/item_small_healing_potion.png") {};
};