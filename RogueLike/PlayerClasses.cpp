#include "PlayerClasses.h"
#include "UI.h"

//--- Warrior ---//
Stats Warrior::base_stats_(15,15,10,5,5);

Warrior::Warrior(string name, Position pos) : Player(name, pos, base_stats_, "../images/player_warrior.png")
{
	class_ = "Warrior";
} 

void Warrior::attack(Character* target)
{
	size_t damage = stats_.str_ + stats_.dex_/2;
	UI::displayText(name_ + " hits " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}

//--- Mage ---//
Stats Mage::base_stats_(5,10,5,15,15);

Mage::Mage(string name, Position pos) : Player(name, pos, base_stats_, "../images/player_mage.png")
{
	class_ = "Mage";
} 

void Mage::attack(Character* target)
{
	size_t damage = stats_.int_ + stats_.will_/2;
	UI::displayText(name_ + " zaps " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}

//--- Thief ---//
Stats Thief::base_stats_(10,10,15,10,5);

Thief::Thief(string name, Position pos) : Player(name, pos, base_stats_, "../images/player_thief.png")
{
	class_ = "Thief";
} 

void Thief::attack(Character* target)
{
	size_t damage = stats_.dex_ + stats_.str_/2;
	UI::displayText(name_ + " stabs " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}
