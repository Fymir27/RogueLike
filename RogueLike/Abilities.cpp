#include "Abilities.h"
#include "AbilityEffectTypes.h"
#include "Character.h"
#include "UI.h"
#include "Player.h"
#include "Effects.h"

Ability::Ability(string name, string descr, unsigned damage, unsigned healing, size_t cd, size_t cost) :
GameObject(name, descr), damage_(damage), healing_(healing), cooldown_(cd), cost_(cost) 
{

}

Ability::Ability(string name, string descr) : GameObject(name, descr)
{

}

bool Ability::cast(Character *target)
{
    if(cooldown_left_ > 0)
    {
        UI::displayText("That Ability is still on cooldown!");
        return false;
    }

    if(damage_ > 0)
        target->damage(damage_);
    if(healing_ > 0)
        target->heal(healing_);

    for(auto e : ab_effects_)
    {
        e->apply(target);
    }

    if(effect_ != nullptr)
        Effect::addEffect(effect_);

    return true;
}

void Ability::coolDown()
{
    if(cooldown_left_ > 0)
        cooldown_left_--;
}



//-----------------------------------------------------------------------------------------------------//

Fireball::Fireball() : Ability("Fireball", "Deals additional damage over time.", 50, 0, 0, 50) 
{
    ab_effects_.push_back(new OverTimeEffect("Burn", "Deals fire damage over 5 turns", true, 10, 5));
    AnimatedSprite* anim = new AnimatedSprite("../images/ab_fireball_animated.png", 40);
    effect_ = new MovingEffect(anim, 3);
}

bool Fireball::cast(Character* target)
{
    Position from = current_player->getPosition();
    Position to = target->getPosition();

    dynamic_cast<MovingEffect*>(effect_)->aim(worldToScreen(from), worldToScreen(to));

    return Ability::cast(target);
}

//-----------------------------------------------------------------------------------------------------//

Regeneration::Regeneration() : Ability("Regeneration", "Heals over time", 0, 0, 3, 30)
{
    ab_effects_.push_back(new OverTimeEffect("Regneration", "Heals you for 10 turns", false, 5, 10));
}

//-----------------------------------------------------------------------------------------------------//

SyphonSoul::SyphonSoul() : Ability("Syphon Soul", "Drains power from the enemy", 0, 0, 5, 70)
{
    Stats minus(-5,-5,-5,-5,-5);
    ab_effects_.push_back(new StatEffect("Power drain", "Stats are lowered", minus, 5));
}

bool SyphonSoul::cast(Character* target)
{
    if(!Ability::cast(target))
        return false;

    UI::displayText(target->getName() + " had its power drained!");
    Stats plus (5,5,5,5,5);
    StatEffect* buff = new StatEffect("Power surge", "Stats are increased", plus, 5);
    buff->apply(current_player);
    return true;
}