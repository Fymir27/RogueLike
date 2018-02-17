#include "EnemyClasses.h"
#include "UI.h"

//--- Ghost ----//

Ghost::Ghost(Position pos) : Enemy("Ghost", pos, Stats(0, 10, 0, 10, 5), FOLLOW, "../images/ghost.png")
{
	exp_reward_ = 5;
}

void Ghost::attack(Character* target)
{
    int damage = stats_.int_ + stats_.will_/2;
	UI::displayText(name_ + " spooks " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}


//--- Spider ----//

Spider::Spider(Position pos) : Enemy("Spider", pos, Stats(5, 2, 10, 0, 5), RANDOM, "../images/spider.png") 
{
	exp_reward_ = 2;
}

void Spider::attack(Character* target)
{
    int damage = stats_.dex_ + stats_.str_/2;
	UI::displayText(name_ + " bites " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}


//--- Golem ----//

Golem::Golem(Position pos) : Enemy("Golem", pos, Stats(15, 10, 5, 5, 10), WAIT, "../images/golem.png") 
{
	exp_reward_ = 10;
}

void Golem::attack(Character* target)
{
    int damage = (int)(stats_.str_ * 1.5);
	UI::displayText(name_ + " smashes " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}