#include "Abilities.h"
#include "AbilityEffectTypes.h"
#include "Character.h"
#include "UI.h"
#include "Player.h"
#include "Effects.h"
#include "AbilityEffects.h"

Ability::Ability(const string& name, const string& descr, unsigned damage, unsigned healing, size_t cd, size_t cost) :
GameObject(name, descr), damage_(damage), healing_(healing), cooldown_(cd), cost_(cost) 
{

}

bool Ability::cast(Character *target)
{
    cout << "Ability cast" << endl;
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
        e->createInstance(target);
        cout << e->getName() << " created successfully!" << endl;
    }

    if(effect_ != nullptr)
    {
        cout << "Adding Visual Effect" << endl;
        Effect::addEffect(effect_);
    }

    cooldown_left_ = cooldown_ + 1; //+1 bec. will cool down once right after cast!
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
    ab_effects_.push_back(new BurnEffect(10, 5));
    AnimatedSprite* anim = new AnimatedSprite("../images/ab_fireball_animated.png", 40);
    effect_ = shared_ptr<Effect>(new MovingSprite(anim, 3));
}

bool Fireball::cast(Character* target)
{
    Position from = current_player->getPosition();
    Position to = target->getPosition();

    dynamic_cast<MovingSprite*>(effect_.get())->aim(worldToScreen(from), worldToScreen(to));

    return Ability::cast(target);
}

//-----------------------------------------------------------------------------------------------------//

Regeneration::Regeneration() : Ability("Regeneration", "Heals over time", 0, 0, 3, 30)
{
    ab_effects_.push_back(new RegenerationEffect(10, 5));
}

//-----------------------------------------------------------------------------------------------------//

SyphonSoul::SyphonSoul() : Ability("Syphon Soul", "Drains power from the enemy", 0, 0, 5, 70)
{
    ab_effects_.push_back(new AllStatsDown(5,5));
    buff_ = new AllStatsUp(5,5);
}

bool SyphonSoul::cast(Character* target)
{
    if(!Ability::cast(target))
        return false;

    UI::displayText(target->getName() + " had its power drained!");
    buff_->createInstance(current_player);
    return true;
}