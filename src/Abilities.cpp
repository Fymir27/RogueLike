#include "Abilities.h"
#include "AbilityEffectTypes.h"
#include "Character.h"
#include "UI.h"
#include "Player.h"
#include "Effects.h"
#include "AbilityEffects.h"

Ability::Ability(string const& name, string const& descr, unsigned damage, unsigned healing,
                 size_t cd, size_t cost, size_t range) :
        GameObject(name, descr), damage_(damage), healing_(healing), cooldown_(cd), cost_(cost), range_(range)
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
    Character* target = getTargetInRange(path);
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
    }

    if(effect_ != nullptr)
    {
        //cout << "Adding Visual Effect" << endl;
        auto e = shared_ptr<Effect>(effect_->createInstance());
        current_room->addVisualEffect(e);
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
    } while (status != SOLID);
    return path;
}

vector<Character*> Ability::getTargetsInRange(vector<Position>& path, size_t max_targets)
{
    vector<Character*> targets;
    Character* tmp = nullptr;

    size_t range_counter = 0;
    for(auto const& pos : path)
    {
        range_counter++;
        tmp = current_room->getCharacter(pos);
        if(tmp != nullptr)
            targets.push_back(tmp);
        if(range_counter >= range_)
            break;
    }
    return targets;
}

Character* Ability::getTargetInRange(vector<Position>& path)
{
    auto targets = getTargetsInRange(path, 1);
    if(targets.empty())
        return nullptr;
    return targets.front();
}

//-----------------------------------------------------------------------------------------------------//

Fireball::Fireball() : Ability("Fireball", "Deals additional damage over time.", 50, 0, 0, 50, 50)
{
    ab_effects_.push_back(new BurnEffect(10, 5));
    AnimatedSprite* anim = new AnimatedSprite("images/ab_fireball_animated.png", 40);
    effect_ = shared_ptr<Effect>(new MovingSprite(anim, 5));
}

bool Fireball::cast(Character* target)
{
    Position from = current_player->getPosition();
    Position to = target->getPosition();

    dynamic_cast<MovingSprite*>(effect_.get())->aim(worldToScreen(from), worldToScreen(to));

    return Ability::cast(target);
}

//-----------------------------------------------------------------------------------------------------//

Regeneration::Regeneration() : Ability("Regeneration", "Heals over time", 0, 0, 3, 30, 50)
{
    ab_effects_.push_back(new RegenerationEffect(10, 5));
}

//-----------------------------------------------------------------------------------------------------//

SyphonSoul::SyphonSoul() : Ability("Syphon Soul", "Drains power from the enemy", 0, 0, 5, 70, 50)
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

WildCharge::WildCharge() : Ability("Wild Charge", "Charges at the enemy", 50, 0, 10, 25, 50)
{

}

bool WildCharge::cast(vector<Position>& path)
{
    //TODO: add effects

    Field* cur = nullptr;
    for(size_t i = 0; i < path.size(); i++)
    {
        if(current_room->getCharacter(path[i]) != nullptr)
        {
            return Ability::cast(current_room->getCharacter(path[i]));
        }
        if(!caster_->move(path[i]))
        {
            UI::displayText(name_ + " misses...");
            return true;
        }
    }
}

//-----------------------------------------------------------------------------------------------------//

ShatteringBlow::ShatteringBlow() :
        Ability("Shattering Blow", "Swings your weapon with all your might", 100, 0, 10, 25, 1)
{

}

//-----------------------------------------------------------------------------------------------------//

Shockwave::Shockwave() :
        Ability("Shockwave", "Your weapon causes a shockwave three fields in front of you", 70, 0, 20, 50, 3)
{

}

bool Shockwave::cast(vector<Position>& path)
{
    auto targets = getTargetsInRange(path, range_);

    if(targets.empty())
        UI::displayText(name_ + " misses...");

    for(auto target : targets)
        Ability::cast(target);

    return true;
}

//-----------------------------------------------------------------------------------------------------//

PoisonStab::PoisonStab() : Ability("Poison Stab", "Deals damage and inflicts Poision", 50, 0, 5, 30, 1)
{
    ab_effects_.push_back(new PoisonEffect(5, 10));
}

//-----------------------------------------------------------------------------------------------------//

ShadowStep::ShadowStep() : Ability("Shadow Step", "Teleports you behind your enemy and stabs them", 80, 0, 10, 40, 50)
{

}

bool ShadowStep::cast(vector<Position>& path)
{
    //keep infinite range?
    Character* target = nullptr;
    Position teleport_location;
    FIELD_STATUS status;
    for(size_t i = 0; i < path.size(); i++)
    {
        target = current_room->getCharacter(path.at(i));
        if(target == nullptr)
            continue;
        teleport_location = path.at(i+1);
        status = current_room->getField(teleport_location)->getFieldStatus();
        if(status == FREE || status == TRIGGER)
        {
            caster_->move(teleport_location);
            return Ability::cast(target);
        }
        else
        {
            UI::displayText("There's not enough space!");
            return false;
        }
    }
    UI::displayText(name_ + " misses...");
    return true;
}

//-----------------------------------------------------------------------------------------------------//

DaggerThrow::DaggerThrow() : Ability("Dagger Throw", "Throws your dagger at the enemy", 50, 0, 0, 20, 50)
{

}

//-----------------------------------------------------------------------------------------------------//
