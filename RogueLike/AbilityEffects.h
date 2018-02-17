#pragma once

#include "Common.h"
#include "Types.h"
#include "GameObject.h"
#include "Character.h"

struct Stats;

class AbilityEffect : public GameObject
{
public:
    AbilityEffect(string name, string descr, size_t dur) : GameObject(name, descr), dur_(dur) {}
    virtual void apply(Character* target) = 0;
    virtual size_t tick() { return --dur_; }; //returns remaining duration
    virtual ~AbilityEffect() {};
protected:
    Character* target_;
    size_t dur_;
};


class OverTimeEffect : public AbilityEffect
{
public:
    OverTimeEffect(string name, string descr, bool harmful, int amount, size_t dur);
    void apply(Character* target);
    size_t tick();
    ~OverTimeEffect();
private:
    bool harmful_;
    int amount_;
};

class StatEffect : public AbilityEffect
{
public:
    StatEffect(string name, string descr, Stats delta, size_t dur);
    void apply(Character* target);
    ~StatEffect();
private:
    Stats delta_;
};

enum ConditionType
{
    STUN,
    FREEZE
};

class ConditionEffect : public AbilityEffect
{
public:
    ConditionEffect(string name, string descr, ConditionType type, size_t dur);
    void apply(Character* target);
    size_t tick();
    ~ConditionEffect();
private:
    ConditionType type_;  
};

