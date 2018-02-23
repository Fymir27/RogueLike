#pragma once

#include "Common.h"
#include "Utils.h"
#include "GameObject.h"
#include "Character.h"
#include "Effects.h"

struct Stats;

class AbilityEffect : public GameObject
{
public:
    virtual size_t tick() { return --dur_; }; //returns remaining duration
    virtual AbilityEffect* createInstance(Character* target) = 0;
    virtual ~AbilityEffect();
protected:
    AbilityEffect(string const& name, string const& descr, size_t dur);
    AbilityEffect(AbilityEffect* orig, Character* target);

    Character* target_ = nullptr;
    shared_ptr<Effect> effect_; //visual effect
    size_t dur_;
};


class OverTimeEffect : public AbilityEffect
{
public:
    virtual size_t tick();
    virtual AbilityEffect* createInstance(Character* target) final;
protected:
    OverTimeEffect(string const& name, string const& descr, bool harmful, int amount, size_t dur);
    OverTimeEffect(OverTimeEffect* orig, Character* target);
private:
    bool harmful_;
    int amount_;
};

class StatEffect : public AbilityEffect
{
public:
    virtual AbilityEffect* createInstance(Character* target) final;
    ~StatEffect();
protected:
    StatEffect(string const& name, string const& descr, Stats delta, size_t dur);
    StatEffect(StatEffect* orig, Character* target);
private:
    Stats delta_;
};
