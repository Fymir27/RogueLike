#include "AbilityEffects.h"

OverTimeEffect::OverTimeEffect(string name, string descr, bool harmful, int amount, size_t dur) :
AbilityEffect(name, descr, dur), harmful_(harmful), amount_(amount)
{

}

void OverTimeEffect::apply(Character* target)
{
    //target->applyEffect(this);
}

size_t OverTimeEffect::tick()
{
    target_->damage(amount_);
    return --dur_;
}

//-----------------------------------------------------------------------------//

StatEffect::StatEffect(string name, string descr, Stats delta, size_t dur) :
AbilityEffect(name, descr, dur), delta_(delta)
{

}

void StatEffect::apply(Character* target)
{
    //modify target stats;
}

StatEffect::~StatEffect()
{
    //remove modification
}

//-----------------------------------------------------------------------------//

ConditionEffect::ConditionEffect(string name, string descr, ConditionType type, size_t dur) :
AbilityEffect(name, descr, dur), type_(type)
{

}

void ConditionEffect::apply(Character* target)
{
    //target->applyEffect()
    //oder
    //target->setStatus()
}

size_t ConditionEffect::tick()
{
    return --dur_;
}

ConditionEffect::~ConditionEffect()
{
    //target->removeEffect() oder so
}

