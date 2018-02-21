#pragma once

#include "Common.h"
#include "Types.h"
#include "GameObject.h"
#include "AbilityEffectTypes.h"
#include "Effects.h"


class Character;

//TODO: Rework to add Effects properly
class Ability : public GameObject 
{
public:
    Ability(const string& name, const string& descr, unsigned damage, unsigned healing, size_t cd, size_t cost);
    virtual bool cast(Character* target); //returns if cast was successful
    void coolDown();

    size_t getCost() { return cost_; }
    size_t getCooldownLeft() { return cooldown_left_; }

protected:
    unsigned damage_ = 0;
    unsigned healing_ = 0;
    size_t cooldown_ = 0;
    size_t cooldown_left_ = 0;
    size_t cost_ = 0;

    vector<AbilityEffect*> ab_effects_;
    shared_ptr<Effect> effect_; //visual effect
};

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
