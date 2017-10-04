#include "HealingPotion.h"
#include "Character.h"

HealingPotion::HealingPotion(string name, string descr, size_t count, size_t max_count, size_t heal_amount, const char* texture_file) : 
Item(name, descr, count, max_count, texture_file), heal_amount_(heal_amount)
{
	if (!texture_.loadFromFile(texture_file))
		cout << "Couldn't load texture for " << name << endl;
	sprite_.setTexture(texture_);
}

void HealingPotion::use(Room* room, Character* character)
{
	cout << endl << character->getName() << " drank a healing Potion - Feeling better already!" << endl;
	character->heal(heal_amount_);
}


