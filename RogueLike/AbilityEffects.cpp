#include "AbilityEffects.h"

OverTimeEffect::OverTimeEffect(string name, string descr, bool harmful, int amount, size_t dur) :
AbilityEffect(name, descr, dur), harmful_(harmful), amount_(amount)
{

}

void OverTimeEffect::apply(Character* target)
{
    //target->applyEffect(this);
}

void OverTimeEffect::tick()
{
    target_->damage(amount_);
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

void ConditionEffect::tick()
{
    //probably nothing
}

ConditionEffect::~ConditionEffect()
{
    //target->removeEffect() oder so
}

