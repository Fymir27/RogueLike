#include "HealingPotion.h"
#include "Character.h"

HealingPotion::HealingPotion(string name, int count, int heal_amount) : Item(name, count), heal_amount_(heal_amount)
{

}

void HealingPotion::use(Room* room, Character* character)
{
	cout << endl << character->getName() << " drank a healing Potion - Feeling better already!" << endl;
	character->heal(heal_amount_);
}

Item* HealingPotion::duplicate()
{
	return new HealingPotion(name_, count_, heal_amount_);
}