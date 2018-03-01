#include "PlayerClasses.h"
#include "UI.h"
#include "Abilities.h"



map<string, PlayerClass> getPlayerClasses()
{
    static map<string, PlayerClass> classes = {
            { "Warrior", WARR },
            { "Mage", MAGE },
            { "Thief", THIEF }
    };
    return classes;
}

Player* getPlayer(PlayerClass player_class, string const& player_name)
{
    Position spawn = current_room->getFreePosition();
    switch(player_class)
    {
        case WARR: return new Warrior(player_name, spawn);
        case MAGE: return new Mage(player_name, spawn);
        case THIEF: return new Thief(player_name, spawn);
    }
}

//--- Warrior ---//
Stats Warrior::base_stats_(15,15,10,5,5);

Warrior::Warrior(string name, Position pos) : Player(name, pos, base_stats_, "images/player_warrior.png")
{
	class_ = "Warrior";

    ability_bar_.push_back(new WildCharge());
	ability_bar_.push_back(new ShatteringBlow());
	ability_bar_.push_back(new Shockwave());
} 

void Warrior::attack(Character* target)
{
	size_t damage = stats_.str_ + stats_.dex_/2;
	UI::displayText(name_ + " hits " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}

//--- Mage ---//
Stats Mage::base_stats_(5,10,5,15,15);

Mage::Mage(string name, Position pos) : Player(name, pos, base_stats_, "images/player_mage.png")
{
	class_ = "Mage";

	ability_bar_.push_back(new Fireball());
	ability_bar_.push_back(new Regeneration());
	ability_bar_.push_back(new SyphonSoul());
} 

void Mage::attack(Character* target)
{
	size_t damage = stats_.int_ + stats_.wil_/2;
	UI::displayText(name_ + " zaps " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}

//--- Thief ---//
Stats Thief::base_stats_(10,10,15,10,5);

Thief::Thief(string name, Position pos) : Player(name, pos, base_stats_, "images/player_thief.png")
{
	class_ = "Thief";

	ability_bar_.push_back(new PoisonStab());
	ability_bar_.push_back(new ShadowStep());
	ability_bar_.push_back(new DaggerThrow());
} 

void Thief::attack(Character* target)
{
	size_t damage = stats_.dex_ + stats_.str_/2;
	UI::displayText(name_ + " stabs " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}
