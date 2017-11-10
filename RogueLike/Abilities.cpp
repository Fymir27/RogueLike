#include "Abilities.h"
#include "AbilityEffects.h"
#include "Character.h"
#include "UI.h"

Ability::Ability(string name, string descr, unsigned damage, unsigned healing, size_t cd, size_t cost) : 
GameObject(name, descr), damage_(damage), healing_(healing), cooldown_(cd), cost_(cost) 
{

}

//-----------------------------------------------------------------------------------------------------//

Fireball::Fireball() : Ability("Fireball", "Deals additional damage over time.", 50, 0, 0, 50) 
{

}

void Fireball::cast(Character* target)
{
    UI::displayText(name_ + " deals " + std::to_string(damage_) + " damage.");
    target->damage(damage_);
    target->applyEffect(new OverTimeEffect("Burn", "Deals fire damage over time", true, 10, 5));
}

//-----------------------------------------------------------------------------------------------------//

Regeneration::Regeneration() : Ability("Regeneration", "Heals over time", 0, 0, 3, 30)
{

}

void Regeneration::cast(Character *target)
{
    UI::displayText(target->getName() + " feels refreshed!");
    target->applyEffect(new OverTimeEffect("Regneration", "Heals over time", false, 5, 10));
}