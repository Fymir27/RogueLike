#include <UI.h>
#include "Potions.h"
#include "Character.h"

HealingPotion::HealingPotion(string name, string descr, size_t count, size_t max_count, size_t heal_amount, string texture_file) : 
Item(name, descr, count, max_count, texture_file), heal_amount_(heal_amount)
{

}

void HealingPotion::use(Character* character)
{
	UI::displayText(character->getName() + " just drank a healing potion!");
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

// -------------------------------------------------------------------------------------------------------------------------

ManaPotion::ManaPotion(string name, string descr, size_t count, size_t max_count,
					   size_t restore_amount, string texture_file) :
		Item(name, descr, count, max_count, texture_file), restore_amount_(restore_amount)
{

}

void ManaPotion::use(Character* who)
{
	UI::displayText(who->getName() + " just drank a mana potion!");
	who->restoreMana(restore_amount_);
}

Item* ManaPotion::clone()
{
	return new ManaPotion(*this);
}

ManaPotion::ManaPotion(const ManaPotion& orig) :
		Item(orig.name_, orig.description_, orig.count_, orig.max_count_, string(orig.texture_file_)),
		restore_amount_(orig.restore_amount_)
{

}
