#include "Abilities.h"
#include "AbilityEffects.h"
#include "Character.h"
#include "UI.h"
#include "Player.h"
#include "Effects.h"

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
    target->applyEffect(new OverTimeEffect("Burn", "Deals fire damage over 5 turns", true, 10, 5));
    //--draw test--//
    Position from = current_player->getPosition();
    Position to = target->getPosition();

    Effect::addEffect(new MovingEffect("../images/ab_fireball.png",
                                       sf::Vector2f(from.x_ * TILE_SIZE, from.y_ * TILE_SIZE),
                                       sf::Vector2f(to.x_ * TILE_SIZE, to.y_ * TILE_SIZE), 2));
}

//-----------------------------------------------------------------------------------------------------//

Regeneration::Regeneration() : Ability("Regeneration", "Heals over time", 0, 0, 3, 30)
{

}

void Regeneration::cast(Character *target)
{
    UI::displayText(target->getName() + " feels refreshed!");
    target->applyEffect(new OverTimeEffect("Regneration", "Heals you for 10 turns", false, 5, 10));
}