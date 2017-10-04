#include "HealingPotion.h"
#include "Character.h"

HealingPotion::HealingPotion(string name, string descr, size_t count, size_t max_count, size_t heal_amount, string texture_file) : 
Item(name, descr, count, max_count, texture_file), heal_amount_(heal_amount)
{

}

void HealingPotion::use(Character* character)
{
	cout << endl << character->getName() << " drank a healing Potion - Feeling better already!" << endl;
	character->heal(heal_amount_);
}

Item* HealingPotion::clone()
{
	return new HealingPotion(*this);
}

HealingPotion::HealingPotion(const HealingPotion & orig) :
Item(orig.name_, orig.description_, orig.count_, orig.max_count_, string(orig.texture_file_)), heal_amount_(orig.heal_amount_)
{
	
}


