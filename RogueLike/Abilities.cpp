#include "Abilities.h"
#include "AbilityEffectTypes.h"
#include "Character.h"
#include "UI.h"
#include "Player.h"
#include "Effects.h"
#include "AbilityEffects.h"

Ability::Ability(string const& name, string const& descr, unsigned damage, unsigned healing, size_t cd, size_t cost) :
GameObject(name, descr), damage_(damage), healing_(healing), cooldown_(cd), cost_(cost) 
{

}

bool Ability::cast(Character* caster, Direction dir, bool self)
{
    caster_ = caster;

    //cout << name_ << " in dir. " << dir << " - self: " << self << endl;

    if(self)
        return cast(caster);

    auto path = getAbilityPath(caster, dir);
    return cast(path);
}

bool Ability::cast(vector<Position>& path)
{
    Character* target = current_room->getField(path.back())->getCharacter();
    if(target == nullptr)
    {
        UI::displayText(name_ + " misses...");
        return true;
    }
    return cast(target);
}

bool Ability::cast(Character *target)
{

    if (damage_ > 0)
    {
        UI::displayText(target->getName() + " gets hit for " + std::to_string(damage_) + " damage.");
        target->damage(damage_);
    }
    if(healing_ > 0)
    {
        UI::displayText(target->getName() + " gets healed for " + std::to_string(healing_) + " damage.");
        target->heal(healing_);
    }

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

    return true;
}

void Ability::coolDown()
{
    if(cooldown_left_ > 0)
        cooldown_left_--;
}

void Ability::putOnCooldown()
{
    cooldown_left_ = cooldown_ + 1; //+1 because cools down right after
}

vector<Position> Ability::getAbilityPath(Character* caster, Direction dir)
{
    //TODO: Implement Ability range
    vector<Position> path;
    Position pos = caster->getPosition();
    Field* current_field = nullptr;
    FIELD_STATUS status;
    do
    {
        pos           = pos + DELTA_POS[dir];
        current_field = current_room->getField(pos);
        status        = current_field->getFieldStatus();

        //special case for doors
        if(status == TRIGGER)
        {
            for(size_t i = 0; i < 4; i ++)
            {
                if(current_room->getDoorPosition((Direction)i) == pos)
                {
                    status = SOLID;
                    break;
                }
            }
        }
        path.push_back(pos);
    } while (status != SOLID && status != OCCUPIED);
    return path;
}



//-----------------------------------------------------------------------------------------------------//

Fireball::Fireball() : Ability("Fireball", "Deals additional damage over time.", 50, 0, 0, 50) 
{
    ab_effects_.push_back(new BurnEffect(10, 5));
    AnimatedSprite* anim = new AnimatedSprite("../images/ab_fireball_animated.png", 40);
    //effect_ = shared_ptr<Effect>(new MovingSprite(anim, 3));
}

bool Fireball::cast(Character* target)
{
    Position from = current_player->getPosition();
    Position to = target->getPosition();

    //dynamic_cast<MovingSprite*>(effect_.get())->aim(worldToScreen(from), worldToScreen(to));

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

//-----------------------------------------------------------------------------------------------------//

WildCharge::WildCharge() : Ability("Wild Charge", "Charges at the enemy", 70, 0, 10, 50)
{

}

bool WildCharge::cast(vector<Position>& path)
{
    //TODO: add effects

    //ParticleEffect pe(sf::Color::Red, 20);
    //static vector<shared_ptr<Effect>> effects;
    //effects.clear();
    Field* cur = nullptr;
    for(size_t i = 0; i < (path.size() - 1); i++)
    {
        //effects.push_back(shared_ptr<Effect>(pe.createInstance()));
        //effects.back()->setPosition(worldToScreen(path[i]));
        //Effect::addEffect(effects.back(), true);
        if(!caster_->move(path[i]))
        {
            break;
        }
    }
    return Ability::cast(path);
}