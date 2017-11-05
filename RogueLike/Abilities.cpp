#include "Abilities.h"
#include "AbilityEffects.h"
#include "Character.h"

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
    target->damage(damage_);
    new OverTimeEffect("Burn", "Deals fire damage over time", true, 10, 5);
}

//-----------------------------------------------------------------------------------------------------//