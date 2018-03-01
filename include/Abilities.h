#pragma once

#include "Common.h"
#include "Utils.h"
#include "GameObject.h"
#include "AbilityEffectTypes.h"
#include "Effects.h"


class Character;

//TODO: Rework to add Effects properly
class Ability : public GameObject
{
public:
    Ability(string const& name, string const& descr, unsigned damage, unsigned healing, size_t cd, size_t cost, size_t range);

    //called in exactly this order
    virtual bool cast(Character* caster, Direction dir, bool self);
    virtual bool cast(vector<Position>& path);
    virtual bool cast(Character* target);

    size_t getCost()
    { return cost_; }

    void coolDown();
    size_t getCooldownLeft()
    { return cooldown_left_; }
    void putOnCooldown();

protected:
    unsigned damage_ = 0;
    unsigned healing_ = 0;
    size_t cooldown_ = 0;
    size_t cooldown_left_ = 0;
    size_t cost_ = 0;
    size_t range_ = 0;

    Character* caster_ = nullptr;

    vector<AbilityEffect*> ab_effects_;
    shared_ptr<Effect> effect_; //visual effect

    vector<Position> getAbilityPath(Character* caster, Direction dir);
    vector<Character*> getTargetsInRange(vector<Position>& path, size_t max_targets);
    Character* getTargetInRange(vector<Position>& path);
};

//------- Mage ---------------------------------//

class Fireball : public Ability
{
public:
    Fireball();

    virtual bool cast(Character* target);
};

class Regeneration : public Ability
{
public:
    Regeneration();
};

class SyphonSoul : public Ability
{
public:
    SyphonSoul();

    virtual bool cast(Character* target);

private:
    AbilityEffect* buff_;
};

//------- Warrior ---------------------------------//

class WildCharge : public Ability
{
public:
    WildCharge();

    virtual bool cast(vector<Position>& path);
};

class ShatteringBlow : public Ability
{
public:
    ShatteringBlow();
};

class Shockwave : public Ability
{
public:
    Shockwave();

    virtual bool cast(vector<Position>& path);
};

//------- Thief ---------------------------------//

class PoisonStab : public Ability
{
public:
    PoisonStab();
};

class ShadowStep : public Ability
{
public:
    ShadowStep();

    virtual bool cast(vector<Position>& path);
};

class DaggerThrow : public Ability
{
public:
    DaggerThrow();
};

