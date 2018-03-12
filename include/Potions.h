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

class ManaPotion : public Item
{
	private:
	size_t restore_amount_;
	public:
	ManaPotion(string name, string descr, size_t count, size_t max_count, size_t restore_amount, string texture_file);
	void use(Character* who);
	Item* clone();
	ManaPotion(const ManaPotion& orig);
};

class SmallHealingPotion : public HealingPotion
{
public:
	SmallHealingPotion(int count = 1) : 
	HealingPotion("Small Healing Potion", "Restores 20 HP", count, 10, 20, "images/items/potion_health_small.png") {};
};

class SmallManaPotion : public ManaPotion
{
	public:
	SmallManaPotion(int count = 1) :
			ManaPotion("Small Mana Potion", "Restores 20 Mana", count, 10, 20, "images/items/potion_mana_small.png") {};
};