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
    UI::displayText(target->getName() + " gets burned!");
    //--draw test--//
    Position from = current_player->getPosition();
    Position to = target->getPosition();

    Effect::addEffect(new MovingEffect("../images/ab_fireball_animated.png",
                                       sf::Vector2f(from.x_ * TILE_SIZE, from.y_ * TILE_SIZE),
                                       sf::Vector2f(to.x_ * TILE_SIZE, to.y_ * TILE_SIZE), 3));
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

//-----------------------------------------------------------------------------------------------------//

SyphonSoul::SyphonSoul() : Ability("Syphon Soul", "Drains power from the enemy", 0, 0, 5, 70)
{

}

void SyphonSoul::cast(Character* target)
{
    UI::displayText(target->getName() + " had its power drained!");
    Stats minus(-5,-5,-5,-5,-5);
    Stats plus (5,5,5,5,5);
    target->applyEffect(new StatEffect("Power drain", "Stats are lowered", minus, 5));
    current_player->applyEffect(new StatEffect("Power surge", "Stats are increased", plus, 5));
}